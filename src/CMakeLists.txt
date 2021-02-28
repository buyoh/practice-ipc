add_library(ipc STATIC 
  shm/shm.cpp
  key/key.cpp
  semaphore/semaphore.cpp
)
add_executable(01_shared_memory ./sample/01_shared_memory.cpp)
target_link_libraries(01_shared_memory ipc)
add_executable(06_semaphore ./sample/06_semaphore.cpp)
target_link_libraries(06_semaphore ipc)
