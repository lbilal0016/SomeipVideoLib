#include <iomanip>
#include <iostream>
#include <sstream>

#include <fstream>
#include <vector>
#include <stdexcept>
#include <string.h>


#include <vsomeip/vsomeip.hpp>

#include "opencv4/opencv2/opencv.hpp"

#include "ServiceLib.h"

#include "VideoReadWrite.h"


int main(int argc, char *argv[])
{
    // Check if the correct number of command line arguments is provided
    if (argc != 2) {
        std::cerr << "Usage: " << argv[1] << " <Path to ConfigFile>\n";
        return 1;
    }
    std::string ConfigFile(argv[1]);
    ReadConfigFile(ConfigFile);

    VideoData SampleVideoData;
    VideoRead(SampleVideoData);

    std::string json_parsing_test = SerialiseVideoData(SampleVideoData);
    std::cout << "JSON String: " << json_parsing_test << std::endl;


    /*
    bool is_parsed = isJsonValid(json_parsing_test);

    if(is_parsed){
        std::cout << "Json string parsed.\n";}
        else{
            std::cout << "Json string did not parse.\n";
        }
        */

return 0;
}
