#include <iomanip>
#include <iostream>
#include <sstream>

#include <fstream>
#include <vector>
#include <stdexcept>
#include <string.h>


#include <vsomeip/vsomeip.hpp>

#include "opencv4/opencv2/opencv.hpp"

#include "ServiceLib.h"

#include "VideoReadWrite.h"


std::shared_ptr <vsomeip::application> app;

int main(int argc, char *argv[])
{
    // Check if the correct number of command line arguments is provided
    if (argc != 2) {
        std::cerr << "Usage: " << argv[1] << " <Path to ConfigFile>\n";
        return 1;
    }
    std::string ConfigFile(argv[1]);
    VideoReadWrite::ReadConfigFile(ConfigFile);

    app = vsomeip::runtime::get()->create_application("Server");
    app->init();
    app->register_message_handler(SAMPLE_SERVICE_ID,SAMPLE_INSTANCE_ID,SAMPLE_METHOD_ID, on_message);
    app->offer_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    set_application(app);   //  Added for compatibility when client functions are packed in another object code.
    app->start();
}
