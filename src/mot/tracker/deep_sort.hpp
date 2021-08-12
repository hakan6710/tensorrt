#ifndef deep_sort_hpp_
#define deep_sort_hpp_

#include "./deep_sort/matching/tracker.h"
#include <opencv2/opencv.hpp>
#include "./deep_sort/feature/FeatureTensor.h"
namespace FHAC{


class deep_sort{

public: 
    void init(std::string root_dir);
    void track(cv::Mat currentFrame, DETECTIONS detections);
    std::vector<Track>  getTracks();

private: 
    tracker _tracker;
    FeatureTensor *_featureTensor;
};

}


#endif