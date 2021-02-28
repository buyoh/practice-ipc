#include <iostream>
#include <vector>

#include <unistd.h>
#include <sys/wait.h>

#include "semaphore/semaphore.hpp"

#define ENABLE_SEMAPHORE 1

int main() {
  const int N = 4;
  const int K = 4;

#if ENABLE_SEMAPHORE
  auto sem = Semaphore::create(-1, 1);
  std::cout << "semaphore enabled" << std::endl;
#else
  std::cout << "semaphore disabled" << std::endl;
#endif

  std::vector<int> pids;
  pids.reserve(N);
  for (int pi = 0; pi < N; ++pi) {
    int pid;
    if (!(pid = fork())) {
      // child
      for (int i = 0; i < K; ++i) {
#if ENABLE_SEMAPHORE
        sem.down(0);
#endif
        // Only one process can sleep if ENABLE_SEMAPHORE is enabled.
        sleep(1);
#if ENABLE_SEMAPHORE
        sem.up(0);
#endif
        std::cout << pi << ": done " << i << std::endl;
      }
      exit(0);
    } else {
      // parent
      pids.push_back(pid);
    }
  }

#if ENABLE_SEMAPHORE
  sem.up(0);
#endif
  for (auto pid : pids) {
    waitpid(pid, nullptr, 0);
  }
  std::cout << std::endl;

#if ENABLE_SEMAPHORE
  sem.destroy();
#endif
  return 0;
}