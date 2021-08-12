#ifndef SORT_H_
#include "sort_tracker.h"
#include <iostream>
#include <fstream>
#include <map>
#include <random>
#include <chrono>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "sort_utils.h"
namespace SORT{

class Sort{

public:
    Tracker tracker;
    std::map<int, Track>  main2(std::vector<cv::Rect> detections);

};
}

#endif