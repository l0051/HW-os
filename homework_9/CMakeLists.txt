cmake_minimum_required(VERSION 3.22)
project(homework_9)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS thread REQUIRED)

if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif()

add_executable(homework_9 main.cpp lib/http.h lib/http.cpp)

if(Boost_FOUND)

    target_link_libraries(homework_9 ${Boost_LIBRARIES} Boost::thread)

endif()