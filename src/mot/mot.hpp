#ifndef mot_hpp_
#define mot_hpp_
#include "./detection/class_detector.hpp"
#include "./tracker/deep_sort/matching/tracker.h"
#include "./evaluation/kitti.h"

#include "../../include/mot_config_files.hpp"
#include "./tracker/deep_sort.hpp"

namespace FHAC {

class mot{

public:
void init();
void detect(cv::Mat currentFrame);
void track(cv::Mat currentFrame);
std::vector<Track>  getResults();
void drawDetections(cv::Mat &currentFrame,DETECTIONS result);
void drawTracks(cv::Mat &currentFrame,std::vector<Track> result);


private:
deep_sort _deepSort;
std::unique_ptr<Detector> _detector;
DETECTIONS _currentDetections;
Config getDetectorConfig();
std::unique_ptr<Detector> InitDetector();
DETECTIONS getNewDetections(Detector &detector,cv::Mat currentFrame);


};


}


#endif
