#include "objectTracker.hpp"
#include <mot_config_files.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;

namespace FHAC{

void objectTracker::init(){
    
}

void objectTracker::test(){

    auto mapWithFolderNames=KITTI_EVA::getFolderPathes(KITTI_Directory);
	for (auto item:mapWithFolderNames){
		auto mapWithFileNames=KITTI_EVA::getFilePathes(item.second);

		cv::Mat currentFrame;
        mot currentMot;
        currentMot.init();
		int counter=0;
		for(auto i2:mapWithFileNames){
			
			std::string image_path = samples::findFile(i2.second);
    		currentFrame = imread(image_path, IMREAD_COLOR);
			
			//detect
			auto start= std::chrono::steady_clock::now();

            currentMot.detect(currentFrame);
            currentMot.track(currentFrame);
            std::vector<Track> results= currentMot.getResults();

            currentMot.drawTracks(currentFrame,results);


			imshow("DeepSortTracking", currentFrame);
			waitKey(10);
			counter++;
			if(counter>10)
				break;
		}
		break;
		
	}
		
	cv::destroyAllWindows();


}





}