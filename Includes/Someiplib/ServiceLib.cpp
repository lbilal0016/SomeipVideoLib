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
    std::cout << message.str() << std::endl;

    static object_type_t detected_object;  //  content of current event message
    static object_type_t detected_object_old;   //  content of previous event message


/*  DIDN'T WORK, DATE = 10.01.2024
    detected_object.type = static_cast<char>((int)*(message_payload->get_data()));
    detected_object.count = (int)*(message_payload->get_data() + 1);
    detected_object.time = static_cast<float>((int)*(message_payload->get_data() + 2));
*/

/*  DIDN'T WORK, DATE = 10.04.2024
    std::vector<int> payload_buffer;
    for(int i = 0; i < message_payload->get_length();++i)
    {
        payload_buffer.push_back((int) (message_payload->get_data()[i]));
    }

    detected_object.type = (payload_buffer[0]);
    detected_object.count = payload_buffer[1];
    detected_object.type = (payload_buffer[2]);
*/

/*  DIDN'T WORK, DATE = 10-01-2024
    std::vector<vsomeip::byte_t> detected_object_raw(message_payload->get_data(), message_payload->get_data() + message_payload->get_length());

    detected_object.type = (char)detected_object_raw[0];
    detected_object.count = (int)detected_object_raw[1];
    detected_object.count = (float)detected_object_raw[2];
*/  
    /*  SAVING RECEIVED RAW DATA IN A UINT8_T VECTOR    */
    std::vector<uint8_t> received_object_raw(message_payload->get_data(), message_payload->get_data() + len);

    /*  CREATING A DETECTION OBJECT WITH RAW DATA   */
    Detection_Object received_object(received_object_raw);
    
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

    received_object.~Detection_Object();
}

void run_events()
{
    std::cout << "SERVER: run_events() is currently being hold.\n";
    std::unique_lock<std::mutex> lock_events(mutex);
    condition.wait(lock_events);
    std::cout << "SERVER: run_events() is now released.\n";

    std::set<vsomeip::eventgroup_t> event_groups;
    event_groups.insert(EVENT_GROUP_ID);
    this_app->request_event(EVENT_SERVICE_ID, EVENT_INSTANCE_ID,EVENT_ID, event_groups, vsomeip::event_type_e::ET_FIELD);
    this_app->subscribe(EVENT_SERVICE_ID, EVENT_INSTANCE_ID, EVENT_GROUP_ID);
}
