PROJECT(Voxophone)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

FILE(GLOB MAIN_SRC "src/*.cpp")

ADD_EXECUTABLE(Voxophone ${MAIN_SRC})
TARGET_LINK_LIBRARIES(Voxophone be root tracker media network netapi)
