cmake_minimum_required(VERSION 3.22.1)

project("jila-android" LANGUAGES CXX)

# Add the library
add_library(jila-android STATIC
    jila-android.cpp
)

include_directories(jila-android
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
