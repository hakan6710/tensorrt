

#include <memory>
#include <thread>
#include <chrono>

#include <vector>
#include <string>
#include <fstream>

#include "mot.hpp"


using namespace cv;
using namespace std;


namespace FHAC{

Config mot::getDetectorConfig()
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
	config_v4.file_model_cfg = "/home/fze2/Desktop/yolo-tensorrt/yolo/configs/yolov4.cfg";
	config_v4.file_model_weights = "/home/fze2/Desktop/yolo-tensorrt/yolo/configs/yolov4.weights";
	config_v4.calibration_image_list_file_txt = "/home/fze2/Desktop/yolo-tensorrt/yolo/configs/calibration_images.txt";
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


DETECTIONS mot::getNewDetections(Detector &detector,cv::Mat currentFrame){

	DETECTIONS detections;
	std::vector<cv::Mat> batch_img;
	batch_img.push_back(currentFrame);

	//Popupalte detections for currentFrame
	detector.detect2(batch_img, detections);
	
	
	return detections;

}


std::unique_ptr<Detector> mot::InitDetector(){
	Config cfg=getDetectorConfig();
	cfg.detect_thresh=0.3;
	std::unique_ptr<Detector> detector(new Detector());
	
	detector->init(cfg);

	return detector;
}




void mot::drawDetections(cv::Mat &currentFrame,DETECTIONS result){
	
	char fname[255], showMsg[100];
	for (auto item:result){
		DETECTBOX tmp = item.tlwh;
		cv::Rect rect = cv::Rect(tmp(0), tmp(1), tmp(2), tmp(3));
		cv::rectangle(currentFrame, rect, Scalar(255, 255, 0), 2);
	}	

}
void mot::drawTracks(cv::Mat &currentFrame,std::vector<Track> result){
	
	char fname[255], showMsg[100];
			
	for(unsigned int k = 0; k < result.size(); k++) {
		DETECTBOX tmp = result[k].to_tlwh();
		cv::Rect rect = cv::Rect(tmp(0), tmp(1), tmp(2), tmp(3));
		cv::rectangle(currentFrame, rect, Scalar(255, 255, 0), 2);
		sprintf(showMsg, "%d classid=%d", result[k].track_id, result[k].class_id);
		
		putText(currentFrame, showMsg, cv::Point(rect.x, rect.y), cv::FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 255, 0), 2);
	}

	


}

void mot::init(){

	_detector=InitDetector();
	_deepSort.init();

};
void mot::detect(cv::Mat currentFrame){
	_currentDetections=getNewDetections(*_detector,currentFrame);
};
void mot::track(cv::Mat currentFrame){
	_deepSort.track(currentFrame, _currentDetections);
};
std::vector<Track>  mot::getResults(){
	return _deepSort.getTracks();
};



std::vector<cv::Rect> castTheData(DETECTIONS result){
	
	std::vector<cv::Rect> detect_rec;
	for(int i=0; i<result.size(); i++){
			cv::Rect temp;
			temp.x=result[i].tlwh[0];
			temp.y=result[i].tlwh[1];
			temp.width=result[i].tlwh[2];
			temp.height=result[i].tlwh[3];

			detect_rec.push_back(temp);
	}
	return detect_rec;
}


}











