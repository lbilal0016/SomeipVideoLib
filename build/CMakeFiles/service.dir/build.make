# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ubuntu1/SomeipLibProject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu1/SomeipLibProject/build

# Include any dependencies generated for this target.
include CMakeFiles/service.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/service.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/service.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/service.dir/flags.make

CMakeFiles/service.dir/service.cpp.o: CMakeFiles/service.dir/flags.make
CMakeFiles/service.dir/service.cpp.o: ../service.cpp
CMakeFiles/service.dir/service.cpp.o: CMakeFiles/service.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu1/SomeipLibProject/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/service.dir/service.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/service.dir/service.cpp.o -MF CMakeFiles/service.dir/service.cpp.o.d -o CMakeFiles/service.dir/service.cpp.o -c /home/ubuntu1/SomeipLibProject/service.cpp

CMakeFiles/service.dir/service.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/service.dir/service.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu1/SomeipLibProject/service.cpp > CMakeFiles/service.dir/service.cpp.i

CMakeFiles/service.dir/service.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/service.dir/service.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu1/SomeipLibProject/service.cpp -o CMakeFiles/service.dir/service.cpp.s

CMakeFiles/service.dir/Includes/ServiceLib.cpp.o: CMakeFiles/service.dir/flags.make
CMakeFiles/service.dir/Includes/ServiceLib.cpp.o: ../Includes/ServiceLib.cpp
CMakeFiles/service.dir/Includes/ServiceLib.cpp.o: CMakeFiles/service.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu1/SomeipLibProject/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/service.dir/Includes/ServiceLib.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/service.dir/Includes/ServiceLib.cpp.o -MF CMakeFiles/service.dir/Includes/ServiceLib.cpp.o.d -o CMakeFiles/service.dir/Includes/ServiceLib.cpp.o -c /home/ubuntu1/SomeipLibProject/Includes/ServiceLib.cpp

CMakeFiles/service.dir/Includes/ServiceLib.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/service.dir/Includes/ServiceLib.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu1/SomeipLibProject/Includes/ServiceLib.cpp > CMakeFiles/service.dir/Includes/ServiceLib.cpp.i

CMakeFiles/service.dir/Includes/ServiceLib.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/service.dir/Includes/ServiceLib.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu1/SomeipLibProject/Includes/ServiceLib.cpp -o CMakeFiles/service.dir/Includes/ServiceLib.cpp.s

CMakeFiles/service.dir/Includes/VideoReadWrite.cpp.o: CMakeFiles/service.dir/flags.make
CMakeFiles/service.dir/Includes/VideoReadWrite.cpp.o: ../Includes/VideoReadWrite.cpp
CMakeFiles/service.dir/Includes/VideoReadWrite.cpp.o: CMakeFiles/service.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu1/SomeipLibProject/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/service.dir/Includes/VideoReadWrite.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/service.dir/Includes/VideoReadWrite.cpp.o -MF CMakeFiles/service.dir/Includes/VideoReadWrite.cpp.o.d -o CMakeFiles/service.dir/Includes/VideoReadWrite.cpp.o -c /home/ubuntu1/SomeipLibProject/Includes/VideoReadWrite.cpp

CMakeFiles/service.dir/Includes/VideoReadWrite.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/service.dir/Includes/VideoReadWrite.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu1/SomeipLibProject/Includes/VideoReadWrite.cpp > CMakeFiles/service.dir/Includes/VideoReadWrite.cpp.i

CMakeFiles/service.dir/Includes/VideoReadWrite.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/service.dir/Includes/VideoReadWrite.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu1/SomeipLibProject/Includes/VideoReadWrite.cpp -o CMakeFiles/service.dir/Includes/VideoReadWrite.cpp.s

# Object files for target service
service_OBJECTS = \
"CMakeFiles/service.dir/service.cpp.o" \
"CMakeFiles/service.dir/Includes/ServiceLib.cpp.o" \
"CMakeFiles/service.dir/Includes/VideoReadWrite.cpp.o"

# External object files for target service
service_EXTERNAL_OBJECTS =

service: CMakeFiles/service.dir/service.cpp.o
service: CMakeFiles/service.dir/Includes/ServiceLib.cpp.o
service: CMakeFiles/service.dir/Includes/VideoReadWrite.cpp.o
service: CMakeFiles/service.dir/build.make
service: /usr/local/lib/libvsomeip3.so.3.3.8
service: /usr/local/lib/libboost_system.so.1.82.0
service: /usr/local/lib/libboost_log.so.1.82.0
service: /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_alphamat.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_aruco.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_barcode.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_bgsegm.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_bioinspired.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_ccalib.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_dnn_objdetect.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_dnn_superres.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_dpm.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_face.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_freetype.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_fuzzy.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_hdf.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_hfs.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_img_hash.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_intensity_transform.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_line_descriptor.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_mcc.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_quality.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_rapid.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_reg.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_rgbd.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_saliency.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_shape.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_stereo.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_structured_light.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_superres.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_surface_matching.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_tracking.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_videostab.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_viz.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_wechat_qrcode.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_xobjdetect.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_xphoto.so.4.5.4d
service: /usr/local/lib/libboost_thread.so.1.82.0
service: /usr/local/lib/libboost_chrono.so.1.82.0
service: /usr/local/lib/libboost_filesystem.so.1.82.0
service: /usr/local/lib/libboost_atomic.so.1.82.0
service: /usr/local/lib/libboost_regex.so.1.82.0
service: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_datasets.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_plot.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_text.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_phase_unwrapping.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_optflow.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_ximgproc.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_video.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_videoio.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_imgcodecs.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_dnn.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.4.5.4d
service: /usr/lib/x86_64-linux-gnu/libopencv_core.so.4.5.4d
service: CMakeFiles/service.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu1/SomeipLibProject/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable service"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/service.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/service.dir/build: service
.PHONY : CMakeFiles/service.dir/build

CMakeFiles/service.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/service.dir/cmake_clean.cmake
.PHONY : CMakeFiles/service.dir/clean

CMakeFiles/service.dir/depend:
	cd /home/ubuntu1/SomeipLibProject/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu1/SomeipLibProject /home/ubuntu1/SomeipLibProject /home/ubuntu1/SomeipLibProject/build /home/ubuntu1/SomeipLibProject/build /home/ubuntu1/SomeipLibProject/build/CMakeFiles/service.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/service.dir/depend

