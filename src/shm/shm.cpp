#include "./shm.hpp"

#include <iostream>

std::optional<SharedMemory> SharedMemory::create(const std::string& filepath, size_t size) {
  key_t key = ftok(filepath.c_str(), 'r');
  std::cout << "key=" << key << std::endl;
  if (key < 0)
    return std::optional<SharedMemory>();
  return SharedMemory::create(key, size);
}

std::optional<SharedMemory> SharedMemory::open(const std::string& filepath, size_t size) {
  key_t key = ftok(filepath.c_str(), 'r');
  if (key < 0)
    return std::optional<SharedMemory>();
  return SharedMemory::open(key, size);
}

std::optional<SharedMemory> SharedMemory::create(key_t key, size_t size) {
  if (key < 0)
    key = IPC_PRIVATE;
  int shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666);
  if (shmid == -1)
    return std::optional<SharedMemory>();
  return std::optional<SharedMemory>(shmid);
}

std::optional<SharedMemory> SharedMemory::open(key_t key, size_t size) {
  int shmid = shmget(key, size, 0666);
  if (shmid == -1)
    return std::optional<SharedMemory>();
  return std::optional<SharedMemory>(shmid);
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