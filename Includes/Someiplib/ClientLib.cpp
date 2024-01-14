#include "ClientLib.h"
#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <condition_variable>
#include <thread>

#include "VideoReadWrite.h"

//std::shared_ptr< vsomeip::application > app;
std::mutex mutex;
std::condition_variable condition;
std::condition_variable condition_detection;

//  Executes print operations with a client tag
void client_printer(const std::stringstream &print_message)
{
    std::cout << "CLIENT : " <<
    print_message.str() << std::endl;
}

//  Executes print operations with a client tag
void client_printer(const std::string &print_message)
{
    std::cout << "CLIENT : " <<
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

//  This function will be run on the client side after the service is offered
void run()
{
    client_printer("run() : ");
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(lock);
    client_printer("lock is removed.");

    //  Creating the object for vsome request message
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    //  Setting service id, instance id, method id
    request->set_service(SAMPLE_SERVICE_ID);
    request->set_instance(SAMPLE_INSTANCE_ID);
    request->set_method(SAMPLE_METHOD_ID);
    
    #ifdef TCP_COMMUNICATION
    //  Use the following if you want to realise a TCP communication:
    request->set_reliable(true);
    #elif
    //  Use the following if you want to realise a UDP communication:
    request->set_reliable(false);
    #endif

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
client_printer("on_message() : ");
std::shared_ptr<vsomeip::payload> its_payload = response->get_payload();    //  Read the someip message payload to the its_payload variable
vsomeip::length_t len = its_payload->get_length();  //  extracting the length of the payload

std::vector<uint8_t> received_video_raw(its_payload->get_data(), its_payload->get_data() + len);    //  extracting the raw bytes

//  Print the received data
std::stringstream print_stream;
print_stream << "Received message with Client/Session ["
<<  std::setw(4) << std::setfill('0') << std::hex
<<  response->get_client() << "/"
<<  std::setw(4) << std::setfill('0') << std::hex
<<  response->get_session() << "]" << std::endl;

client_printer(print_stream);

/*  Extract payload and write it on a local directory   */

VideoData receivedVideo = DeserialiseVideoData(received_video_raw);

//  log message
client_printer("Video file is received and ready to be saved locally ...");
//std::cout << "Client : Video file is received and ready to be saved locally...\n";

VideoWrite(receivedVideo);

//  remove the lock on the object detection after video is written successfully.
condition_detection.notify_one();
}

void on_availability(vsomeip::service_t Service, vsomeip::instance_t Instance, bool is_available)
{
    client_printer("on_availability() : ");

//  Checking the availability of the service offered by the server

std::stringstream print_stream;
print_stream << "Service (Service.Instance) [" << std::setw(4) << std::setfill('0') << std::hex 
<< Service << "." << Instance << "] is " << (is_available ? "available" : "NOT available") << std::endl;

client_printer(print_stream);

if(is_available){
//  Sending wake-up call for the waiting thread on the client side after the service is available
client_printer("on_availability : service is available, condition lock will now be removed.");
condition.notify_one();
}
}

//  This function reads the configuration file in which input and output directories for the videos are set
void SomeIpLib::ReadConfigFile(const std::string &ConfigFile)
{
    CONFIGURATION_VIDEO_DETECTION = ConfigFile;
    client_printer("Video configuration for someip library has been successfully defined.");
}

void run_detection()
{
    std::unique_lock<std::mutex> lock_detection(mutex);
    condition_detection.wait(lock_detection);
    
    //  lock for the object detection is removed
    client_printer("Object detection is running ...");

    /*  FURTHER STEPS: 

    1.  Engineer a function for checking whether the received video can be opened */
    std::string Path_to_video = ConfigureInputOutput("Output", CONFIGURATION_VIDEO_DETECTION);
    CheckVideoFile(Path_to_video);

    /*
    2. Obtain the json configuration file for object detection*/ 
    std::string Path_to_detection_Json = ConfigureInputOutput("Detection", CONFIGURATION_VIDEO_DETECTION);
    std::stringstream print_stream;
    print_stream << "Path_to_detection_Json was configured as : " << Path_to_detection_Json << std::endl;
    client_printer(print_stream);

    /*
    3.  Using the configuration file, carry out detection process*/
    std::vector<object_type_t> detected_objects;
    DetectObjectsFromJson(Path_to_detection_Json, detected_objects);

    /*
    4.  Call the send_data function to send the quasi-detected objects and their quantities as events   */
    
    for (auto& objects : detected_objects)
    {
        client_printer("Sending data ...");
        send_data(objects);
    }
}

void send_data(object_type_t &object_data) {

    /*  CREATE PAYLOAD OBJECT   */
    std::shared_ptr<vsomeip::payload> payload = vsomeip::runtime::get() -> create_payload();

    /*  CREATE A VECTOR CONTAINER FOR PAYLOAD DATA  */
    std::vector<vsomeip::byte_t> payload_data;

    /*  CONSTRUCTING THE OBJECT TO BE SENT  */
    Detection_Object object_to_send(object_data);

    //  TEST OF SENDING DETECTED OBJECTS
    object_to_send.print_object();

    std::vector<uint8_t> payload_vector = object_to_send.GetSerializedObjectData();

    for(const auto &payload_element : payload_vector)
    {
        payload_data.push_back(payload_element);
    }

    payload->set_data(payload_data);
    
    this_app->notify(EVENT_SERVICE_ID, EVENT_INSTANCE_ID, EVENT_ID, payload);
    object_to_send.~Detection_Object();
}

void offer_client_event()
{
    //  Offering events as service
    this_app->offer_service(EVENT_SERVICE_ID, EVENT_INSTANCE_ID);
    //  Creating a set of event groups
    std::set<vsomeip::eventgroup_t> its_groups;
    its_groups.insert(EVENT_GROUP_ID);   //  Adding group id
    //  Offer event
    this_app->offer_event(EVENT_SERVICE_ID, EVENT_INSTANCE_ID, EVENT_ID, its_groups, vsomeip_v3::event_type_e::ET_SELECTIVE_EVENT, 
    std::chrono::milliseconds(1000),false,true,nullptr, vsomeip_v3::reliability_type_e::RT_UNKNOWN);

    //  LOG MESSAGE
    client_printer("Object detection events are now ready to be offered.");
}

void on_message_event(const std::shared_ptr<vsomeip::message> &response)
{
    std::shared_ptr<vsomeip::payload> payload = response->get_payload();
    vsomeip::length_t len = payload->get_length();

    //  Payload register
    std::stringstream string;
    std::stringstream print_string;
    for(vsomeip::length_t i = 0; i < len; ++i){
        string << std::setw(2) << std::setfill('0') << std::hex <<
        (int) (payload->get_data()[i]) << " ";
    }

    print_string << "Received message Client/Session ["
    << std::setw(4) << std::setfill('0') << std::hex
    << response->get_client() << "/"
    << response->get_session() << "]"
    << string.str() << std::endl;
    client_printer(print_string);
}