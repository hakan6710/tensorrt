#include "objectTracker.hpp"
#include <mot_config_files.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;

namespace FHAC{

void objectTracker::init(){
    
}

void objectTracker::test(string root_dir){

    auto mapWithFolderNames=KITTI_EVA::getFolderPathes(KITTI_Directory);
	for (auto item:mapWithFolderNames){
		auto mapWithFileNames=KITTI_EVA::getFilePathes(item.second);

		cv::Mat currentFrame;
        mot currentMot;
        currentMot.init(root_dir);
		int counter=0;
		for(auto i2:mapWithFileNames){
			
			std::string image_path = samples::findFile(i2.second);
    		currentFrame = imread(image_path, IMREAD_COLOR);
			
			//detect
			auto start= std::chrono::steady_clock::now();

            currentMot.detect(currentFrame);
            currentMot.track(currentFrame);

			std::vector<Track>  results= currentMot.getResults();
			currentMot.drawTracks(currentFrame,results);

			//Calls For Sort   
			//std::map<int, SORT::Track> results= currentMot.getResults_sort();
            //currentMot.drawTracks2(currentFrame,results);


			imshow("DeepSortTracking", currentFrame);
			waitKey(10);
			counter++;
		}
		break;
		
	}
		
	cv::destroyAllWindows();


}





}