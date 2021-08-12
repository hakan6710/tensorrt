

#include <memory>
#include <thread>
#include <chrono>

#include <vector>
#include <string>
#include <fstream>

#include "mot.hpp"

#include <mot_config_files.hpp>



using namespace cv;
using namespace std;


namespace FHAC{


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

Config mot::getDetectorConfig()
{
	
	Config config_v4;
	config_v4.net_type = YOLOV4;
	config_v4.file_model_cfg = YOLO_V4_CONFIG;
	config_v4.file_model_weights = YOLO_V4_WEIGHTS;
	config_v4.calibration_image_list_file_txt = YOLO_V4_CALIBRATION;
	config_v4.inference_precison =FP32;
	config_v4.detect_thresh = 0.5;


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


std::unique_ptr<Detector> mot::InitDetector(string root_dir){
	Config cfg=getDetectorConfig();
	cfg.detect_thresh=0.3;
	std::unique_ptr<Detector> detector(new Detector());

	//Add the root_dir
	cfg.calibration_image_list_file_txt= root_dir+cfg.calibration_image_list_file_txt;
	cfg.file_model_cfg=root_dir+cfg.file_model_cfg;
	cfg.file_model_weights=root_dir+cfg.file_model_weights;

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

void mot::drawTracks2(cv::Mat &currentFrame, std::map<int, SORT::Track> tracks){
	char fname[255], showMsg[100];
	for(unsigned int k = 0; k < tracks.size(); k++) {
		
		cv::Rect rect = tracks[k].GetStateAsBbox();
		cv::rectangle(currentFrame, rect, Scalar(255, 255, 0), 2);
		sprintf(showMsg, "%d classid=%d", k, 0);
		
		putText(currentFrame, showMsg, cv::Point(rect.x, rect.y), cv::FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 255, 0), 2);
	}

}

void mot::init(string root_dir){

	_detector=InitDetector(root_dir);
	//_deepSort.init(root_dir);

};
void mot::detect(cv::Mat currentFrame){
	_currentDetections=getNewDetections(*_detector,currentFrame);
};
void mot::track(cv::Mat currentFrame){
	//_deepSort.track(currentFrame, _currentDetections);
	this->sortTracks=_sort.main2(castTheData(_currentDetections));
};
std::vector<Track>  mot::getResults(){
	return _deepSort.getTracks();
};

std::map<int, SORT::Track> mot::getResults_sort(){
	return this->sortTracks;
} 





}











