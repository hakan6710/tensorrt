#ifndef mot_hpp_
#define mot_hpp_
#include "./detection/class_detector.hpp"
#include "./tracker/deep_sort/matching/tracker.h"
#include "./evaluation/kitti.h"

#include "../include/mot_config_files.hpp"
#include "./tracker/deep_sort.hpp"
#include "tracker/sort/include/sort.h"

namespace FHAC {

class mot{

public:
void init(string root_dir);
void detect(cv::Mat currentFrame);
void track(cv::Mat currentFrame);
std::vector<Track>  getResults();
void drawDetections(cv::Mat &currentFrame,DETECTIONS result);
void drawTracks(cv::Mat &currentFrame,std::vector<Track> result);
void drawTracks2(cv::Mat &currentFrame,std::map<int, SORT::Track>);

std::map<int, SORT::Track> getResults_sort();

private:
deep_sort _deepSort;
SORT::Sort _sort;
std::unique_ptr<Detector> _detector;
DETECTIONS _currentDetections;
Config getDetectorConfig();
std::unique_ptr<Detector> InitDetector(string root_dir);
DETECTIONS getNewDetections(Detector &detector,cv::Mat currentFrame);

std::map<int, SORT::Track> sortTracks;

};


}


#endif
