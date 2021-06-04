#include "./extra/class_timer.hpp"
#include "./modules/class_detector.h"
#include "./deep_sort/matching/tracker.h"
#include "./deep_sort/feature/FeatureTensor.h"


#include <memory>
#include <thread>
#include <chrono>

#define args_nn_budget 100
#define args_max_cosine_distance 0.2
#define args_min_confidence 0.3
#define args_nms_max_overlap 1.0

cv::VideoCapture *cap;

cv::Mat getNextFrame()
{
  cv::Mat frame;
  *cap>>frame;

  return frame;	

}

void InitCap(){
	cap=new cv::VideoCapture("../aufnahme_Fahrt_Aachen.h264");
	if(!cap->isOpened()){
		throw std::runtime_error("VideoCap broken!"); 
	}

}

Config getDetectorConfig()
{
	Config config_v3;
	config_v3.net_type = YOLOV3;
	config_v3.file_model_cfg = "../configs/yolov3.cfg";
	config_v3.file_model_weights = "../configs/yolov3.weights";
	config_v3.calibration_image_list_file_txt = "../configs/calibration_images.txt";
	config_v3.inference_precison =FP32;
	config_v3.detect_thresh = 0.5;

	Config config_v3_tiny;
	config_v3_tiny.net_type = YOLOV3_TINY;
	config_v3_tiny.detect_thresh = 0.7;
	config_v3_tiny.file_model_cfg = "../configs/yolov3-tiny.cfg";
	config_v3_tiny.file_model_weights = "../configs/yolov3-tiny.weights";
	config_v3_tiny.calibration_image_list_file_txt = "../configs/calibration_images.txt";
	config_v3_tiny.inference_precison = FP32;

	Config config_v4;
	config_v4.net_type = YOLOV4;
	config_v4.file_model_cfg = "../configs/yolov4.cfg";
	config_v4.file_model_weights = "../configs/yolov4.weights";
	config_v4.calibration_image_list_file_txt = "../configs/calibration_images.txt";
	config_v4.inference_precison =FP32;
	config_v4.detect_thresh = 0.5;

	Config config_v4_tiny;
	config_v4_tiny.net_type = YOLOV4_TINY;
	config_v4_tiny.detect_thresh = 0.5;
	config_v4_tiny.file_model_cfg = "../configs/yolov4-tiny.cfg";
	config_v4_tiny.file_model_weights = "../configs/yolov4-tiny.weights";
	config_v4_tiny.calibration_image_list_file_txt = "../configs/calibration_images.txt";
	config_v4_tiny.inference_precison = FP32;

	Config config_v5;
	config_v5.net_type = YOLOV5;
	config_v5.detect_thresh = 0.5;
	config_v5.file_model_cfg = "../configs/yolov5-4.0/yolov5s.cfg";
	config_v5.file_model_weights = "../configs/yolov5-4.0/yolov5s.weights";
	config_v5.calibration_image_list_file_txt = "../configs/calibration_images.txt";
	config_v5.inference_precison = FP32;

	return config_v4;
}

int main()
{
	tracker t;
	Timer timer;
	InitCap();
	cv::Mat currentFrame;
	int counter=0;
	currentFrame=getNextFrame();
	Config cfg=getDetectorConfig();
	std::unique_ptr<Detector> detector(new Detector());
	detector->init(cfg);

	
	while(!(currentFrame=getNextFrame()).empty()){
		std::vector<BatchResult> batch_res;
		std::vector<cv::Mat> batch_img;
		batch_img.push_back(currentFrame);

		//detect
		
		auto start= std::chrono::steady_clock::now();


		detector->detect(batch_img, batch_res);
		auto end= std::chrono::steady_clock::now();

		double elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		double fps=1000/elapsed;

		std::stringstream stream2;
		stream2<<"FPS="<<fps<<" Inference time="<<elapsed<<" "<<cfg.file_model_weights;
		cv:putText(batch_img[0],stream2.str(),cv::Point(20,50),0, 0.5, cv::Scalar(0, 0, 255), 2);
		//display
		for (int i=0;i<batch_img.size();++i)
		{
			for (const auto &r : batch_res[i])
			{
				std::cout <<"batch "<<i<< " id:" << r.id << " prob:" << r.prob << " rect:" << r.rect << std::endl;
				cv::rectangle(batch_img[i], r.rect, cv::Scalar(255, 0, 0), 2);
				std::stringstream stream;

				stream << std::fixed << std::setprecision(2) << "id:" << r.id << "  score:" << r.prob;
				stream2<<"Test Test";
				cv::putText(batch_img[i], stream.str(), cv::Point(r.rect.x, r.rect.y - 5), 0, 0.5, cv::Scalar(0, 0, 255), 2);
				

			}
			cv::imshow("imag2e"+std::to_string(i), batch_img[i]);
		}
		cv::waitKey(10);
	
	}
	
	cap->release();
	cv::destroyAllWindows();
	
}


