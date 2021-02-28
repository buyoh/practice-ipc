#include <stdexcept>
#include <sys/ipc.h>

#include "./key.hpp"

int getIPCkey(const std::string& filepath) {
  key_t key = ftok(filepath.c_str(), 'r');
  if (key < 0)
    throw std::runtime_error("ftok failed");
  return key;
}
