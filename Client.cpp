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
    ReadConfigFile(ConfigFile);


    app = vsomeip::runtime::get()->create_application("Client");
    app->init();
    app->register_availability_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, on_availability);
    app->request_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    app->register_message_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_METHOD_ID, on_message);
    std::thread sender(run);
    set_application(app);   //  Added for compatibility when client functions are packed in another object code.
    app->start();
}
