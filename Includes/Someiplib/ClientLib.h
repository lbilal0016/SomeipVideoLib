#ifndef CLIENTLIB
#define CLIENTLIB
//  INCLUDES
#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <condition_variable>
#include <thread>

#include "VideoReadWrite.h"


/*  DEFINES */
#define SAMPLE_SERVICE_ID 0x1777
#define SAMPLE_INSTANCE_ID 0x5677
#define SAMPLE_METHOD_ID 0x0421

#define EVENT_ID 0x8778
#define EVENT_SERVICE_ID 0x1000
#define EVENT_INSTANCE_ID 0x3000
#define EVENT_GROUP_ID 0x4465
#define EVENT_METHOD_ID 0x0425

//  Comment out the following line if UDP communication is wished
#define TCP_COMMUNICATION

static std::string CONFIGURATION_VIDEO_DETECTION;

using  Detection_Object = VideoReadWrite::Detection_Object;
using Video_Object = VideoReadWrite::Video_Object;

//  Executes print operations with a client tag
void client_printer(const std::stringstream &print_message);
//  Function overload for previous function with std::string type
void client_printer(const std::string &print_message);

void run(); //  vsomeip client thread
void on_message(const std::shared_ptr<vsomeip::message> &response);

void on_availability(vsomeip::service_t Service, vsomeip::instance_t Instance, bool is_available);

void set_application(std::shared_ptr<vsomeip::application> app);
void set_video_object(Video_Object &video_obj);

void run_detection();   //  function for object detection thread
void send_data(object_type_t &object_data);

namespace SomeIpLib{
//  This function reads the configuration file to be used from the main function
void ReadConfigFile(const std::string &ConfigFile);
}

//  This function is called, when client is ready to offer object detection events.
void offer_client_event();

//  This function is for register_message_handler for event management
void on_message_event(const std::shared_ptr<vsomeip::message> &response);

#endif