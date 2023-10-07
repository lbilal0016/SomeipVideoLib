#include "ClientLib.h"
#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <condition_variable>
#include <thread>


//std::shared_ptr< vsomeip::application > app;
std::mutex mutex;
std::condition_variable condition;


//  This function passes the someip application object from the Client executable to this object code
static std::shared_ptr<vsomeip::application> this_app;
void set_application(std::shared_ptr<vsomeip::application> app)
{
    this_app = app;
}

//  This function will be run on the client side after the service is offered
void run()
{
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(lock);

    //  Creating the object for vsome request message
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    //  Setting service id, instance id, method id
    request->set_service(SAMPLE_SERVICE_ID);
    request->set_instance(SAMPLE_INSTANCE_ID);
    request->set_method(SAMPLE_METHOD_ID);

    //  Create and add payload to someip message object
    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    //      Creating the vector to carry bytes
    std::vector<vsomeip::byte_t> payload_data;

    //      Filling the vector with random data (for training purposes)
    for(vsomeip::byte_t i = 0; i<10; ++i)
    {
        payload_data.push_back(i % 256);
    }

    //      Loading the data vector to someip payload
    its_payload->set_data(payload_data);
    
    //      Adding the payload to someip message
    request->set_payload(its_payload);
    this_app->send(request);
}

void on_message(const std::shared_ptr<vsomeip::message> &response)
{
std::shared_ptr<vsomeip::payload> its_payload = response->get_payload();
vsomeip::length_t len = its_payload->get_length();

//  Get payload
std::stringstream ss;
for(vsomeip::length_t i = 0; i<len; ++i)
{
    ss << std::setw(4)<<std::setfill('0') << std::hex
    <<(int) *(its_payload->get_data() + i) << " ";
}

//  Print the received data
std::cout << "CLIENT: Received message with Client/Session ["
<<  std::setw(4) << std::setfill('0') << std::hex
<<  response->get_client() << "/"
<<  std::setw(4) << std::setfill('0') << std::hex
<<  response->get_session() << "]" << ss.str() << std::endl;
}

void on_availability(vsomeip::service_t Service, vsomeip::instance_t Instance, bool is_available)
{
//  Checking the availability of the service offered by the server
//	    std::setw(4) -> set the width of hexadecimal number to 4
//	    std::setfill(0) -> if the service id has less than 4 digits, fill the leading digits with 0s.
std::cout << "CLIENT: Service (Service.Instance) [" << std::setw(4) << std::setfill('0') << std::hex 
<< Service << "." << Instance << "] is " << (is_available ? "available" : "NOT available") << std::endl;

//  Sending wake-up call for the waiting thread on the client side after the service is available
condition.notify_one();
}