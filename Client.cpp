#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <fstream>
#include <vector>
#include <stdexcept>
#include <string.h>


#include <condition_variable>
#include <thread>

#include "opencv4/opencv2/opencv.hpp"

#include "VideoReadWrite.h"

#include "ClientLib.h"

std::shared_ptr< vsomeip::application > app;

int main(int argc, char *argv[]) {

    // Check if the correct number of command line arguments is provided
    if (argc != 2) {
        std::cerr << "Usage: " << argv[1] << " <Path to ConfigFile>\n";
        return 1;
    }
    std::string ConfigFile(argv[1]);
    //  Video configuration file is read for video library
    VideoReadWrite::ReadConfigFile(ConfigFile);
    //  Video configuration file is read for vsomeip library
    SomeIpLib::ReadConfigFile(ConfigFile);

    app = vsomeip::runtime::get()->create_application("Client");
    set_application(app);   //  Added for compatibility when client functions are packed in another object code.
    app->init();
    app->register_availability_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, on_availability);
    app->request_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    app->register_message_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_METHOD_ID, on_message);
    app->register_message_handler(EVENT_SERVICE_ID, EVENT_INSTANCE_ID, EVENT_METHOD_ID, on_message_event);

    //  OBJECT DETECTION EVENTS
    offer_client_event();
    
    std::thread sender(run);
    std::thread object_detection(run_detection);

    app->start();
    
    //object_detection.join();    //  Terminate the object_detection function somewhere
}
