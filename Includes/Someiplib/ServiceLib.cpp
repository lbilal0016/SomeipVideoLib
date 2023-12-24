#include "ServiceLib.h"

#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <condition_variable>
#include <thread>

#include "VideoReadWrite.h"

std::mutex mutex;
std::condition_variable condition;

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
    std::shared_ptr<vsomeip::payload> response_payload = Request->get_payload();
    //  Getting the payload length
    vsomeip::length_t len = response_payload->get_length();

    //  Getting the payload
    std::stringstream ss;
    //  This initiates a loop that iterates over each byte of the payload.
    for(vsomeip::length_t i=0; i<len; ++i)
    {
        ss << std::setw(2) << std::setfill('0') << std::hex
        //  response_payload->get_data() returns a pointer to the start of the payload data
        //  << " ";: A space is added after each formatted hexadecimal value to separate them in the final string.
        <<(int)*(response_payload->get_data()+i) << " "; 
    }

    /*  ACKNOWLEDGE THE VIDEO REQUEST FROM THE CLIENT SIDE  */
    std::cout << "SERVICE: Received request with Client/Session [ "
    << std::setw(4) << std::setfill('0') << std::hex << Request->get_client() << "/"
    << std::setw(4) << std::setfill('0') << std::hex << Request->get_session() << "]"
    << ss.str() << std::endl;

    /*      READ THE INPUT FROM CONFIG AND PUT IT ON PAYLOAD    */
    //  Reading the input video file
    std::cout << "Server: Input file is being read ... \n"; //  log message
    VideoData Payload_Video;    //  Create the container for video data 
    VideoRead(Payload_Video);   //  Read the input into this container for video data

    /*  SERIALISE VIDEO DATA AND LOAD IT INTO AN STD::VECTOR<UINT8_T> OBJECT   */
    std::vector<uint8_t> raw_video = SerialiseVideoData(Payload_Video);

    /*  CREATE RESPONSE    */  
    std::shared_ptr<vsomeip::message> response = vsomeip::runtime::get()->create_response(Request);
    //      Refilling the response_payload object
    response_payload = vsomeip::runtime::get()->create_payload();

    response_payload->set_data(reinterpret_cast<const vsomeip::byte_t*>(raw_video.data()), raw_video.size());
    response->set_payload(response_payload);
    this_app->send(response);
}

void on_availability_event(vsomeip::service_t Service, vsomeip::instance_t Instance, bool is_available)
{
    std::cout << "SERVICE: Service[" << std::setw(4) << std::setfill('0') << std::hex << 
    Service << "." << Instance << "] is " << (is_available ? "available." : "Not available.") << std::endl;

    if(is_available)
    {
    //  Sending wake-up call for the waiting thread on the service side after the detection event becomes available on the client side
    std::cout << "SERVICE: Condition lock will now be removed, since Service[" << std::setw(4) << std::setfill('0') 
    << Service << "." << Instance << "] is available now." << std::endl;
    
    condition.notify_one();
    }
}

void on_message_event(const std::shared_ptr<vsomeip::message>& event_message)
{
    /*  ACKNOWLEDGE EVENT_MESSAGE */

   std::stringstream message;

   message << "SERVICE: A notification for event [" << 
   std::setw(4) << std::setfill('0') << std::hex <<
   event_message->get_service() << "." << 
   event_message->get_instance() << "." <<
   std::setw(4) << std::setfill('0') << std::hex <<
   event_message->get_method() << "] to ClientID/Session [" <<
   std::setw(4) << std::setfill('0') << std::hex <<
   event_message->get_client() <<
   std::setw(4) << std::setfill('0') << std::hex <<
   event_message->get_session() << "]" << std::endl;

    /*  WRITE THE DATA FROM EVENT_MESSAGE INTO AN OBJECT_TYPE_T VARIABLE  */
    std::shared_ptr<vsomeip::payload> message_payload = event_message->get_payload();

    object_type_t detected_object;
    detected_object.type = (char)*(message_payload->get_data());
    detected_object.count = (int)*(message_payload->get_data() + 1);
    detected_object.time = (float)*(message_payload->get_data() + 2);

    Detection_Object Captured_object(detected_object);
    Captured_object.print_object();
    Captured_object.~Detection_Object();
}

void run_events()
{
    std::unique_lock<std::mutex> lock_events(mutex);
    condition.wait(lock_events);

    std::set<vsomeip::eventgroup_t> event_groups;
    event_groups.insert(EVENT_GROUP_ID);
    this_app->request_event(EVENT_SERVICE_ID, EVENT_INSTANCE_ID,EVENT_ID, event_groups, vsomeip::event_type_e::ET_UNKNOWN);
    this_app->subscribe(EVENT_SERVICE_ID, EVENT_INSTANCE_ID, EVENT_GROUP_ID);
}
