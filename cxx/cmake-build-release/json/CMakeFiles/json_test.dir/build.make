# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/dzhiblavi/Documents/prog/tgnews/cxx

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release

# Include any dependencies generated for this target.
include json/CMakeFiles/json_test.dir/depend.make

# Include the progress variables for this target.
include json/CMakeFiles/json_test.dir/progress.make

# Include the compile flags for this target's objects.
include json/CMakeFiles/json_test.dir/flags.make

json/CMakeFiles/json_test.dir/test_json.cpp.o: json/CMakeFiles/json_test.dir/flags.make
json/CMakeFiles/json_test.dir/test_json.cpp.o: ../json/test_json.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object json/CMakeFiles/json_test.dir/test_json.cpp.o"
	cd /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/json && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/json_test.dir/test_json.cpp.o -c /Users/dzhiblavi/Documents/prog/tgnews/cxx/json/test_json.cpp

json/CMakeFiles/json_test.dir/test_json.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/json_test.dir/test_json.cpp.i"
	cd /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/json && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/dzhiblavi/Documents/prog/tgnews/cxx/json/test_json.cpp > CMakeFiles/json_test.dir/test_json.cpp.i

json/CMakeFiles/json_test.dir/test_json.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/json_test.dir/test_json.cpp.s"
	cd /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/json && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/dzhiblavi/Documents/prog/tgnews/cxx/json/test_json.cpp -o CMakeFiles/json_test.dir/test_json.cpp.s

json/CMakeFiles/json_test.dir/json.cpp.o: json/CMakeFiles/json_test.dir/flags.make
json/CMakeFiles/json_test.dir/json.cpp.o: ../json/json.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object json/CMakeFiles/json_test.dir/json.cpp.o"
	cd /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/json && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/json_test.dir/json.cpp.o -c /Users/dzhiblavi/Documents/prog/tgnews/cxx/json/json.cpp

json/CMakeFiles/json_test.dir/json.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/json_test.dir/json.cpp.i"
	cd /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/json && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/dzhiblavi/Documents/prog/tgnews/cxx/json/json.cpp > CMakeFiles/json_test.dir/json.cpp.i

json/CMakeFiles/json_test.dir/json.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/json_test.dir/json.cpp.s"
	cd /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/json && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/dzhiblavi/Documents/prog/tgnews/cxx/json/json.cpp -o CMakeFiles/json_test.dir/json.cpp.s

json/CMakeFiles/json_test.dir/__/gtest/gtest-all.cc.o: json/CMakeFiles/json_test.dir/flags.make
json/CMakeFiles/json_test.dir/__/gtest/gtest-all.cc.o: ../gtest/gtest-all.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object json/CMakeFiles/json_test.dir/__/gtest/gtest-all.cc.o"
	cd /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/json && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/json_test.dir/__/gtest/gtest-all.cc.o -c /Users/dzhiblavi/Documents/prog/tgnews/cxx/gtest/gtest-all.cc

json/CMakeFiles/json_test.dir/__/gtest/gtest-all.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/json_test.dir/__/gtest/gtest-all.cc.i"
	cd /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/json && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/dzhiblavi/Documents/prog/tgnews/cxx/gtest/gtest-all.cc > CMakeFiles/json_test.dir/__/gtest/gtest-all.cc.i

json/CMakeFiles/json_test.dir/__/gtest/gtest-all.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/json_test.dir/__/gtest/gtest-all.cc.s"
	cd /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/json && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/dzhiblavi/Documents/prog/tgnews/cxx/gtest/gtest-all.cc -o CMakeFiles/json_test.dir/__/gtest/gtest-all.cc.s

json/CMakeFiles/json_test.dir/__/gtest/gtest_main.cc.o: json/CMakeFiles/json_test.dir/flags.make
json/CMakeFiles/json_test.dir/__/gtest/gtest_main.cc.o: ../gtest/gtest_main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object json/CMakeFiles/json_test.dir/__/gtest/gtest_main.cc.o"
	cd /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/json && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/json_test.dir/__/gtest/gtest_main.cc.o -c /Users/dzhiblavi/Documents/prog/tgnews/cxx/gtest/gtest_main.cc

json/CMakeFiles/json_test.dir/__/gtest/gtest_main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/json_test.dir/__/gtest/gtest_main.cc.i"
	cd /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/json && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/dzhiblavi/Documents/prog/tgnews/cxx/gtest/gtest_main.cc > CMakeFiles/json_test.dir/__/gtest/gtest_main.cc.i

json/CMakeFiles/json_test.dir/__/gtest/gtest_main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/json_test.dir/__/gtest/gtest_main.cc.s"
	cd /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/json && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/dzhiblavi/Documents/prog/tgnews/cxx/gtest/gtest_main.cc -o CMakeFiles/json_test.dir/__/gtest/gtest_main.cc.s

# Object files for target json_test
json_test_OBJECTS = \
"CMakeFiles/json_test.dir/test_json.cpp.o" \
"CMakeFiles/json_test.dir/json.cpp.o" \
"CMakeFiles/json_test.dir/__/gtest/gtest-all.cc.o" \
"CMakeFiles/json_test.dir/__/gtest/gtest_main.cc.o"

# External object files for target json_test
json_test_EXTERNAL_OBJECTS =

json/json_test: json/CMakeFiles/json_test.dir/test_json.cpp.o
json/json_test: json/CMakeFiles/json_test.dir/json.cpp.o
json/json_test: json/CMakeFiles/json_test.dir/__/gtest/gtest-all.cc.o
json/json_test: json/CMakeFiles/json_test.dir/__/gtest/gtest_main.cc.o
json/json_test: json/CMakeFiles/json_test.dir/build.make
json/json_test: json/CMakeFiles/json_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable json_test"
	cd /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/json && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/json_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
json/CMakeFiles/json_test.dir/build: json/json_test

.PHONY : json/CMakeFiles/json_test.dir/build

json/CMakeFiles/json_test.dir/clean:
	cd /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/json && $(CMAKE_COMMAND) -P CMakeFiles/json_test.dir/cmake_clean.cmake
.PHONY : json/CMakeFiles/json_test.dir/clean

json/CMakeFiles/json_test.dir/depend:
	cd /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/dzhiblavi/Documents/prog/tgnews/cxx /Users/dzhiblavi/Documents/prog/tgnews/cxx/json /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/json /Users/dzhiblavi/Documents/prog/tgnews/cxx/cmake-build-release/json/CMakeFiles/json_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : json/CMakeFiles/json_test.dir/depend

