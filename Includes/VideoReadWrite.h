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

//  Reads the video capture object in the first argument into the second argument struct
void VideoRead(VideoData &InputVideoData);

//  Writes the second argument of video data struct into the first argument of videowriter object
void VideoWrite(VideoData &InputVideoData);

//  Configures Input/Output files via json configuration file and returns the directory for input/output files
std::string ConfigureInputOutput(const std::string &typeInputOutput, std::string &ConfigFile);

//  This function reads the configuration file to be used from the main function
void ReadConfigFile(const std::string &ConfigFile);

//  This function serialises VideoData struct to JSON String and returns that sring
std::vector<uint8_t> SerialiseVideoData(const VideoData &videodata);

//  This function deserialises a JSON string which carries a VideoData struct, and returns this VideoData struct
VideoData DeserialiseVideoData(const std::vector<uint8_t> &raw_video_vector);

//  This function is included to test the json string which is used for serialisation and deserialisation of video data
bool isJsonValid(const std::string& jsonString);
