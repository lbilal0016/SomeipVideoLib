cmake_minimum_required (VERSION 3.10.2)

project(VideoReadWriteProject VERSION 1.0.0 LANGUAGES C CXX)

set (CMAKE_CXX_FLAGS "-g -std=c++0x")

find_package (vsomeip3 3.3.8 REQUIRED)
find_package( Boost 1.55 COMPONENTS system thread log REQUIRED )

include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.11.2
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(nlohmann_json)

find_package(OpenCV REQUIRED)

include_directories (
    ${Boost_INCLUDE_DIR}
    ${VSOMEIP3_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/Includes
    ${CMAKE_CURRENT_SOURCE_DIR}/Includes/Someiplib
    ${CMAKE_CURRENT_SOURCE_DIR}/Includes/Videolib
)

#   Compile service
add_executable(service ./service.cpp Includes/Someiplib/ServiceLib.cpp Includes/Videolib/VideoReadWrite.cpp Includes/Videolib/VideoData.pb.cc Includes/Videolib/ObjectData.pb.cc)
target_link_libraries(service vsomeip3 ${Boost_LIBRARIES} ${OpenCV_LIBS} nlohmann_json protobuf)

#   Compile client
add_executable(Client ./Client.cpp Includes/Someiplib/ClientLib.cpp Includes/Videolib/VideoReadWrite.cpp Includes/Videolib/VideoData.pb.cc Includes/Videolib/ObjectData.pb.cc)
target_link_libraries(Client vsomeip3 ${Boost_LIBRARIES} ${OpenCV_LIBS} nlohmann_json protobuf) 

#   Compile test
#add_executable(Test ./testFunctionalities.cpp Includes/Someiplib/ServiceLib.cpp Includes/Videolib/VideoReadWrite.cpp Includes/Videolib/VideoData.pb.cc Includes/Videolib/ObjectData.pb.cc)
#target_link_libraries(Test vsomeip3 ${Boost_LIBRARIES} ${OpenCV_LIBS} nlohmann_json protobuf)