# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.2.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.2.1\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Dataingenior\Semester_4\Nettvreksprogrammering\programmeringsprosjekt2\IDATT2104---Project---Onion-router

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Dataingenior\Semester_4\Nettvreksprogrammering\programmeringsprosjekt2\IDATT2104---Project---Onion-router\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/IDATT2104___Project___Onion_router.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/IDATT2104___Project___Onion_router.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/IDATT2104___Project___Onion_router.dir/flags.make

CMakeFiles/IDATT2104___Project___Onion_router.dir/main.cpp.obj: CMakeFiles/IDATT2104___Project___Onion_router.dir/flags.make
CMakeFiles/IDATT2104___Project___Onion_router.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Dataingenior\Semester_4\Nettvreksprogrammering\programmeringsprosjekt2\IDATT2104---Project---Onion-router\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/IDATT2104___Project___Onion_router.dir/main.cpp.obj"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\IDATT2104___Project___Onion_router.dir\main.cpp.obj -c C:\Dataingenior\Semester_4\Nettvreksprogrammering\programmeringsprosjekt2\IDATT2104---Project---Onion-router\main.cpp

CMakeFiles/IDATT2104___Project___Onion_router.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/IDATT2104___Project___Onion_router.dir/main.cpp.i"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Dataingenior\Semester_4\Nettvreksprogrammering\programmeringsprosjekt2\IDATT2104---Project---Onion-router\main.cpp > CMakeFiles\IDATT2104___Project___Onion_router.dir\main.cpp.i

CMakeFiles/IDATT2104___Project___Onion_router.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/IDATT2104___Project___Onion_router.dir/main.cpp.s"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Dataingenior\Semester_4\Nettvreksprogrammering\programmeringsprosjekt2\IDATT2104---Project---Onion-router\main.cpp -o CMakeFiles\IDATT2104___Project___Onion_router.dir\main.cpp.s

# Object files for target IDATT2104___Project___Onion_router
IDATT2104___Project___Onion_router_OBJECTS = \
"CMakeFiles/IDATT2104___Project___Onion_router.dir/main.cpp.obj"

# External object files for target IDATT2104___Project___Onion_router
IDATT2104___Project___Onion_router_EXTERNAL_OBJECTS =

IDATT2104___Project___Onion_router.exe: CMakeFiles/IDATT2104___Project___Onion_router.dir/main.cpp.obj
IDATT2104___Project___Onion_router.exe: CMakeFiles/IDATT2104___Project___Onion_router.dir/build.make
IDATT2104___Project___Onion_router.exe: CMakeFiles/IDATT2104___Project___Onion_router.dir/linklibs.rsp
IDATT2104___Project___Onion_router.exe: CMakeFiles/IDATT2104___Project___Onion_router.dir/objects1.rsp
IDATT2104___Project___Onion_router.exe: CMakeFiles/IDATT2104___Project___Onion_router.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Dataingenior\Semester_4\Nettvreksprogrammering\programmeringsprosjekt2\IDATT2104---Project---Onion-router\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable IDATT2104___Project___Onion_router.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\IDATT2104___Project___Onion_router.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/IDATT2104___Project___Onion_router.dir/build: IDATT2104___Project___Onion_router.exe
.PHONY : CMakeFiles/IDATT2104___Project___Onion_router.dir/build

CMakeFiles/IDATT2104___Project___Onion_router.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\IDATT2104___Project___Onion_router.dir\cmake_clean.cmake
.PHONY : CMakeFiles/IDATT2104___Project___Onion_router.dir/clean

CMakeFiles/IDATT2104___Project___Onion_router.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Dataingenior\Semester_4\Nettvreksprogrammering\programmeringsprosjekt2\IDATT2104---Project---Onion-router C:\Dataingenior\Semester_4\Nettvreksprogrammering\programmeringsprosjekt2\IDATT2104---Project---Onion-router C:\Dataingenior\Semester_4\Nettvreksprogrammering\programmeringsprosjekt2\IDATT2104---Project---Onion-router\cmake-build-debug C:\Dataingenior\Semester_4\Nettvreksprogrammering\programmeringsprosjekt2\IDATT2104---Project---Onion-router\cmake-build-debug C:\Dataingenior\Semester_4\Nettvreksprogrammering\programmeringsprosjekt2\IDATT2104---Project---Onion-router\cmake-build-debug\CMakeFiles\IDATT2104___Project___Onion_router.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/IDATT2104___Project___Onion_router.dir/depend
