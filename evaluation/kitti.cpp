#include "kitti.h"
#include <string>
#include <iostream> 
#include <map>

using namespace std;



namespace KITTI_EVA{


map<int, string> getFilePathes(string directory)
{
    map<int, string> map1;
    int number_of_zeros=6;
 
	string original_string=to_string(42);
	std::string dest = std::string( number_of_zeros, '0').append( original_string);

	for(int i=0; i<1000; i++){
        
		string original_string=to_string(i);
		std::string dest = std::string( number_of_zeros-original_string.size(), '0').append( original_string);


        string filepath=directory+dest+".png";

        if (FILE *file = fopen(filepath.c_str(), "r")) 
        {
            fclose(file);
            map1.insert(std::pair<int, string>(i,filepath));
            
        }else {break;}
        
        
	}

    return map1;
}

map<int, string> getFolderPathes(string directory)
{
    map<int, string> map1;
    int number_of_zeros=4;
	string original_string=to_string(42);
	std::string dest = std::string( number_of_zeros, '0').append( original_string);

	for(int i=0; i<1000; i++){
        
		string original_string=to_string(i);
		std::string dest = std::string( number_of_zeros-original_string.size(), '0').append( original_string);

        string filepath=directory+dest;
        if (FILE *file = fopen(filepath.c_str(), "r")) 
        {
            fclose(file);
            map1.insert(std::pair<int, string>(i,filepath+"/"));
            
        }else {break;}
        
        
	}

    return map1;
}

}