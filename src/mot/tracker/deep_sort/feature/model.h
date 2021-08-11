#ifndef MODEL_H
#define MODEL_H
#include "./dataType.h"
#include <map>


class DETECTION_ROW {
public:
    DETECTBOX tlwh; //np.float
    float confidence; //float
    FEATURE feature; //np.float32
    int class_id;
    DETECTBOX to_xyah() const;
    DETECTBOX to_tlbr() const;
};

typedef std::vector<DETECTION_ROW> DETECTIONS;
#endif // MODEL_H