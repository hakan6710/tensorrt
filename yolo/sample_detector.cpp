#include "./extra/class_timer.hpp"
#include "./modules/class_detector.h"
#include "../deep_sort/matching/tracker.h"
#include "../deep_sort/feature/FeatureTensor.h"
#include "../evaluation/kitti.h"

#include <memory>
#include <thread>
#include <chrono>

#include <vector>
#include <string>
#include <fstream>

#define args_nn_budget 100
#define args_max_cosine_distance 0.2
#define args_min_confidence 0.3
#define args_nms_max_overlap 1.0
using namespace cv;
using namespace std;


cv::VideoCapture *cap;

cv::Mat getNextFrame()
{
  cv::Mat frame;
  *cap>>frame;

  return frame;	

}

void InitCap(){
	cap=new cv::VideoCapture("../data/aufnahme_Fahrt_Aachen.h264");
	if(!cap->isOpened()){
		throw std::runtime_error("VideoCap broken!"); 
	}
}

Config getDetectorConfig()
{
	Config config_v3;
	config_v3.net_type = YOLOV3;
	config_v3.file_model_cfg = "../yolo/configs/yolov3.cfg";
	config_v3.file_model_weights = "../yolo/configs/yolov3.weights";
	config_v3.calibration_image_list_file_txt = "../yolo/configs/calibration_images.txt";
	config_v3.inference_precison =FP32;
	config_v3.detect_thresh = 0.5;

	Config config_v3_tiny;
	config_v3_tiny.net_type = YOLOV3_TINY;
	config_v3_tiny.detect_thresh = 0.7;
	config_v3_tiny.file_model_cfg = "../yolo/configs/yolov3-tiny.cfg";
	config_v3_tiny.file_model_weights = "../yolo/configs/yolov3-tiny.weights";
	config_v3_tiny.calibration_image_list_file_txt = "../yolo/configs/calibration_images.txt";
	config_v3_tiny.inference_precison = FP32;

	Config config_v4;
	config_v4.net_type = YOLOV4;
	config_v4.file_model_cfg = "../yolo/configs/yolov4.cfg";
	config_v4.file_model_weights = "../yolo/configs/yolov4.weights";
	config_v4.calibration_image_list_file_txt = "../yolo/configs/calibration_images.txt";
	config_v4.inference_precison =FP32;
	config_v4.detect_thresh = 0.5;

	Config config_v4_tiny;
	config_v4_tiny.net_type = YOLOV4_TINY;
	config_v4_tiny.detect_thresh = 0.5;
	config_v4_tiny.file_model_cfg = "../yolo/configs/yolov4-tiny.cfg";
	config_v4_tiny.file_model_weights = "../yolo/configs/yolov4-tiny.weights";
	config_v4_tiny.calibration_image_list_file_txt = "../yolo/configs/calibration_images.txt";
	config_v4_tiny.inference_precison = FP32;

	Config config_v5;
	config_v5.net_type = YOLOV5;
	config_v5.detect_thresh = 0.5;
	config_v5.file_model_cfg = "../yolo/configs/yolov5-4.0/yolov5s.cfg";
	config_v5.file_model_weights = "../yolo/configs/yolov5-4.0/yolov5s.weights";
	config_v5.calibration_image_list_file_txt = "../yolo/configs/calibration_images.txt";
	config_v5.inference_precison = FP32;

	return config_v4;
}

string directory="/home/fze2/Desktop/kitti_data/testing/image_02/";

DETECTIONS getNewDetections(Detector &detector,cv::Mat currentFrame){

	DETECTIONS detections;
	std::vector<cv::Mat> batch_img;
	batch_img.push_back(currentFrame);


	//Popupalte detections for currentFrame
	detector.detect2(batch_img, detections);

	//TENSORFLOW get rect's feature.
	if(FeatureTensor::getInstance()->getRectsFeature(currentFrame, detections) == false) {
		throw std::runtime_error("Error Tensorflow get rects"); 
	}

	return detections;

}

std::vector<Track> trackWithNewDetections(tracker &tracker, DETECTIONS &detections){
	
	tracker.predict();
	tracker.update(detections);

	std::vector<Track> result;
	
	for(Track& track: tracker.tracks){
		if (!track.is_confirmed() || track.time_since_update > 1) continue;
		result.push_back(track);
	}

	return result;
}

std::unique_ptr<Detector> InitDetector(){
	Config cfg=getDetectorConfig();
	cfg.detect_thresh=0.3;
	std::unique_ptr<Detector> detector(new Detector());
	detector->init(cfg);

	return detector;
}

void drawTracks(cv::Mat &currentFrame,std::vector<Track> result){
	
	char fname[255], showMsg[100];
			
	for(unsigned int k = 0; k < result.size(); k++) {
		DETECTBOX tmp = result[k].to_tlwh();
		cv::Rect rect = cv::Rect(tmp(0), tmp(1), tmp(2), tmp(3));
		cv::rectangle(currentFrame, rect, Scalar(255, 255, 0), 2);
		sprintf(showMsg, "%d classid=%d", result[k].track_id, result[k].class_id);
		
		putText(currentFrame, showMsg, cv::Point(rect.x, rect.y), cv::FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 255, 0), 2);
	}
}
//0 -1 DontCare -1 -1 -10.000000 219.310000 188.490000 245.500000 218.560000 -1000.000000 -1000.000000 
//-1000.000000 -10.000000 -1.000000 -1.000000 -1.000000

//https://github.com/JonathonLuiten/TrackEval/blob/master/docs/KITTI-format.txt Template for textfile
void writeTracks(std::vector<Track> result, string fileName,int frameId){

	for(unsigned int k = 0; k < result.size(); k++) {
		Track t=result[k];
		int class_id=result[k].class_id;

		string class_name;
		switch (class_id){
			//person -> Pedestrian
			case 0: 
				class_name="Pedestrian";
				break;
			
			//bicycle->
			case 1:
				class_name="Cyclist";
				break;

			//car->
			case 2:
				class_name="Car";
				break;

			//motorbike->
			case 3:
				class_name="DontCare";
				break;

			//bus->
			case 5:
				class_name="Car";
				break;

			//truck->
			case 7:
				class_name="Car";
				break;
			
			default:
				class_name="DontCare";
				break;
		}


		string dRow="";
		string spacer=" ";

		//frameid
		dRow+=std::to_string(frameId);
		//track id
		dRow+=spacer+std::to_string(t.track_id);
		//type
		dRow+=spacer+class_name;
		//truncated
		dRow+=spacer+std::to_string(-1);
		//occluded
		dRow+=spacer+std::to_string(-1);
		//alpha
		dRow+=spacer+std::to_string(-10.000000);

		//bbox
		DETECTBOX db=t.to_tlwh();
		dRow+=spacer+std::to_string(db(0));
		dRow+=spacer+std::to_string(db(1));
		dRow+=spacer+std::to_string(db(0)+db(2));
		dRow+=spacer+std::to_string(db(1)+db(3));
		
		//dimensions
		dRow+=spacer+std::to_string(-1000.000000);
		dRow+=spacer+std::to_string(-1000.000000);
		dRow+=spacer+std::to_string(-1000.000000);

		//location
		dRow+=spacer+std::to_string(-1000.000000);
		dRow+=spacer+std::to_string(-1000.000000);
		dRow+=spacer+std::to_string(-1000.000000);

		//rotation_y
		dRow+=spacer+std::to_string(-1.000000);

		//score
		dRow+=spacer+std::to_string(-1.000000);

		ofstream myfile;
    	myfile.open (fileName+".txt",ios::out | ios::app);
    	myfile << dRow <<"\n";
    	myfile.close();


	}

}

int main()
{
	
	auto mapWithFolderNames=KITTI_EVA::getFolderPathes(directory);
	

	for (auto item:mapWithFolderNames){
		auto mapWithFileNames=KITTI_EVA::getFilePathes(item.second);

		cv::Mat currentFrame;
		DETECTIONS detections;
		tracker tracker;
		std::unique_ptr<Detector> detector=InitDetector();

		for(auto i2:mapWithFileNames){

			std::string image_path = samples::findFile(i2.second);
    		currentFrame = imread(image_path, IMREAD_COLOR);
			
			
			//detect
			auto start= std::chrono::steady_clock::now();
			detections=getNewDetections(*detector, currentFrame);
			

			std::vector<Track> result=trackWithNewDetections(tracker,detections);
			
			drawTracks(currentFrame,result);
			writeTracks(result, item.first,i2.first);

			imshow("DeepSortTracking", currentFrame);
			waitKey(10);
		}
		
	}
		
	cv::destroyAllWindows();
	
}



//0 -1 DontCare -1 -1 -10.000000 219.310000 188.490000 245.500000 218.560000 -1000.000000 -1000.000000 -1000.000000 -10.000000 -1.000000 -1.000000 -1.000000






