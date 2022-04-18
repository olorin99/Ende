
add_executable(result result.cpp)
target_link_libraries(result Ende)

add_executable(notify notify.cpp)
target_link_libraries(notify Ende)

add_executable(filesystem filesystem.cpp)
target_link_libraries(filesystem Ende)

add_executable(threading threading.cpp)
target_link_libraries(threading Ende)