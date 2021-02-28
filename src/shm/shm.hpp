#include <string>
#include <sys/ipc.h>
#include <sys/shm.h>

class SharedMemory {
 public:
  static SharedMemory create(key_t key, size_t size);
  static SharedMemory open(key_t key, size_t size);

  ~SharedMemory();
  SharedMemory& operator=(SharedMemory&& sm) {
    shmid_ = sm.shmid_;
    ptr_ = sm.ptr_;
    sm.shmid_ = -1;
    sm.ptr_ = nullptr;
    return *this;
  }
  SharedMemory(SharedMemory&& sm) { *this = std::move(sm); }
  SharedMemory(const SharedMemory&) = delete;
  SharedMemory& operator=(const SharedMemory&) = delete;

  void* get() { return ptr_; }
  void destroy();

  SharedMemory(int shmid);

 private:
  int shmid_;
  void* ptr_;
};
