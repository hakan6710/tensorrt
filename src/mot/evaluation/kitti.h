#ifndef KITTI_H
#define KITTI_H
#include <string>
#include <iostream> 
#include <map>
using namespace std;

namespace KITTI_EVA{

map<int, string> getFilePathes(string directory);
map<string, string> getFolderPathes(string directory);
}



#endif