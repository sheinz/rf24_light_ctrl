# This configuration file outlines some of the ways Arduino CMake
# can be used. For a detailed explenation of all the options please
# reade README.rst.

set(ARDUINO_DEFAULT_BOARD diecimila) # Default Board ID, when not specified
#set(ARDUINO_DEFAULT_PORT /dev/ttyUSB0) # Default Port, when not specified
set(ARDUINO_DEFAULT_PORT /dev/rfcomm3) # Default Port, when not specified

# for eclipse indexer
#add_definitions(-DARDUINO=105)
# 
include_directories(
)



set(SOURCES 
    main.cpp 
)

set(LIBRARIES
    )

generate_arduino_firmware(window-temp-ctrl
    SRCS ${SOURCES}
    LIBS ${LIBRARIES}
    BOARD pro5v328
    #    ARDLIBS "SPI"
    )

