cmake_minimum_required(VERSION 4.4.0)
project(Ende)

set(CMAKE_CXX_STANDARD 26)

option(ENDE_BUILD_EXAMPLES "enable examples" ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

function(filter_items items regex)
    foreach(item ${${items}})
        if(${item} MATCHES ${regex})
            list(REMOVE_ITEM ${items} ${item})
        endif()
    endforeach()
    set(${items} ${${items}} PARENT_SCOPE)
endfunction()

set(PROJECT_FILES include/Ende/platform.h include/Ende/maybe.h include/Ende/profile.h)

if (UNIX)
    filter_items(PROJECT_FILES ".*win.*")
elseif(WIN32)
    filter_items(PROJECT_FILES ".*unix.*")
endif()

add_library(Ende ${PROJECT_FILES})
target_sources(Ende
PUBLIC FILE_SET CXX_MODULES FILES
  src/ende.cpp

  src/filesystem/filesystem.cpp
  src/filesystem/File.cpp
  src/filesystem/FileWatcher.cpp

  src/graph.cpp

  src/io.cpp

  src/math/math.cpp
  src/math/Frustum.cpp
  src/math/intersect.cpp
  src/math/Matrix.cpp
  src/math/Quaternion.cpp
  src/math/random.cpp
  src/math/utils.cpp

  src/os/os.cpp
  src/os/DynamicLib.cpp
  src/os/Process.cpp

  src/profile/profile.cpp

  src/sys/sys.cpp
  src/sys/dl.cpp
  src/sys/FileDesc.cpp
  src/sys/notify.cpp
  src/sys/pipe.cpp
  src/sys/process.cpp
  src/sys/Socket.cpp
  src/sys/system.cpp

  src/Singleton.cpp

  src/thread/thread.cpp
  src/thread/ThreadPool.cpp
  src/thread/Mutex.cpp
  src/thread/Semaphore.cpp
  src/thread/Queue.cpp

  src/time/time.cpp
  src/time/Clock.cpp
  src/time/StopWatch.cpp

  src/JobSystem.cpp

  src/util.cpp
)

target_include_directories(Ende
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        third_party)

if (${ENDE_BUILD_EXAMPLES})
    add_subdirectory(examples)
endif ()
add_subdirectory(third_party)

#option(ENDE_BUILD_TESTS "ENDE_BUILD_TESTS" ON)
#if (${ENDE_BUILD_TESTS})
    add_subdirectory(tests)
#endif ()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(spdlog REQUIRED)
target_link_libraries(Ende PRIVATE Threads::Threads spdlog::spdlog)

set(include_dest "include")
set(main_lib_dest "lib/Ende")

# install(TARGETS Ende EXPORT Ende DESTINATION "${main_lib_dest}")
# install(DIRECTORY include/Ende DESTINATION "${include_dest}")
# install(FILES cmake/EndeConfig.cmake DESTINATION "${main_lib_dest}")
# install(EXPORT Ende DESTINATION "${main_lib_dest}")
