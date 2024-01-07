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

using Detection_Object = VideoReadWrite::Detection_Object;
static std::string CONFIGURATION_VIDEO_DETECTION;

void run_detection()
{
    //  lock for the object detection is removed
    std::cout << "CLIENT : Object detection is running ...\n";


    /*  FURTHER STEPS: 

    1.  Engineer a function for checking whether the received video can be opened */
    std::string Path_to_video = ConfigureInputOutput("Output", CONFIGURATION_VIDEO_DETECTION);
    CheckVideoFile(Path_to_video);

    /*
    2. Obtain the json configuration file for object detection*/ 
    std::string Path_to_detection_Json = ConfigureInputOutput("Detection", CONFIGURATION_VIDEO_DETECTION);
    std::cout << "Path_to_detection_Json was configured as : " << Path_to_detection_Json << std::endl;
    /*
    
    3.  Using the configuration file, carry out detection process*/
    std::vector<object_type_t> detected_objects;

    DetectObjectsFromJson(Path_to_detection_Json, detected_objects);

    for(const auto &objects : detected_objects)
    {
        Detection_Object captured_object(objects);
        captured_object.print_object();
        captured_object.~Detection_Object();
    }

}

int main(int argc, char *argv[])
{
    // Check if the correct number of command line arguments is provided
    if (argc != 2) {
        std::cerr << "Usage: " << argv[1] << " <Path to ConfigFile>\n";
        return 1;
    }
    std::string ConfigFile(argv[1]);
    CONFIGURATION_VIDEO_DETECTION = ConfigFile;
/* 
    VideoData SampleVideoData;
    VideoRead(SampleVideoData); */


    run_detection();
   
return 0;
}
