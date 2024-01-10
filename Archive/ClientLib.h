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

//  Comment out the following line if UDP communication is wished
#define TCP_COMMUNICATION

static std::string CONFIGURATION_VIDEO_DETECTION;

using  Detection_Object = VideoReadWrite::Detection_Object;

void run(); //  vsomeip client thread
void on_message(const std::shared_ptr<vsomeip::message> &response);
void on_availability(vsomeip::service_t Service, vsomeip::instance_t Instance, bool is_available);
void set_application(std::shared_ptr<vsomeip::application> app);

void run_detection();   //  function for object detection thread
void send_data(object_type_t &object_data);

namespace SomeIpLib{
//  This function reads the configuration file to be used from the main function
void ReadConfigFile(const std::string &ConfigFile);
}

//  This function is called, when client is ready to offer object detection events.
void offer_client_event();

#endif