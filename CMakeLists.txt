cmake_minimum_required(VERSION 3.1)
project( DisplayImage )
set (CMAKE_CXX_STANDARD 17)


find_package( OpenCV REQUIRED )
# find_package( LIBUV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} ${UV_INCLUDE_DIRS})

add_executable( main bkup/bkup.cpp )
target_link_libraries( main ${OpenCV_LIBS} )


add_executable( capture bkup/capture.cpp )
# target_compile_definitions( capture PUBLIC "-std=c++17" )
target_link_libraries( capture ${OpenCV_LIBS} yuv)

# add_executable( test src/test.cpp )
# # target_compile_definitions( capture PUBLIC "-std=c++17" )
# target_link_libraries( test ${OpenCV_LIBS})