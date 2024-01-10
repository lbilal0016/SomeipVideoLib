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


//  This function passes the someip application object from the Client executable to this object code
static std::shared_ptr<vsomeip::application> this_app;
void set_application(std::shared_ptr<vsomeip::application> app)
{
    this_app = app;
}

//  This function will be run on the client side after the service is offered
void run()
{
    std::cout << "CLIENT: run() : \n";
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(lock);
    std::cout << "CLIENT: lock is removed\n";

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
std::cout << "CLIENT: on_message() : \n";
std::shared_ptr<vsomeip::payload> its_payload = response->get_payload();    //  Read the someip message payload to the its_payload variable
vsomeip::length_t len = its_payload->get_length();  //  extracting the length of the payload

std::vector<uint8_t> received_video_raw(its_payload->get_data(), its_payload->get_data() + len);    //  extracting the raw bytes

//  Print the received data
std::cout << "CLIENT: Received message with Client/Session ["
<<  std::setw(4) << std::setfill('0') << std::hex
<<  response->get_client() << "/"
<<  std::setw(4) << std::setfill('0') << std::hex
<<  response->get_session() << "]" << std::endl;

/*  Extract payload and write it on a local directory   */

VideoData receivedVideo = DeserialiseVideoData(received_video_raw);

//  log message
std::cout << "Client : Video file is received and ready to be saved locally...\n";

VideoWrite(receivedVideo);

//  remove the lock on the object detection after video is written successfully.
condition_detection.notify_one();
}

void on_availability(vsomeip::service_t Service, vsomeip::instance_t Instance, bool is_available)
{
    std::cout << "CLIENT: on_availability() : \n";
//  Checking the availability of the service offered by the server
//	    std::setw(4) -> set the width of hexadecimal number to 4
//	    std::setfill(0) -> if the service id has less than 4 digits, fill the leading digits with 0s.
std::cout << "CLIENT: Service (Service.Instance) [" << std::setw(4) << std::setfill('0') << std::hex 
<< Service << "." << Instance << "] is " << (is_available ? "available" : "NOT available") << std::endl;

if(is_available){
//  Sending wake-up call for the waiting thread on the client side after the service is available
std::cout << "on_availability : service is available, condition lock will now be removed.\n";
condition.notify_one();
}
}

//  This function reads the configuration file in which input and output directories for the videos are set
void SomeIpLib::ReadConfigFile(const std::string &ConfigFile)
{
    CONFIGURATION_VIDEO_DETECTION = ConfigFile;
    std::cout << "CLIENT: Video configuration for someip library has been successfully defined.\n";
}

void run_detection()
{
    std::unique_lock<std::mutex> lock_detection(mutex);
    condition_detection.wait(lock_detection);
    
    //  lock for the object detection is removed
    std::cout << "CLIENT : Object detection is running ...\n";


    /*  FURTHER STEPS: 

    1.  Engineer a function for checking whether the received video can be opened */
    std::string Path_to_video = ConfigureInputOutput("Output", CONFIGURATION_VIDEO_DETECTION);
    CheckVideoFile(Path_to_video);

    /*
    2. Obtain the json configuration file for object detection*/ 
    std::string Path_to_detection_Json = ConfigureInputOutput("Detection", CONFIGURATION_VIDEO_DETECTION);
    std::cout << "Path_to_detection_Json was configured as : " << Path_to_detection_Json << std::endl;

    /*
    3.  Using the configuration file, carry out detection process*/
    std::vector<object_type_t> detected_objects;
    DetectObjectsFromJson(Path_to_detection_Json, detected_objects);

    /*
    3.a.    Offer events from the clients' side
    */

    //offer_client_event();

    //  TEST OF PRINTING DETECTED OBJECTS
    /*for(const auto &objects : detected_objects)
    {
        Detection_Object test_objects(objects);
        test_objects.print_object();
    }*/

    /*
    4.  Call the send_data function to send the quasi-detected objects and their quantities as events   */
    
    for (auto& objects : detected_objects)
    {
        std::cout << "CLIENT: Sending data ... \n";
        send_data(objects);
        //  TEST OF SENDING DETECTED OBJECTS
        Detection_Object test_objects(objects);
        test_objects.print_object();
    }
    
    //  send_data(detected_objects);
}

void send_data(object_type_t &object_data) {

  std::shared_ptr<vsomeip::payload> payload = vsomeip::runtime::get() -> create_payload();

  std::vector<vsomeip::byte_t> payload_data;


  payload_data.push_back(object_data.type);
  payload_data.push_back(object_data.count);
  payload_data.push_back(object_data.time);
  
  payload->set_data(payload_data);


//    payload->set_data(reinterpret_cast<const vsomeip::byte_t*>(object_data.data()), object_data.size());
  
  this_app->notify(EVENT_SERVICE_ID, EVENT_INSTANCE_ID, EVENT_ID, payload);
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
    std::cout << "CLIENT: Object detection events are now ready to be offered.\n";
}

void on_message_event(const std::shared_ptr<vsomeip::message> &response)
{
    std::shared_ptr<vsomeip::payload> payload = response->get_payload();
    vsomeip::length_t len = payload->get_length();

    //  Payload register
    std::stringstream string;
    for(vsomeip::length_t i = 0; i < len; ++i){
        string << std::setw(2) << std::setfill('0') << std::hex <<
        (int) (payload->get_data()[i]) << " ";
    }

    std::cout << "CLIENT : Received message Client/Session ["
    << std::setw(4) << std::setfill('0') << std::hex
    << response->get_client() << "/"
    << response->get_session() << "]"
    << string.str() << std::endl;
}