#include "./class_detector.hpp"
#include "./yolo/modules/class_yolo_detector.hpp"

class Detector::Impl
{
public:
	Impl() {}

	~Impl(){}

	YoloDectector _detector;
};

Detector::Detector()
{
	_impl = new Impl();
}

Detector::~Detector()
{
	if (_impl)
	{
		delete _impl;
		_impl = nullptr;
	}
}

void Detector::init(const Config &config)
{
	_impl->_detector.init(config);
}

void Detector::detect(const std::vector<cv::Mat> &mat_image, std::vector<BatchResult> &vec_batch_result)
{
	_impl->_detector.detect(mat_image, vec_batch_result);
}

void Detector::detect2(const std::vector<cv::Mat>	&vec_image,DETECTIONS &detections)
{
	_impl->_detector.detect2(vec_image, detections);
}
