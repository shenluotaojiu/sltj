# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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

# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_SOURCE_DIR = /home/taojiu/桌面/sltj

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/taojiu/桌面/sltj/build

# Include any dependencies generated for this target.
include CMakeFiles/sltj.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/sltj.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/sltj.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sltj.dir/flags.make

CMakeFiles/sltj.dir/src/log.cc.o: CMakeFiles/sltj.dir/flags.make
CMakeFiles/sltj.dir/src/log.cc.o: /home/taojiu/桌面/sltj/src/log.cc
CMakeFiles/sltj.dir/src/log.cc.o: CMakeFiles/sltj.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/taojiu/桌面/sltj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sltj.dir/src/log.cc.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sltj.dir/src/log.cc.o -MF CMakeFiles/sltj.dir/src/log.cc.o.d -o CMakeFiles/sltj.dir/src/log.cc.o -c /home/taojiu/桌面/sltj/src/log.cc

CMakeFiles/sltj.dir/src/log.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/sltj.dir/src/log.cc.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/taojiu/桌面/sltj/src/log.cc > CMakeFiles/sltj.dir/src/log.cc.i

CMakeFiles/sltj.dir/src/log.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/sltj.dir/src/log.cc.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/taojiu/桌面/sltj/src/log.cc -o CMakeFiles/sltj.dir/src/log.cc.s

CMakeFiles/sltj.dir/src/util.cc.o: CMakeFiles/sltj.dir/flags.make
CMakeFiles/sltj.dir/src/util.cc.o: /home/taojiu/桌面/sltj/src/util.cc
CMakeFiles/sltj.dir/src/util.cc.o: CMakeFiles/sltj.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/taojiu/桌面/sltj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/sltj.dir/src/util.cc.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sltj.dir/src/util.cc.o -MF CMakeFiles/sltj.dir/src/util.cc.o.d -o CMakeFiles/sltj.dir/src/util.cc.o -c /home/taojiu/桌面/sltj/src/util.cc

CMakeFiles/sltj.dir/src/util.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/sltj.dir/src/util.cc.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/taojiu/桌面/sltj/src/util.cc > CMakeFiles/sltj.dir/src/util.cc.i

CMakeFiles/sltj.dir/src/util.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/sltj.dir/src/util.cc.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/taojiu/桌面/sltj/src/util.cc -o CMakeFiles/sltj.dir/src/util.cc.s

CMakeFiles/sltj.dir/src/config.cc.o: CMakeFiles/sltj.dir/flags.make
CMakeFiles/sltj.dir/src/config.cc.o: /home/taojiu/桌面/sltj/src/config.cc
CMakeFiles/sltj.dir/src/config.cc.o: CMakeFiles/sltj.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/taojiu/桌面/sltj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/sltj.dir/src/config.cc.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sltj.dir/src/config.cc.o -MF CMakeFiles/sltj.dir/src/config.cc.o.d -o CMakeFiles/sltj.dir/src/config.cc.o -c /home/taojiu/桌面/sltj/src/config.cc

CMakeFiles/sltj.dir/src/config.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/sltj.dir/src/config.cc.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/taojiu/桌面/sltj/src/config.cc > CMakeFiles/sltj.dir/src/config.cc.i

CMakeFiles/sltj.dir/src/config.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/sltj.dir/src/config.cc.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/taojiu/桌面/sltj/src/config.cc -o CMakeFiles/sltj.dir/src/config.cc.s

CMakeFiles/sltj.dir/src/thread.cc.o: CMakeFiles/sltj.dir/flags.make
CMakeFiles/sltj.dir/src/thread.cc.o: /home/taojiu/桌面/sltj/src/thread.cc
CMakeFiles/sltj.dir/src/thread.cc.o: CMakeFiles/sltj.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/taojiu/桌面/sltj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/sltj.dir/src/thread.cc.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sltj.dir/src/thread.cc.o -MF CMakeFiles/sltj.dir/src/thread.cc.o.d -o CMakeFiles/sltj.dir/src/thread.cc.o -c /home/taojiu/桌面/sltj/src/thread.cc

CMakeFiles/sltj.dir/src/thread.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/sltj.dir/src/thread.cc.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/taojiu/桌面/sltj/src/thread.cc > CMakeFiles/sltj.dir/src/thread.cc.i

CMakeFiles/sltj.dir/src/thread.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/sltj.dir/src/thread.cc.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/taojiu/桌面/sltj/src/thread.cc -o CMakeFiles/sltj.dir/src/thread.cc.s

# Object files for target sltj
sltj_OBJECTS = \
"CMakeFiles/sltj.dir/src/log.cc.o" \
"CMakeFiles/sltj.dir/src/util.cc.o" \
"CMakeFiles/sltj.dir/src/config.cc.o" \
"CMakeFiles/sltj.dir/src/thread.cc.o"

# External object files for target sltj
sltj_EXTERNAL_OBJECTS =

/home/taojiu/桌面/sltj/lib/libsltj.so: CMakeFiles/sltj.dir/src/log.cc.o
/home/taojiu/桌面/sltj/lib/libsltj.so: CMakeFiles/sltj.dir/src/util.cc.o
/home/taojiu/桌面/sltj/lib/libsltj.so: CMakeFiles/sltj.dir/src/config.cc.o
/home/taojiu/桌面/sltj/lib/libsltj.so: CMakeFiles/sltj.dir/src/thread.cc.o
/home/taojiu/桌面/sltj/lib/libsltj.so: CMakeFiles/sltj.dir/build.make
/home/taojiu/桌面/sltj/lib/libsltj.so: CMakeFiles/sltj.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/taojiu/桌面/sltj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX shared library /home/taojiu/桌面/sltj/lib/libsltj.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sltj.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sltj.dir/build: /home/taojiu/桌面/sltj/lib/libsltj.so
.PHONY : CMakeFiles/sltj.dir/build

CMakeFiles/sltj.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sltj.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sltj.dir/clean

CMakeFiles/sltj.dir/depend:
	cd /home/taojiu/桌面/sltj/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/taojiu/桌面/sltj /home/taojiu/桌面/sltj /home/taojiu/桌面/sltj/build /home/taojiu/桌面/sltj/build /home/taojiu/桌面/sltj/build/CMakeFiles/sltj.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/sltj.dir/depend

