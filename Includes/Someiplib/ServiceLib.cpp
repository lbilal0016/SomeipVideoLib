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

//  Executes print operations with a client tag
void service_printer(const std::stringstream &print_message)
{
    std::cout << "SERVICE : " <<
    print_message.str() << std::endl;
}

//  Executes print operations with a client tag
void service_printer(const std::string &print_message)
{
    std::cout << "SERVICE : " <<
    print_message << std::endl;
}

//  This function passes the someip application object from the Client executable to this object code
static std::shared_ptr<vsomeip::application> this_app;
void set_application(std::shared_ptr<vsomeip::application> app)
{
    this_app = app;
}

//  This function passes the video object from client source to the object code
static Video_Object Video_Obj;
void set_video_object(Video_Object &video_obj)
{
    Video_Obj = video_obj;
}

void on_message(const std::shared_ptr<vsomeip::message>& Request)
{
    service_printer("on_message():");
    //  Reading the payload
    std::shared_ptr<vsomeip::payload> response_payload = Request->get_payload();
    //  Getting the payload length
    vsomeip::length_t len = response_payload->get_length();

    //  Getting the payload
    std::stringstream ss;
    std::stringstream print_stream;
    //  This initiates a loop that iterates over each byte of the payload.
    for(vsomeip::length_t i=0; i<len; ++i)
    {
        ss << std::setw(2) << std::setfill('0') << std::hex
        //  response_payload->get_data() returns a pointer to the start of the payload data
        //  << " ";: A space is added after each formatted hexadecimal value to separate them in the final string.
        <<(int)*(response_payload->get_data()+i) << " "; 
    }

    /*  ACKNOWLEDGE THE VIDEO REQUEST FROM THE CLIENT SIDE  */
    print_stream << "Received request with Client/Session [ "
    << std::setw(4) << std::setfill('0') << std::hex << Request->get_client() << "/"
    << std::setw(4) << std::setfill('0') << std::hex << Request->get_session() << "]"
    << ss.str();
    service_printer(print_stream);

    /*      READ THE INPUT FROM CONFIG AND PUT IT ON PAYLOAD    */
    //  Reading the input video file
    service_printer("Input file is being read ...");    //  log message
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
    std::stringstream print_stream;
    print_stream << "Service[" << std::setw(4) << std::setfill('0') << std::hex << 
    Service << "." << Instance << "] is " << (is_available ? "available." : "Not available.");
    service_printer(print_stream);
    print_stream.str("");   

    if(is_available)
    {
    //  Sending wake-up call for the waiting thread on the service side after the detection event becomes available on the client side
    print_stream << "Condition lock will now be removed, since Service[" << std::setw(4) << std::setfill('0') 
    << Service << "." << Instance << "] is available now.";
    service_printer(print_stream);
    
    condition.notify_one();
    }
}

void on_message_event(const std::shared_ptr<vsomeip::message>& event_message)
{
    /*  ACKNOWLEDGE EVENT_MESSAGE */

   std::stringstream message;
   std::stringstream print_stream;

   message << "SERVICE: A notification for event [" << 
   std::setw(4) << std::setfill('0') << std::hex <<
   event_message->get_service() << "." << 
   event_message->get_instance() << "." <<
   std::setw(4) << std::setfill('0') << std::hex <<
   event_message->get_method() << "] to ClientID/Session [" <<
   std::setw(4) << std::setfill('0') << std::hex <<
   event_message->get_client() << "." <<
   std::setw(4) << std::setfill('0') << std::hex <<
   event_message->get_session() << "] = ";

    /*  WRITE THE DATA FROM EVENT_MESSAGE INTO AN OBJECT_TYPE_T VARIABLE  */
    std::shared_ptr<vsomeip::payload> message_payload = event_message->get_payload();
    vsomeip::length_t len = message_payload->get_length();

    message << "(" << std::dec << len << ") ";
    for (uint32_t i = 0; i < len; ++i){
        message << std::hex << std::setw(2) << std::setfill('0')
        << (int) message_payload->get_data()[i] << " ";
    }
    print_stream << message.str();
    service_printer(print_stream);

    static object_type_t detected_object;  //  content of current event message
    static object_type_t detected_object_old;   //  content of previous event message
 
    /*  SAVING RECEIVED RAW DATA IN A UINT8_T VECTOR    */
    std::vector<uint8_t> received_object_raw(message_payload->get_data(), message_payload->get_data() + len);

    /*  CREATING A DETECTION OBJECT WITH RAW DATA   */
    Detection_Object received_object(received_object_raw, "Service");
    
    /*  DESERIALIZATION OF DETECTION OBJECT RAW DATA    */
    detected_object = received_object.GetDeserializedObjectData();

    /*  PRINTING RECEIVED OBJECT IF IT IS DIFFERENT THAN PREVIOUS ONE   */
    if( detected_object.type != detected_object_old.type ||
        detected_object.count != detected_object_old.count ||
        detected_object.time != detected_object_old.time)
    {
    std::cout << "Content: ";
    received_object.print_object();
    }

    /*  UNIT-DELAY FOR RECEIVED OBJECT FOR THE NEXT DETECTION   */
    detected_object_old.type = detected_object.type;
    detected_object_old.count = detected_object.count;
    detected_object_old.time = detected_object.time;

    //received_object.~Detection_Object();
}

void run_events()
{
    service_printer("run_events() is currently being hold.");
    std::unique_lock<std::mutex> lock_events(mutex);
    condition.wait(lock_events);
    service_printer("run_events() is now released.");

    std::set<vsomeip::eventgroup_t> event_groups;
    event_groups.insert(EVENT_GROUP_ID);
    this_app->request_event(EVENT_SERVICE_ID, EVENT_INSTANCE_ID,EVENT_ID, event_groups, vsomeip::event_type_e::ET_FIELD);
    this_app->subscribe(EVENT_SERVICE_ID, EVENT_INSTANCE_ID, EVENT_GROUP_ID);
}