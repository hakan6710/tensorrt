#ifndef objectTracker_HPP_
#define objectTracker_HPP_
#include "../src/mot/mot.hpp"


namespace FHAC{

class objectTracker{

public:

void init();

void test(string root_dir);

private:
Config currentConfig;


};


}



#endif