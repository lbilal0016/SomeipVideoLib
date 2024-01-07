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

    //  Test of serialisation of VideoData object
    std::vector<uint8_t> serialized_video = SerialiseVideoData(SampleVideoData);
/*     std::cout << "Test of serialisation.\n";
    std::cout << "Serialised Data : " << serialized_video << std::endl; */

    //  Test of deserialisation of serialized_video string
    VideoData receivedVideo = DeserialiseVideoData(serialized_video);

        //  Verification of whether the deserialisation is successful
        std::cout << "Test for deserialisation.\n";
        if(receivedVideo.VideoFrames.size() == SampleVideoData.VideoFrames.size()){std::cout << "VideoFrames size is deserialised.\n";}
        if(receivedVideo.SingleFrame.size() == SampleVideoData.SingleFrame.size()) {std::cout << "Single frame size is deserialised.\n";}
        if(receivedVideo.frameSize == SampleVideoData.frameSize){std::cout << "Frame size is deserialised.\n";}
        if(receivedVideo.FPS_Rate == SampleVideoData.FPS_Rate){std::cout << "FPS rate is deserialised.\n";}
        if(receivedVideo.frameSizeCaptured == SampleVideoData.frameSizeCaptured){std::cout << "Frame size capture flag is deserialised.\n";}

        if(receivedVideo.VideoFrames.size() != SampleVideoData.VideoFrames.size() ||
        receivedVideo.SingleFrame.size() != SampleVideoData.SingleFrame.size() ||
        receivedVideo.frameSize != SampleVideoData.frameSize ||
        receivedVideo.FPS_Rate != SampleVideoData.FPS_Rate ||
        receivedVideo.frameSizeCaptured != SampleVideoData.frameSizeCaptured){
            std::cout << "Something went wrong during deserialisation.\n";
        }
    VideoWrite(receivedVideo);

return 0;
}
