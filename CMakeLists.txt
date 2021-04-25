cmake_minimum_required(VERSION 3.3)

#빌드 커맨드를 화면에 출력
set (CMAKE_VERBOSE_MAKEFILE ON)
set(PROJECT_NAME "vmq_test_tool")

#set( REDIST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../redist)

# 프로젝트 이름
project(${PROJECT_NAME})

set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})


set( OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/output)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -g -O3")

aux_source_directory(./vmq_task SOURCE)
list (APPEND FULL_SOURCE ${SOURCE})
message( STATUS "source list ${FULL_SOURCE}")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(APPLICATION_NAME ${PROJECT_NAME})
add_executable(${APPLICATION_NAME} ${FULL_SOURCE})
