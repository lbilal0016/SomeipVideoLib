#pragma once
#include "opencv4/opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string.h>
#include <nlohmann/json.hpp>

#include "VideoData.pb.h"

using VideoCapture = cv::VideoCapture;
using VideoWriter = cv::VideoWriter;
using json = nlohmann::json;

static std::string CONFIGURATION_VIDEO_IO;

struct VideoData
{
    std::vector<cv::Mat> VideoFrames;
    cv::Mat SingleFrame;
    cv::Size frameSize;
    double FPS_Rate;
    bool frameSizeCaptured;
};

typedef struct object_type{
    char type;
    int count;
    float time;
}object_type_t;



//  Reads the video capture object in the first argument into the second argument struct
void VideoRead(VideoData &InputVideoData);

//  Writes the second argument of video data struct into the first argument of videowriter object
void VideoWrite(VideoData &InputVideoData);

//  Configures Input/Output files via json configuration file and returns the directory for input/output files
std::string ConfigureInputOutput(const std::string &typeInputOutput, std::string &ConfigFile);

namespace VideoReadWrite{
//  This function reads the configuration file to be used from the main function

void ReadConfigFile(const std::string &ConfigFile);

//  This class performs operations on the detected objects which are sent from the client side
class Detection_Object
{
public:
Detection_Object(object_type_t object); //  Constructor

void print_object() const;  //  Print method for received detected objects

~Detection_Object();    //  Deconstructor

private:
object_type_t m_object;
};

}

//  This function serialises VideoData struct to an std::vector<uint8_t> and returns that object
std::vector<uint8_t> SerialiseVideoData(const VideoData &videodata);

//  This function deserialises an std::vector<uint8_t> which carries a VideoData struct, and returns this VideoData struct
VideoData DeserialiseVideoData(const std::vector<uint8_t> &raw_video_vector);

//  This function is included to test the json string which is used for serialisation and deserialisation of video data
bool isJsonValid(const std::string& jsonString);

//  This function is used to check whether a specific video file can b
void CheckVideoFile(std::string &VideoPath);

//  This function reads the quasi-detected objects from a json configuration file and writes them into a object_type_t variable
void DetectObjectsFromJson(const std::string& JsonPathDetection, std::vector<object_type_t>& objects);