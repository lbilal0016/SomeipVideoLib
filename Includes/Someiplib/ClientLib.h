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

//  Comment out the following line if UDP communication is wished
#define TCP_COMMUNICATION

void run();
void on_message(const std::shared_ptr<vsomeip::message> &response);
void on_availability(vsomeip::service_t Service, vsomeip::instance_t Instance, bool is_available);
void set_application(std::shared_ptr<vsomeip::application> app);


#endif