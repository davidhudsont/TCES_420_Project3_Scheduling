
add_executable(Scheduling job.cpp node.cpp queue.cpp scheduler_threads scheduler.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include)