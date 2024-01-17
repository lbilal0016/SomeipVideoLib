#pragma once
#include "opencv4/opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string.h>
#include <nlohmann/json.hpp>

#include "VideoData.pb.h"
#include "ObjectData.pb.h"

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
    std::string type;
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
Detection_Object(object_type_t object, std::string host_info); //  Constructor

Detection_Object(std::vector<uint8_t> serialized_object, std::string host_info); //  Constructor overload function

void print_object() const;  //  Print method for received detected objects

std::vector<uint8_t> GetSerializedObjectData(); //  A method for serializing object_type_t objects into std::vector<uint8>

object_type_t GetDeserializedObjectData();  //  A method for deserializing std::vector<uint8> containers into object_type_t objects

~Detection_Object();    //  Deconstructor

private:
object_type_t m_object;
std::vector<uint8_t> m_serialized_object;
std::string m_host_info;
};


class Video_Object
{
    public:
    Video_Object(); //  Constructor for initiating an empty Video_Object
    Video_Object(std::string config_io, std::string host_info);    //  constructor

    //  Function added for compatibility : it allows redefining video io directory
    void RedefineDirectory(std::string config_io);

    //  Reads the video capture object in the first argument into the second argument struct
    void VideoRead();
    
    //  Writes the second argument of video data struct into the first argument of videowriter object
    void VideoWrite();

    //  This function serialises VideoData struct to an std::vector<uint8_t> and returns that object
    std::vector<uint8_t> SerialiseVideoData();

    //  This function deserialises an std::vector<uint8_t> which carries a VideoData struct, and returns this VideoData struct
    void DeserialiseVideoData(const std::vector<uint8_t> &raw_video_vector);

    Video_Object &operator=(const Video_Object &other_video_object);
    ~Video_Object();    //  deconstructor

    private:

    std::string m_video_config_io;
    //  std::string m_video_reconfig_io;    //  TODO : This member will be used to overload IO functions with a post-defined configuration file path
    std::string m_host_info;
    VideoData m_video_data;

    //  Configure the json file for reading the directory information
    std::string ConfigureInputOutput(const std::string &typeInputOutput);

    //  Executes print operations with a client tag
    void info_printer(const std::stringstream &print_message);
    //  Function overload for previous function with std::string type
    void info_printer(const std::string &print_message);
};

//  Namespace VideoReadWrite end
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