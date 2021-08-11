/**
 * SORT: A Simple, Online and Realtime Tracker
 */

#include "../include/sort.h"

namespace SORT{

    
int Sort::main2(std::vector<cv::Rect> detections) {
        // create SORT tracker
        
        /*** Run SORT tracker ***/
        std::vector<std::vector<cv::Rect>> all_detections;
        tracker.Run(detections);
            
        const auto tracks = tracker.GetTracks();
        /*** Tracker update done ***/

}





}