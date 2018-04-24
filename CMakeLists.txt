cmake_minimum_required(VERSION 2.8)

project(trader_rating)

include(CheckCXXCompilerFlag)

set(CXX "g++" CACHE STRING "CXX compiler, g++ by default")
set(CMAKE_CXX_COMPILER ${CXX})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
SET(CXX_LANGUAGE_FLAGS "-std=c++1y-stdlib=libc++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -Wno-extra -pipe -DREVISION=${REVISION} ")

set(LINKFLAGS ${LINKFLAGS} ${LDFLAGS})
set(LINKFLAGS ${LINKFLAGS} -g -Wl,--eh-frame-hdr)

option(DEBUG "DEBUG" OFF)
option(OPTIMIZE "OPTIMIZE" OFF)

option(ASAN "Address sanitizer" OFF)
option(TSAN "Thread sanitizer" OFF)
option(UBSAN "Undefinded behavior sanitizer" OFF)

include_directories(src ~/usr/include)
link_directories(~/usr/lib)
set(LINKFLAGS ${LINKFLAGS} -Wl,--eh-frame-hdr)

set(STARTSTATIC "-Wl,-Bstatic")
set(ENDSTATIC "-Wl,-Bdynamic")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_PARALLEL -fopenmp")

if (DEBUG)
    set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -ggdb -g3")
endif ()

if (OPTIMIZE)
    set(LINKFLAGS ${LINKFLAGS} -g)
    set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -O3")
endif()

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBRARIES OFF)
set(CMAKE_EXE_LINKER_FLAGS " ${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++ -Wl,--no-as-needed -lpthread")

file(GLOB HEADERS "src/*.h")
file(GLOB SRC     "src/*.cpp")

set(LIBS ${LIBS} zmq protobuf)

if (ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address ")
    set(LINKFLAGS ${LINKFLAGS} -fsanitize=address)
    set(LIBS ${LIBS} asan pthread )
endif()

if (TSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread ")
    set(LINKFLAGS ${LINKFLAGS} -fsanitize=thread)
    set(LIBS ${LIBS} tsan pthread )
endif()

if (UBSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined ")
    set(LINKFLAGS ${LINKFLAGS} -fsanitize=undefined)
    set(LIBS ${LIBS} ubsan pthread )
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed -pthread")
set(LINKFLAGS ${LINKFLAGS} -Wl,--no-as-needed -lpthread)

add_executable(${PROJECT_NAME} ${SRC} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${LIBS} )


