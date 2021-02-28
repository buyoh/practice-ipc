#include <stdexcept>
#include "./shm.hpp"

#include <iostream>

// staic
SharedMemory SharedMemory::create(key_t key, size_t size) {
  if (key < 0)
    key = IPC_PRIVATE;
  int shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666);
  if (shmid == -1)
    throw std::runtime_error("shmget failed");
  return SharedMemory(shmid);
}

// staic
SharedMemory SharedMemory::open(key_t key, size_t size) {
  int shmid = shmget(key, size, 0666);
  if (shmid == -1)
    throw std::runtime_error("shmget failed");
  return SharedMemory(shmid);
}

void SharedMemory::destroy() {
  if (ptr_)
    shmdt(ptr_);
  if (shmid_ >= 0)
    shmctl(shmid_, IPC_RMID, 0);
  ptr_ = nullptr;
  shmid_ = -1;
}

SharedMemory::SharedMemory(int shmid) : shmid_(shmid) {
  ptr_ = shmat(shmid, nullptr, 0);
}

SharedMemory::~SharedMemory() {
  if (ptr_)
    shmdt(ptr_);
  ptr_ = nullptr;
}