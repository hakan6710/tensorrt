/**
 * SORT: A Simple, Online and Realtime Tracker
 */

#include "../include/sort.h"

namespace SORT{

    
std::map<int, Track>  Sort::main2(std::vector<cv::Rect> detections) {
        
        tracker.Run(detections);
            
        std::map<int, Track> tracks = tracker.GetTracks();
        /*** Tracker update done ***/
        return tracks;
}       
}