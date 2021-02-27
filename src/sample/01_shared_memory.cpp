#include <iostream>
#include <optional>

#include <unistd.h>

#include "shm/shm.hpp"

template <typename T>
T&& unwrap(std::optional<T>&& o) {
  if (!o) {
    fprintf(stderr, "unwrap failed: %s\n", typeid(T).name());
    exit(1);
  }
  return std::move(*o);
}

int main() {
  const int N = 8;
  auto shared = unwrap(SharedMemory::create(-1, sizeof(int) * N));

  if (fork()) {
    // parent-process
    int* ptr = reinterpret_cast<int*>(shared.get());
    for (int i = 0; i < N; ++i) {
      ptr[i] = (i + 1) * (i + 1);
    }
    sleep(1);
    sleep(1);
    shared.destroy();
    return 0;
  } else {
    // child-process
    int* ptr = reinterpret_cast<int*>(shared.get());
    sleep(1);
    for (int i = 0; i < N; ++i) {
      std::cout << ptr[i] << ' ';
    }
    std::cout << std::endl;
    sleep(1);
    return 0;
  }
}