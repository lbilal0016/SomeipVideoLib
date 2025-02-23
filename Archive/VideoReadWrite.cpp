#include "opencv4/opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string.h>

#include "VideoReadWrite.h"
#include <nlohmann/json.hpp>

void VideoRead(VideoData &InputVideoData)
{
    VideoCapture VideoCap(ConfigureInputOutput("Input", CONFIGURATION_VIDEO_IO));

    //  Checking against opening the file
    if(!VideoCap.isOpened()){
        std::cerr << "Error: Video file could not be opened.\n";
        throw std::runtime_error("Error: Video file read is not successful.\n");
    }   else{
        //  Log message
        std::cout << "Video file opened successfully and is currently being read ...\n";}

    //  Reading the FPS Rate into the FPS_Rate attribute
    InputVideoData.FPS_Rate = VideoCap.get(cv::CAP_PROP_FPS);

    InputVideoData.frameSizeCaptured = false;   //  This flag will be set to 1 once the first frame size is captured
    while(VideoCap.read(InputVideoData.SingleFrame)){
        if(!InputVideoData.SingleFrame.empty()){ // Add the frame to Frames vector only if it is not empty
        InputVideoData.VideoFrames.push_back(InputVideoData.SingleFrame.clone());
        }
        //  Saving the frame size for once
        if(!InputVideoData.frameSizeCaptured){
            InputVideoData.frameSize = InputVideoData.SingleFrame.size();
            InputVideoData.frameSizeCaptured = true;    //  First video frame size is captured
        }
    }
    //  Log message 
    std::cout << "Input video file is being successfully read.\n";
}

void VideoWrite(VideoData &InputVideoData)
{
    //  1.  Creating the cv::VideoWriter object
    VideoWriter VideoWriter(ConfigureInputOutput("Output", CONFIGURATION_VIDEO_IO), VideoWriter::fourcc('X','2','6','4'),InputVideoData.FPS_Rate, InputVideoData.frameSize);

    //  Checking against the createability of output file
    if(!VideoWriter.isOpened()){
        std::cerr << "Error: Creating an output video file is not successful.\n";
        throw std::runtime_error("Error: Video file write is not successful.\n");
    }//else{std::cout << "Creating the output video file is successful.\n";}

    //  Writing the output file
    std::cout << "Writing the output ...\n";
    for(const cv::Mat &frame : InputVideoData.VideoFrames){
        VideoWriter.write(frame);
    }

    //  Log message
    std::cout << "Output file is written in the following directory: ";
    std::cout << ConfigureInputOutput("Output", CONFIGURATION_VIDEO_IO) << std::endl;
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

//  This function reads the configuration file in which input and output directories for the videos are set
void ReadConfigFile(const std::string &ConfigFile)
{
    CONFIGURATION_VIDEO_IO = ConfigFile;
    std::cout << "Configuration for video input/output has been successfully defined.\n";
}

/*      Protobuf overloading for serialisation and deserialisation methods    */
#ifdef USE_PROTOBUF
std::vector<uint8_t> SerialiseVideoData(const VideoData &videodata)
{
    //  Filling videodata struct into the Protovideo object
    VideoDataProto ProtovideoSent;

    //  Filling video frames field in proto object
    for(const auto &frame : videodata.VideoFrames)
    {
        std::vector<uchar> buf;
        if(!frame.empty()){ //  Checking against empty frames
        cv::imencode(".jpg", frame, buf);   //  Encoding current frame to JPEG format
        ProtovideoSent.add_video_frames(reinterpret_cast<const char *>(buf.data()), buf.size());    //  Add encoded frame to proto's repeated bytes field
        }
    }

    //  Encoding the single frame attribute to JPEG format and filling it into ProtovideoSent object
/*     std::vector<uchar> SingleFrameBuf;
    cv::imencode(".jpg", videodata.SingleFrame, SingleFrameBuf);
    ProtovideoSent.set_single_frame(reinterpret_cast<const char *>(SingleFrameBuf.data()), SingleFrameBuf.size()); */

    //  Filling simple fields in ProtovideoSent object
    ProtovideoSent.set_width(videodata.frameSize.width);    //  setting width
    ProtovideoSent.set_height(videodata.frameSize.height);  //  setting height
    ProtovideoSent.set_fps_rate(videodata.FPS_Rate);    //  setting fps rate
    ProtovideoSent.set_frame_size_captured(videodata.frameSizeCaptured);    //  settig the flag for frame size captured

    //  Creating the return object and 
    std::string serialized_video_string;
    ProtovideoSent.SerializeToString(&serialized_video_string);
    std::vector<uint8_t> serialized_video_vector(serialized_video_string.begin(),serialized_video_string.end());

    //  Log message
    std::cout << "Video file is serialised and ready to be sent.\n";

    return serialized_video_vector;
}


VideoData DeserialiseVideoData(const std::vector<uint8_t> &raw_video_vector)
{
    VideoData receivedVideo;
    VideoDataProto ProtoVideoRec;  

    //  Log message
    std::cout << "Converting raw video data into readable video ... \n";

    //  Reading (parsing) the input string into ProtoVideoRec object
    std::string protoString(raw_video_vector.begin(),raw_video_vector.end());
    ProtoVideoRec.ParseFromString(protoString);

    //  Fill the receivedVideo struct here:

    //  Filling the VideoFrames vector
    for(const auto &frames : ProtoVideoRec.video_frames())
    {
        std::vector<uchar> buf(frames.begin(), frames.end());
        cv::Mat frame = cv::imdecode(buf, cv::IMREAD_COLOR);
        receivedVideo.VideoFrames.push_back(frame);
    }

    //  Decode and fill SingleFrame attribute
/*     const std::string &singleFrameRec = ProtoVideoRec.single_frame();
    std::vector<uchar> single_frame_buf(singleFrameRec.begin(), singleFrameRec.end());
    receivedVideo.SingleFrame = cv::imdecode(single_frame_buf, cv::IMREAD_COLOR); */

    //  Filling simple attributes
    receivedVideo.frameSize = cv::Size(ProtoVideoRec.width(), ProtoVideoRec.height());
    receivedVideo.FPS_Rate = ProtoVideoRec.fps_rate();
    receivedVideo.frameSizeCaptured = ProtoVideoRec.frame_size_captured();

    //  Log message
    std::cout << "Received raw video is now converted into an opencv object, and is ready to be written.\n";

    //  Returning receivedVideo struct
    return receivedVideo;
}
#endif


/*      JSON overloading for serialisation and deserialisation methods    */
#ifdef USE_JSON_MET
std::vector<uint8_t> SerialiseVideoData(const VideoData &videodata)
{
    //  json string object for serialisation
    json jsonData;

    //  Simple attributes of videoData struct
    jsonData["frameSize"] = {videodata.frameSize.width, videodata.frameSize.height};
    jsonData["FPS_Rate"] = videodata.FPS_Rate;
    jsonData["frameSizeCaptured"] = videodata.frameSizeCaptured;

    /*  Serialisation of frames to base64 strings   */
    std::vector<std::string> base64Frames;  //  Container to store encoded video frames
    for(const auto &frame : videodata.VideoFrames)
    {
        std::vector<uchar> buf; //  buffer uchar vector for storing frames temporarily in .jpg format
        if(!frame.empty()){
        cv::imencode(".jpg", frame, buf);   //  opencv function for converting and storing the current frame in buf variable
        std::string frame_base64encode(reinterpret_cast<const char *>(buf.data()), buf.size()); //   convert uchar vector into std::string object
        base64Frames.push_back(frame_base64encode); //  Adding the current frame into video frames container
        }
    }
    jsonData["VideoFrames"] = base64Frames; //  Creating a VideoFrames attribute in json variable which contains a string with encoding of video frames

    //  std::vector<uint8_t> object for serialisation
    std::vector<uint8_t> serialised_video_data(jsonData.dump().begin(), jsonData.dump().end());  //  Dump the JSON data to a string and fill the uint8 vector with this string

    return serialised_video_data;

}

VideoData DeserialiseVideoData(const std::vector<uint8_t> &jsonStringBytes)
{
    VideoData receivedVideo;
    std::string jsonString(jsonStringBytes.begin(), jsonStringBytes.end());
    json jsonData = json::parse(jsonString);    //  Parse the input JSON string into a json object

    //  Deserialisation of simple attributes
    receivedVideo.frameSize.width = jsonData["frameSize"][0];
    receivedVideo.frameSize.height = jsonData["frameSize"][1];
    receivedVideo.FPS_Rate = jsonData["FPS_Rate"];
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
#endif