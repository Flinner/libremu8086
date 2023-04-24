cmake_minimum_required(VERSION 3.15)
project(libremu8086 CXX)



add_executable(libremu8086 src/libremu8086.cpp src/main.cpp)



install(TARGETS libremu8086 DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
