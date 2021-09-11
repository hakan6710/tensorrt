#include "projection.hpp"
#include "cameraIntrinsicExtrinsic.hpp"
using std::placeholders::_1;
namespace FHAC{


//cv::undistortPoints (InputArray src, OutputArray dst, InputArray cameraMatrix, InputArray distCoeffs, InputArray R=noArray(), InputArray P=noArray())


void calculateProjection(cv::Mat_<cv::Point2f> pointMatrix,std::vector<projectedPoint> &pPoints ){

    cv::Mat undistortedAndNormalizedPointMatrix;

    cv::Mat_<cv::Point2f> test(pointMatrix);


    //distcoefficients()
    cv::undistortPoints(pointMatrix, undistortedAndNormalizedPointMatrix, intrinsic_calibration_matrix, cv::noArray());

    
    int index=0;
    for (cv::MatIterator_<cv::Point2f> i = undistortedAndNormalizedPointMatrix.begin<cv::Point2f>(); 
    i != undistortedAndNormalizedPointMatrix.end<cv::Point2f>(); ++i, index++)
    {
        float norm[] = {(*i).x, (*i).y, 1};
        cv::Mat rotated_ray = cv::Mat_<float>(1, 3, norm) * rotation_matrix.inv();
        cv::Mat rotated_normalised_ray = rotated_ray / rotated_ray.at<float>(2);
        float xGround = transform_matrix.at<float>(2) * rotated_normalised_ray.at<float>(0) + transform_matrix.at<float>(0);
        float yGround = transform_matrix.at<float>(2) * rotated_normalised_ray.at<float>(1) + transform_matrix.at<float>(1);

        pPoints[index].setProjectedPoint(xGround,yGround);

    }
    
}
    //  CV_Assert( CV_IS_MAT(_src) && CV_IS_MAT(_dst) &&
    //     (_src->rows == 1 || _src->cols == 1) &&
    //     (_dst->rows == 1 || _dst->cols == 1) &&
    //     _src->cols + _src->rows - 1 == _dst->rows + _dst->cols - 1 &&
    //     (CV_MAT_TYPE(_src->type) == CV_32FC2 || CV_MAT_TYPE(_src->type) == CV_64FC2) &&
    //     (CV_MAT_TYPE(_dst->type) == CV_32FC2 || CV_MAT_TYPE(_dst->type) == CV_64FC2));





    
}