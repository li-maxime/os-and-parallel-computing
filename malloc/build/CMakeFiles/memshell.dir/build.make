# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/maxime/Desktop/SEPC/malloc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maxime/Desktop/SEPC/malloc/build

# Include any dependencies generated for this target.
include CMakeFiles/memshell.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/memshell.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/memshell.dir/flags.make

CMakeFiles/memshell.dir/src/memshell.c.o: CMakeFiles/memshell.dir/flags.make
CMakeFiles/memshell.dir/src/memshell.c.o: ../src/memshell.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maxime/Desktop/SEPC/malloc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/memshell.dir/src/memshell.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/memshell.dir/src/memshell.c.o   -c /home/maxime/Desktop/SEPC/malloc/src/memshell.c

CMakeFiles/memshell.dir/src/memshell.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/memshell.dir/src/memshell.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/maxime/Desktop/SEPC/malloc/src/memshell.c > CMakeFiles/memshell.dir/src/memshell.c.i

CMakeFiles/memshell.dir/src/memshell.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/memshell.dir/src/memshell.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/maxime/Desktop/SEPC/malloc/src/memshell.c -o CMakeFiles/memshell.dir/src/memshell.c.s

# Object files for target memshell
memshell_OBJECTS = \
"CMakeFiles/memshell.dir/src/memshell.c.o"

# External object files for target memshell
memshell_EXTERNAL_OBJECTS =

memshell: CMakeFiles/memshell.dir/src/memshell.c.o
memshell: CMakeFiles/memshell.dir/build.make
memshell: libemalloc.so
memshell: CMakeFiles/memshell.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/maxime/Desktop/SEPC/malloc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable memshell"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/memshell.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/memshell.dir/build: memshell

.PHONY : CMakeFiles/memshell.dir/build

CMakeFiles/memshell.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/memshell.dir/cmake_clean.cmake
.PHONY : CMakeFiles/memshell.dir/clean

CMakeFiles/memshell.dir/depend:
	cd /home/maxime/Desktop/SEPC/malloc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maxime/Desktop/SEPC/malloc /home/maxime/Desktop/SEPC/malloc /home/maxime/Desktop/SEPC/malloc/build /home/maxime/Desktop/SEPC/malloc/build /home/maxime/Desktop/SEPC/malloc/build/CMakeFiles/memshell.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/memshell.dir/depend

