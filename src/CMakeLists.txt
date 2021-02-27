add_library(ipc STATIC 
  shm/shm.cpp
)
add_executable(01_shared_memory ./sample/01_shared_memory.cpp)
target_link_libraries(01_shared_memory ipc)
