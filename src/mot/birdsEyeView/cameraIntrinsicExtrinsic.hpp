#ifndef cameraIntrinsicExtrinsic_hpp_
#define cameraIntrinsicExtrinsic_hpp_
#include <opencv2/opencv.hpp>
namespace FHAC{

//ROS Calibrator Output projection= [R|t]
float rotation_values[] = {1.0000000,  0.0000000,  0.0000000,
   0.0000000,  0.9743701,  0.2249511,
   0.0000000, -0.2249511,  0.9743701  };
cv::Mat_<float> rotation_matrix(3, 3, rotation_values);

float transform_values[] = {0.785,
                            1.21,
                            0.63};
cv::Mat_<float> transform_matrix(1, 3, transform_values);



//ROS Calibrator Output camera_matrix=intrinsic values 
float intrinsic_values[] = {989.437842, 0.000000, 980.319408, 
                            0.000000, 996.522603, 658.606498, 
                            0.000000, 0.000000, 1.000000};
cv::Mat_<float> intrinsic_calibration_matrix(3, 3, intrinsic_values);


//distortion coefficients
// (k1,k2,p1,p2,k3)
float dist_coefficients[]= {-0.299377, 0.069821, -0.000843, -0.001001, 0.000000};
cv::Mat_<float> distortion_matrix(1, 5, dist_coefficients);
}



#endif