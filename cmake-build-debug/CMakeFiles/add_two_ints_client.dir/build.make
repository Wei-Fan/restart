# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/wade/clion-2018.2.5/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/wade/clion-2018.2.5/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wade/catkin_ws/src/restart

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wade/catkin_ws/src/restart/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/add_two_ints_client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/add_two_ints_client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/add_two_ints_client.dir/flags.make

CMakeFiles/add_two_ints_client.dir/src/add_two_ints_client.cpp.o: CMakeFiles/add_two_ints_client.dir/flags.make
CMakeFiles/add_two_ints_client.dir/src/add_two_ints_client.cpp.o: ../src/add_two_ints_client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wade/catkin_ws/src/restart/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/add_two_ints_client.dir/src/add_two_ints_client.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/add_two_ints_client.dir/src/add_two_ints_client.cpp.o -c /home/wade/catkin_ws/src/restart/src/add_two_ints_client.cpp

CMakeFiles/add_two_ints_client.dir/src/add_two_ints_client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/add_two_ints_client.dir/src/add_two_ints_client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wade/catkin_ws/src/restart/src/add_two_ints_client.cpp > CMakeFiles/add_two_ints_client.dir/src/add_two_ints_client.cpp.i

CMakeFiles/add_two_ints_client.dir/src/add_two_ints_client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/add_two_ints_client.dir/src/add_two_ints_client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wade/catkin_ws/src/restart/src/add_two_ints_client.cpp -o CMakeFiles/add_two_ints_client.dir/src/add_two_ints_client.cpp.s

# Object files for target add_two_ints_client
add_two_ints_client_OBJECTS = \
"CMakeFiles/add_two_ints_client.dir/src/add_two_ints_client.cpp.o"

# External object files for target add_two_ints_client
add_two_ints_client_EXTERNAL_OBJECTS =

devel/lib/restart/add_two_ints_client: CMakeFiles/add_two_ints_client.dir/src/add_two_ints_client.cpp.o
devel/lib/restart/add_two_ints_client: CMakeFiles/add_two_ints_client.dir/build.make
devel/lib/restart/add_two_ints_client: /opt/ros/kinetic/lib/libcv_bridge.so
devel/lib/restart/add_two_ints_client: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_core3.so.3.3.1
devel/lib/restart/add_two_ints_client: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_imgproc3.so.3.3.1
devel/lib/restart/add_two_ints_client: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_imgcodecs3.so.3.3.1
devel/lib/restart/add_two_ints_client: /opt/ros/kinetic/lib/libimage_transport.so
devel/lib/restart/add_two_ints_client: /opt/ros/kinetic/lib/libmessage_filters.so
devel/lib/restart/add_two_ints_client: /usr/lib/x86_64-linux-gnu/libtinyxml2.so
devel/lib/restart/add_two_ints_client: /opt/ros/kinetic/lib/libclass_loader.so
devel/lib/restart/add_two_ints_client: /usr/lib/libPocoFoundation.so
devel/lib/restart/add_two_ints_client: /usr/lib/x86_64-linux-gnu/libdl.so
devel/lib/restart/add_two_ints_client: /opt/ros/kinetic/lib/libroslib.so
devel/lib/restart/add_two_ints_client: /opt/ros/kinetic/lib/librospack.so
devel/lib/restart/add_two_ints_client: /usr/lib/x86_64-linux-gnu/libpython2.7.so
devel/lib/restart/add_two_ints_client: /usr/lib/x86_64-linux-gnu/libboost_program_options.so
devel/lib/restart/add_two_ints_client: /usr/lib/x86_64-linux-gnu/libtinyxml.so
devel/lib/restart/add_two_ints_client: /opt/ros/kinetic/lib/libdynamic_reconfigure_config_init_mutex.so
devel/lib/restart/add_two_ints_client: /opt/ros/kinetic/lib/libroscpp.so
devel/lib/restart/add_two_ints_client: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
devel/lib/restart/add_two_ints_client: /usr/lib/x86_64-linux-gnu/libboost_signals.so
devel/lib/restart/add_two_ints_client: /opt/ros/kinetic/lib/librosconsole.so
devel/lib/restart/add_two_ints_client: /opt/ros/kinetic/lib/librosconsole_log4cxx.so
devel/lib/restart/add_two_ints_client: /opt/ros/kinetic/lib/librosconsole_backend_interface.so
devel/lib/restart/add_two_ints_client: /usr/lib/x86_64-linux-gnu/liblog4cxx.so
devel/lib/restart/add_two_ints_client: /usr/lib/x86_64-linux-gnu/libboost_regex.so
devel/lib/restart/add_two_ints_client: /opt/ros/kinetic/lib/libroscpp_serialization.so
devel/lib/restart/add_two_ints_client: /opt/ros/kinetic/lib/libxmlrpcpp.so
devel/lib/restart/add_two_ints_client: /opt/ros/kinetic/lib/librostime.so
devel/lib/restart/add_two_ints_client: /opt/ros/kinetic/lib/libcpp_common.so
devel/lib/restart/add_two_ints_client: /usr/lib/x86_64-linux-gnu/libboost_system.so
devel/lib/restart/add_two_ints_client: /usr/lib/x86_64-linux-gnu/libboost_thread.so
devel/lib/restart/add_two_ints_client: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
devel/lib/restart/add_two_ints_client: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
devel/lib/restart/add_two_ints_client: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
devel/lib/restart/add_two_ints_client: /usr/lib/x86_64-linux-gnu/libpthread.so
devel/lib/restart/add_two_ints_client: /usr/lib/x86_64-linux-gnu/libconsole_bridge.so
devel/lib/restart/add_two_ints_client: CMakeFiles/add_two_ints_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wade/catkin_ws/src/restart/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable devel/lib/restart/add_two_ints_client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/add_two_ints_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/add_two_ints_client.dir/build: devel/lib/restart/add_two_ints_client

.PHONY : CMakeFiles/add_two_ints_client.dir/build

CMakeFiles/add_two_ints_client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/add_two_ints_client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/add_two_ints_client.dir/clean

CMakeFiles/add_two_ints_client.dir/depend:
	cd /home/wade/catkin_ws/src/restart/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wade/catkin_ws/src/restart /home/wade/catkin_ws/src/restart /home/wade/catkin_ws/src/restart/cmake-build-debug /home/wade/catkin_ws/src/restart/cmake-build-debug /home/wade/catkin_ws/src/restart/cmake-build-debug/CMakeFiles/add_two_ints_client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/add_two_ints_client.dir/depend

