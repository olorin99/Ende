
add_executable(notify notify.cpp)
target_link_libraries(notify Ende)

add_executable(filesystem filesystem.cpp)
target_link_libraries(filesystem Ende)

add_executable(threading threading.cpp)
target_link_libraries(threading Ende)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -DENDE_PROFILE)
add_executable(profile_test profile_test.cpp)
target_link_libraries(profile_test Ende)

add_executable(math math.cpp)
target_link_libraries(math Ende)