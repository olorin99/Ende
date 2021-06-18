cmake_minimum_required(VERSION 3.19)
project(Ende)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

function(filter_items items regex)
    foreach(item ${${items}})
        if(${item} MATCHES ${regex})
            list(REMOVE_ITEM ${items} ${item})
        endif()
    endforeach()
    set(${items} ${${items}} PARENT_SCOPE)
endfunction()

set(PROJECT_FILES include/Ende/platform.h src/sys/unix/system.cpp include/Ende/sys/system.h src/sys/unix/time.cpp include/Ende/sys/time.h src/sys/unix/FileDesc.cpp include/Ende/sys/FileDesc.h include/Ende/Span.h src/sys/unix/filesystem.cpp include/Ende/sys/filesystem.h include/Ende/Optional.h include/Ende/Vector.h include/Ende/sys/memory.h include/Ende/profile/profile.h src/profile/ProfileManager.cpp include/Ende/profile/ProfileManager.h include/Ende/Singleton.h src/time/time.cpp include/Ende/time/time.h include/Ende/Pointer.h include/Ende/Shared.h include/Ende/Result.h include/Ende/util/traits.h src/sys/unix/pipe.cpp include/Ende/sys/pipe.h src/sys/unix/process.cpp include/Ende/sys/process.h src/sys/unix/dl.cpp include/Ende/sys/dl.h src/sys/unix/signal.cpp include/Ende/sys/signal.h src/sys/unix/Socket.cpp include/Ende/sys/Socket.h src/os/DynamicLib.cpp include/Ende/os/DynamicLib.h src/os/Process.cpp include/Ende/os/Process.h src/sys/unix/notify.cpp include/Ende/sys/notify.h src/filesystem/Path.cpp include/Ende/filesystem/Path.h src/util/string.cpp include/Ende/util/string.h src/thread/thread.cpp include/Ende/thread/thread.h src/filesystem/File.cpp include/Ende/filesystem/File.h include/Ende/math/Vec.h include/Ende/math/Mat.h include/Ende/math/Quaternion.h include/Ende/math/math.h)

if (UNIX)
    filter_items(PROJECT_FILES ".*win.*")
elseif(WIN32)
    filter_items(PROJECT_FILES ".*unix.*")
endif()

add_library(Ende ${PROJECT_FILES})

target_include_directories(Ende
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        third_party)

add_subdirectory(examples)
add_subdirectory(third_party)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Ende PRIVATE Threads::Threads)

set(include_dest "include")
set(main_lib_dest "lib/Ende")

install(TARGETS Ende EXPORT Ende DESTINATION "${main_lib_dest}")
install(DIRECTORY include/Ende DESTINATION "${include_dest}")
install(FILES cmake/EndeConfig.cmake DESTINATION "${main_lib_dest}")
install(EXPORT Ende DESTINATION "${main_lib_dest}")