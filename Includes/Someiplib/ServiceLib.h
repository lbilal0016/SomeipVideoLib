#ifndef SERVICELIB
#define SERVICELIB
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

/*  IDS FOR OBJECT DETECTION EVENT COMMUNICATION    */
#define EVENT_ID 0x8778
#define EVENT_SERVICE_ID 0x1000
#define EVENT_INSTANCE_ID 0x3000
#define EVENT_GROUP_ID 0x4465

using Detection_Object = VideoReadWrite::Detection_Object;

void on_message(const std::shared_ptr<vsomeip::message>& Request);

void set_application(std::shared_ptr<vsomeip::application> app);

void on_availability_event(vsomeip::service_t Service, vsomeip::instance_t Instance, bool is_available);
void on_message_event(const std::shared_ptr<vsomeip::message>& event_message);

void run_events();

#endif
