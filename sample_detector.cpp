#include "./extra/class_timer.hpp"
#include "./modules/class_detector.h"
#include "./deep_sort/matching/tracker.h"
#include "./deep_sort/feature/FeatureTensor.h"


#include <memory>
#include <thread>
#include <chrono>

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
	cap=new cv::VideoCapture("../aufnahme_Fahrt_Aachen.h264");
	if(!cap->isOpened()){
		throw std::runtime_error("VideoCap broken!"); 
	}

}

Config getDetectorConfig()
{
	Config config_v3;
	config_v3.net_type = YOLOV3;
	config_v3.file_model_cfg = "../configs/yolov3.cfg";
	config_v3.file_model_weights = "../configs/yolov3.weights";
	config_v3.calibration_image_list_file_txt = "../configs/calibration_images.txt";
	config_v3.inference_precison =FP32;
	config_v3.detect_thresh = 0.5;

	Config config_v3_tiny;
	config_v3_tiny.net_type = YOLOV3_TINY;
	config_v3_tiny.detect_thresh = 0.7;
	config_v3_tiny.file_model_cfg = "../configs/yolov3-tiny.cfg";
	config_v3_tiny.file_model_weights = "../configs/yolov3-tiny.weights";
	config_v3_tiny.calibration_image_list_file_txt = "../configs/calibration_images.txt";
	config_v3_tiny.inference_precison = FP32;

	Config config_v4;
	config_v4.net_type = YOLOV4;
	config_v4.file_model_cfg = "../configs/yolov4.cfg";
	config_v4.file_model_weights = "../configs/yolov4.weights";
	config_v4.calibration_image_list_file_txt = "../configs/calibration_images.txt";
	config_v4.inference_precison =FP32;
	config_v4.detect_thresh = 0.5;

	Config config_v4_tiny;
	config_v4_tiny.net_type = YOLOV4_TINY;
	config_v4_tiny.detect_thresh = 0.5;
	config_v4_tiny.file_model_cfg = "../configs/yolov4-tiny.cfg";
	config_v4_tiny.file_model_weights = "../configs/yolov4-tiny.weights";
	config_v4_tiny.calibration_image_list_file_txt = "../configs/calibration_images.txt";
	config_v4_tiny.inference_precison = FP32;

	Config config_v5;
	config_v5.net_type = YOLOV5;
	config_v5.detect_thresh = 0.5;
	config_v5.file_model_cfg = "../configs/yolov5-4.0/yolov5s.cfg";
	config_v5.file_model_weights = "../configs/yolov5-4.0/yolov5s.weights";
	config_v5.calibration_image_list_file_txt = "../configs/calibration_images.txt";
	config_v5.inference_precison = FP32;

	return config_v4;
}

int main()
{
	
	tracker t;
	Timer timer;
	InitCap();
	cv::Mat currentFrame;
	int counter=0;
	currentFrame=getNextFrame();
	Config cfg=getDetectorConfig();
	std::unique_ptr<Detector> detector(new Detector());
	detector->init(cfg);

	DETECTIONS detections;
	while(!(currentFrame=getNextFrame()).empty()){
		std::vector<cv::Mat> batch_img;
		batch_img.push_back(currentFrame);

		//detect
		
		auto start= std::chrono::steady_clock::now();

		//std::cout<<"Detect Count="<<detections.size()<<"\n";

		detector->detect2(batch_img, detections);
		//TENSORFLOW get rect's feature.
		if(FeatureTensor::getInstance()->getRectsFeature(currentFrame, detections) == false) {
			std::cout<<"Error Tensorflow get rects";
		}

		t.predict();
		t.update(detections);

		


		std::vector<RESULT_DATA> result;
		
		for(Track& track: t.tracks){
			if (!track.is_confirmed() || track.time_since_update > 5) continue;
			result.push_back(std::make_pair(track.track_id, track.to_tlwh()));
		}
		char fname[255], showMsg[10];
		
		for(unsigned int k = 0; k < result.size(); k++) {
			DETECTBOX tmp = result[k].second;
			cv::Rect rect = cv::Rect(tmp(0), tmp(1), tmp(2), tmp(3));
			cv::rectangle(currentFrame, rect, Scalar(255, 255, 0), 2);
			sprintf(showMsg, "%d", result[k].first);
			putText(currentFrame, showMsg, cv::Point(rect.x, rect.y), cv::FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 255, 0), 2);
		}
		auto end= std::chrono::steady_clock::now();

		double elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		double fps=1000/elapsed;

		std::stringstream stream2;
		stream2<<"FPS="<<fps<<" Inference time="<<elapsed<<" "<<cfg.file_model_weights;
		cv:putText(currentFrame,stream2.str(),cv::Point(20,50),0, 0.5, cv::Scalar(0, 0, 255), 2);


		imshow("DeepSortTracking", currentFrame);
		waitKey(10);
	
	}
	
	cap->release();
	cv::destroyAllWindows();
	
}




