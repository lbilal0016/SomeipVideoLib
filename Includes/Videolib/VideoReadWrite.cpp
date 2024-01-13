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
    //  Configure Detection Json file  
    else if(typeInputOutput.compare("Detection") == 0){
        json detectionConfig;
        std::ifstream detectionConfigFile(ConfigFile);
        if(!detectionConfigFile.is_open())
        {
            std::cerr << "Error: Failed to open output configuration file. Please make sure that json file is placed within ConfigFiles folder.\n";
            throw std::runtime_error("Error: Configuration file could not be opened.\n");
        }
        detectionConfigFile >> detectionConfig;
        detectionConfigFile.close();

        Result = detectionConfig["Detection"];
    }
    //  False argument to the function
    else{
        std::cerr << "Error: Input argument to the function ConfigureInputOutput is not correct. Correct usage is Input, Output, or Detection.\n";
        throw std::runtime_error("Error: Wrong argument to functon ConfigureInputOutput.\n");
    }

    return Result;
}

//  This function reads the configuration file in which input and output directories for the videos are set
void VideoReadWrite::ReadConfigFile(const std::string &ConfigFile)
{
    CONFIGURATION_VIDEO_IO = ConfigFile;
    std::cout << "Configuration for video input/output has been successfully defined.\n";
}


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

    //  Filling simple attributes
    receivedVideo.frameSize = cv::Size(ProtoVideoRec.width(), ProtoVideoRec.height());
    receivedVideo.FPS_Rate = ProtoVideoRec.fps_rate();
    receivedVideo.frameSizeCaptured = ProtoVideoRec.frame_size_captured();

    //  Log message
    std::cout << "Received raw video is now converted into an opencv object, and is ready to be written.\n";

    //  Returning receivedVideo struct
    return receivedVideo;
}

void CheckVideoFile(std::string &VideoPath)
{
    VideoCapture VideoCap(VideoPath);

    //  Checking against opening the file
    std::cout << "CLIENT/SERVER : Video file is currently being checked against openability.\n";
    if(!VideoCap.isOpened()){
        std::cerr << "Error: Video file could not be opened.\n";
        throw std::runtime_error("Error: Video file read is not successful.\n");
    }
}

void DetectObjectsFromJson(const std::string& JsonPathDetection, std::vector<object_type_t>& objects)
{
    //  Convert json file into a filestream object
    std::ifstream file(JsonPathDetection);

    //  Check whether the json file can be opened
    if(!file.is_open()){
        std::cerr << "Error opening json file for object detection:" << JsonPathDetection 
        << "Please ensure that a proper json file is available in the right directory.\n";
    return;
    }

    try
    {
        json DetectionJson;
        file >> DetectionJson;

        //  Scan the json file from the beginning until the end
        if(DetectionJson.find("objects") != DetectionJson.end()){

            object_type_t obj;
            
            for (const auto& entry : DetectionJson["objects"])
            {
               //  GETTING OBJECT TYPE
               obj.type = entry["name"].get<std::string>();
                
                //  Get number of identified object
                obj.count = entry["number"].get<int>();

                //  Get the time instance in which detection of identified object took place
                obj.time = entry["time"].get<float>();

                //  Load identified object into the objects vector
                objects.push_back(obj);
            }
        }
        else{
            std::cerr << "Error: 'objects' field not found in JSON file.\n";
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error parsing Json: " <<e.what() << '\n';
    }

//  Close configuration file
file.close();
}

//  Constructor of object detection class by using directly object_type_t 
VideoReadWrite::Detection_Object::Detection_Object(object_type_t object): m_object(object)
{}

//  Constructor overlaod of object detection class by using serialized object type
VideoReadWrite::Detection_Object::Detection_Object(std::vector<uint8_t> serialized_object) : m_serialized_object(serialized_object)
{
    /*  THIS CONSTRUCTOR OVERLOAD IS RESPONSIBLE FOR INTERNAL CONVERSION OF STD::VECTOR<UINT8_T> INTO OBJECT_TYPE_T */

    ObjectDataProto detection_object_proto;

    /*  CONVERSION OF UINT8 VECTOR OBJECT INTO A STRING */
    std::string ProtoObjString(m_serialized_object.begin(), m_serialized_object.end());
    
    /*  PARSING STRING OBJECT INTO PROTO OBJECT */
    detection_object_proto.ParseFromString(ProtoObjString);

    m_object.type = detection_object_proto.object();    //  Extracting object type from proto field (1)
    m_object.count = detection_object_proto.count();    //  Extracting count of objects from proto field (2)
    m_object.time = detection_object_proto.time();    //  Extracting observation time of object from proto field (3)
}

VideoReadWrite::Detection_Object::~Detection_Object()
{}

//  This method is used to print detected objects
void VideoReadWrite::Detection_Object::print_object() const
{   

    /*  CONSOLE OUTPUT FOR DETECTED OBJECT */

    if(m_object.count != 1 && m_object.count > 0)
    {   //  Detected object has multiple instances
        std::cout << m_object.count << " objects of type : " << m_object.type <<
        " are detected at time " << m_object.time << "s.\n";
    }
    else if(m_object.count == 0)
    {   //  There is a problem with the detection, so that an object has been detected with count = 0
        std::cout << "Error at detection, an object with no number was identified!\n";
    }
    else
    {   //  A single object was detected
        std::cout << "An object of type : " << m_object.type <<
        " is detected at time " << m_object.time << "s.\n";
    }
}

std::vector<uint8_t> VideoReadWrite::Detection_Object::GetSerializedObjectData()
{
    ObjectDataProto serialized_object_proto;
    std::string serialized_object_string;

    /*  SET SERIALIZATION STRING FIELDS USING PROTO LIBRARY */
    serialized_object_proto.set_object(m_object.type);  //  Serialize object type field (1)
    serialized_object_proto.set_count(m_object.count);  //  Serialize object count field (2)
    serialized_object_proto.set_time(m_object.time);  //  Serialize object time field (3)

    /*  PREPARE SERIALIZED STRING   */
    serialized_object_proto.SerializeToString(&serialized_object_string);

    /*  PREPARE SERIALIZED VECTOR OBJECT    */
    std::vector<uint8_t> serialized_object(serialized_object_string.begin(), serialized_object_string.end());

    return serialized_object;
} 

object_type_t VideoReadWrite::Detection_Object::GetDeserializedObjectData()
{
    /*  DESERIALIZATION ALREADY SUCCEEDED IN THE CONSTRUCTOR FUNCTION OVERLOAD, THIS FUNCTION ONLY RETURN MEMBERS   */
    return m_object;
}  



