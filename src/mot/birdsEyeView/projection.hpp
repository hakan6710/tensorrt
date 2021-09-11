#ifndef projection_hpp_
#define projection_hpp_
#include <math.h>
#include <vector>
#include <opencv2/opencv.hpp>

namespace FHAC{


class projectedPoint{
    
public:
    //Euclidean distance
    //p1=(0,0)  p2(x,y) 
    //no z, because always on ground 
    projectedPoint(float x_pic, float y_pic){
        this->x_picture=x_pic;
        this->y_picture=y_pic;
    }
    float x_projected;
    float y_projected;
    float distance;

    //getFromTracks;
    int track_id;
    float x_picture;
    float y_picture;

    void setProjectedPoint(float x, float y){
        x_projected=x;
        y_projected=y;
        distance=sqrt((x*x)+(y*y));
    }
    
    
};

void calculateProjection(cv::Mat_<cv::Point2f> pointMatrix, std::vector<projectedPoint> &pPoints );
    
}

#endif