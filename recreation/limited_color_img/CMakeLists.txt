cmake_minimum_required(VERSION 2.8)
project("limited_color_img")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} )
include_directories(include)
add_executable(lci-encoder 
    src/encoder.cpp
    src/convert.cpp
    include/convert.h
    include/format.h
)
target_link_libraries(lci-encoder ${OpenCV_LIBS} )
add_executable(lci-decoder 
    src/decoder.cpp
    src/convert.cpp
    include/convert.h
    include/format.h
)
target_link_libraries(lci-decoder ${OpenCV_LIBS} )