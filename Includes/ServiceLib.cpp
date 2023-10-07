#include "ServiceLib.h"

#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <condition_variable>
#include <thread>

//  This function passes the someip application object from the Client executable to this object code
static std::shared_ptr<vsomeip::application> this_app;
void set_application(std::shared_ptr<vsomeip::application> app)
{
    this_app = app;
}

void on_message(const std::shared_ptr<vsomeip::message>& Request)
{
    //  Reading the payload
    std::shared_ptr<vsomeip::payload> its_payload = Request->get_payload();
    //  Getting the payload length
    vsomeip::length_t len = its_payload->get_length();

    //  Getting the payload
    std::stringstream ss;
    //  This initiates a loop that iterates over each byte of the payload.
    for(vsomeip::length_t i=0; i<len; ++i)
    {
        ss << std::setw(2) << std::setfill('0') << std::hex
        //  its_payload->get_data() returns a pointer to the start of the payload data
        //  << " ";: A space is added after each formatted hexadecimal value to separate them in the final string.
        <<(int)*(its_payload->get_data()+i) << " "; 
    }

    std::cout << "SERVICE: Received message with Client/Session [ "
    << std::setw(4) << std::setfill('0') << std::hex << Request->get_client() << "/"
    << std::setw(4) << std::setfill('0') << std::hex << Request->get_session() << "]"
    << ss.str() << std::endl;

    //  Create Response:
    //      Creating a vsomeip::message object
    //          vsomeip::runtime::get() returns an instance of a vsomeip::runtime class for the first call
    //          in all the following calls, it returns the same pointer to the runnint vsomeip::runtime instance
    //          ->create_response() is a method within vsomeip::runtime class. 
    std::shared_ptr<vsomeip::message> its_response = vsomeip::runtime::get()->create_response(Request);
    //      Refilling the its_payload object
    its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> payload_data;
    for (int i=9; i>=0; --i)
    {
        payload_data.push_back(i%256);
    }
    its_payload->set_data(payload_data);
    its_response->set_payload(its_payload);
    this_app->send(its_response);
}