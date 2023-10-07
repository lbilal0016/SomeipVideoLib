#ifndef SERVICELIB
#define SERVICELIB
//  INCLUDES
#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <condition_variable>
#include <thread>

/*  DEFINES */
#define SAMPLE_SERVICE_ID 0x1777
#define SAMPLE_INSTANCE_ID 0x5677
#define SAMPLE_METHOD_ID 0x0421

void on_message(const std::shared_ptr<vsomeip::message>& Request);
void set_application(std::shared_ptr<vsomeip::application> app);

#endif
