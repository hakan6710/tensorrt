#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/protobuf/meta_graph.pb.h"

#include "model.h"
#include "dataType.h"
#include <string>

typedef unsigned char uint8;

class FeatureTensor
{
public:
	static FeatureTensor* getInstance(std::string root_dir);
	bool getRectsFeature(const cv::Mat& img, DETECTIONS& d);
	void killIt();

private:
	FeatureTensor(std::string root_dir);
	FeatureTensor(const FeatureTensor&);
	FeatureTensor& operator = (const FeatureTensor&);
	static FeatureTensor* instance;
	bool init(std::string root_dir);
	~FeatureTensor();
	

	void tobuffer(const std::vector<cv::Mat> &imgs, uint8 *buf);

	int feature_dim;
	tensorflow::Session* session;
	std::vector<tensorflow::Tensor> output_tensors;
	std::vector<std::string> outnames;
	tensorflow::string input_layer;
};
