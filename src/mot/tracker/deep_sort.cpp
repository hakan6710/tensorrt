#include "deep_sort.hpp"

namespace FHAC{

void deep_sort::init(){

    
    _featureTensor=FeatureTensor::getInstance();

}

std::vector<Track>   deep_sort::getTracks(){

    std::vector<Track> result;
    for(Track& track: _tracker.tracks){
		if (!track.is_confirmed() || track.time_since_update > 1) continue;
		result.push_back(track);
	}

	
    return result;
   
}

void  deep_sort::track(cv::Mat currentFrame, DETECTIONS detections){
    

    if(_featureTensor->getRectsFeature(currentFrame, detections) == false) {
		throw std::runtime_error("Error Tensorflow get rects"); 
	}

    _tracker.predict();
	_tracker.update(detections);

}




}