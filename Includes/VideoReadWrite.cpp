#include "opencv4/opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string.h>

#include "VideoReadWrite.h"
#include <nlohmann/json.hpp>

void VideoRead(VideoCapture &VideoCap, VideoData &InputVideoData)
{
    //  Checking against opening the file
    if(!VideoCap.isOpened()){
        std::cerr << "Error: Video file could not be opened.\n";
        throw std::runtime_error("Error: Video file read is not successful.\n");
    }   //else{std::cout << "Video file opened successfully.\n";}

    InputVideoData.frameSizeCaptured = false;   //  This flag will be set to 1 once the first frame size is captured
    while(VideoCap.read(InputVideoData.SingleFrame)){
        InputVideoData.VideoFrames.push_back(InputVideoData.SingleFrame.clone());

        //  Saving the frame size for once
        if(!InputVideoData.frameSizeCaptured){
            InputVideoData.frameSize = InputVideoData.SingleFrame.size();
            InputVideoData.frameSizeCaptured = true;    //  First video frame size is captured
        }
    }
}

void VideoWrite(VideoWriter &VideoWrite, VideoData &InputVideoData)
{
    //  Checking against the createability of output file
    if(!VideoWrite.isOpened()){
        std::cerr << "Error: Creating an output video file is not successful.\n";
        throw std::runtime_error("Error: Video file write is not successful.\n");
    }//else{std::cout << "Creating the output video file is successful.\n";}

    //  Writing the output file
    std::cout << "Writing the output ...\n";
    for(const cv::Mat &frame : InputVideoData.VideoFrames){
        VideoWrite.write(frame);
    }
    std::cout << "Output file is written.\n";
}

std::string ConfigureInputOutput(const std::string &typeInputOutput, std::string &ConfigFile)
{      
    std::string Result;
    //  Configure input
    if(typeInputOutput.compare("Input") == 0)
    {
        json inputConfig;
        std::ifstream inputConfigFile(ConfigFile);
        if(!inputConfigFile.is_open())
        {
            std::cerr << "Error: Failed to open input configuration file. Please make sure that json file is placed within ConfigFiles folder.\n";
            throw std::runtime_error("Error: Configuration file could not be opened.\n");
        }
        inputConfigFile >> inputConfig;
        inputConfigFile.close();

        Result = inputConfig["InputFile"];
    }
    //  Configure Output   
    else if(typeInputOutput.compare("Output") == 0){
        json outputConfig;
        std::ifstream outputConfigFile(ConfigFile);
        if(!outputConfigFile.is_open())
        {
            std::cerr << "Error: Failed to open output configuration file. Please make sure that json file is placed within ConfigFiles folder.\n";
            throw std::runtime_error("Error: Configuration file could not be opened.\n");
        }
        outputConfigFile >> outputConfig;
        outputConfigFile.close();

        Result = outputConfig["OutputFile"];
    }
    //  False argument to the function
    else{
        std::cerr << "Error: Input argument to the function ConfigureInputOutput is not correct. Correct usage is Input or Output.\n";
        throw std::runtime_error("Error: Wrong argument to functon ConfigureInputOutput.\n");
    }

    return Result;
}

std::string SerialiseVideoData(const VideoData &videodata)
{
    //  json string object for serialisation
    json jsonData;

    //  Simple attributes of videoData struct
    jsonData["frameSize"] = {videodata.frameSize.width, videodata.frameSize.height};
    jsonData["frameSizeCaptured"] = videodata.frameSizeCaptured;

    /*  Serialisation of frames to base64 strings   */
    std::vector<std::string> base64Frames;  //  Container to store encoded video frames
    for(const auto &frame : videodata.VideoFrames)
    {
        std::vector<uchar> buf; //  buffer uchar vector for storing frames temporarily in .jpg format
        cv::imencode(".jpg", frame, buf);   //  opencv function for converting and storing the current frame in buf variable
        std::string frame_base64encode(buf.begin(), buf.end()); //   convert uchar vector into std::string object
        base64Frames.push_back(frame_base64encode); //  Adding the current frame into video frames container
    }
    jsonData["VideoFrames"] = base64Frames; //  Creating a VideoFrames attribute in json variable which contains a string with encoding of video frames

    return jsonData.dump(); //  Dump the JSON data to a string

}

VideoData DeserialiseVideoData(const std::string &jsonString)
{
    VideoData receivedVideo;
    json jsonData = json::parse(jsonString);    //  Parse the input JSON string into a json object

    //  Deserialisation of simple attributes
    receivedVideo.frameSize.width = jsonData["frameSize"][0];
    receivedVideo.frameSize.height = jsonData["frameSize"][1];
    receivedVideo.frameSizeCaptured = jsonData["frameSizeCaptured"];

    /*  Deserialisation of video frames from base64 strings */
    std::vector<std::string> base64Frames = jsonData["VideoFrames"];    //  Container to extract video frames from json stack
    for(const auto &base64Frame : base64Frames)
    {
        std::vector<uchar> buf(base64Frame.begin(), base64Frame.end()); //  buffer uchar vector for storing read frames temporarily before decoding
        cv::Mat decoded_frame = cv::imdecode(buf, cv::IMREAD_COLOR);    //  decode single frames from buffer container with color option from cv library
        receivedVideo.VideoFrames.push_back(decoded_frame); //  insert decoded frames into VideoFrames container 
    }

    return receivedVideo;
}