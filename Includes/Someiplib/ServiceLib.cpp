#include "ServiceLib.h"

#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <condition_variable>
#include <thread>

#include "VideoReadWrite.h"

//  This function passes the someip application object from the Client executable to this object code
static std::shared_ptr<vsomeip::application> this_app;
void set_application(std::shared_ptr<vsomeip::application> app)
{
    this_app = app;
}

void on_message(const std::shared_ptr<vsomeip::message>& Request)
{
    std::cout << "on_message():\n";
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

    std::cout << "SERVICE: Received request with Client/Session [ "
    << std::setw(4) << std::setfill('0') << std::hex << Request->get_client() << "/"
    << std::setw(4) << std::setfill('0') << std::hex << Request->get_session() << "]"
    << ss.str() << std::endl;

    /*      READ THE INPUT FROM CONFIG AND PUT IT ON PAYLOAD    */
    //  Reading the input video file
    //  log message
    std::cout << "Server: Input file is being read ... \n";
    VideoData Payload_Video;    //  Create the container for video data 
    VideoRead(Payload_Video);   //  Read the input into this container for video data

    //  Serialise video data and load it into an std::vector<uint8_t> object
    std::vector<uint8_t> raw_video = SerialiseVideoData(Payload_Video);

    //  Create Response:
    std::shared_ptr<vsomeip::message> its_response = vsomeip::runtime::get()->create_response(Request);
    //      Refilling the its_payload object
    its_payload = vsomeip::runtime::get()->create_payload();

    its_payload->set_data(reinterpret_cast<const vsomeip::byte_t*>(raw_video.data()), raw_video.size());
    its_response->set_payload(its_payload);
    this_app->send(its_response);
}