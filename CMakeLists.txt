cmake_minimum_required(VERSION 2.8.11)

project(cracker)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set (CMAKE_CXX_STANDARD 11)

IF (WIN32)
    # Set the path to your qt setup here
    set (CMAKE_PREFIX_PATH "C:/Qt/5.9/mingw53_32/")
ENDIF()

# Find the QtCore library
find_package(Qt5Core)
get_target_property(QtCore_location Qt5::Core LOCATION)

# Tell CMake to create the helloworld executable
add_executable(cracker
        cracker/main.cpp
        cracker/keyspacethread.cpp
        cracker/runthread.cpp

        cracker/keyspacethread.h
        cracker/runthread.h)

# Use the Widgets module from Qt 5.
target_link_libraries(cracker Qt5::Core)