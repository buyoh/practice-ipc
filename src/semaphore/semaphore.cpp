#include <stdexcept>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "./semaphore.hpp"

// note: SEM_UNDO is not implemented.

namespace {

int operate(int semid, unsigned short index, short op, short flg) {
  sembuf sops[1] = {
      index,
      op,
      flg,
  };
  return semop(semid, sops, 1);
}

}  // namespace

// static
Semaphore Semaphore::create(key_t key, unsigned short num) {
  if (key < 0)
    key = IPC_PRIVATE;
  int semid = semget(key, num, 0666 | IPC_CREAT);
  if (semid < 0)
    throw std::runtime_error("semget failed");
  return Semaphore(semid);
}

// static
Semaphore Semaphore::open(key_t key, unsigned short num) {
  int semid = semget(key, num, 0666);
  if (semid < 0)
    throw std::runtime_error("semget failed");
  return Semaphore(semid);
}

Semaphore::~Semaphore() {}

void Semaphore::up(unsigned short index, unsigned int v) {
  operate(semid_, index, static_cast<short>(v), 0);
}

void Semaphore::down(unsigned short index, unsigned int v) {
  operate(semid_, index, -static_cast<short>(v), 0);
}

bool Semaphore::downNoblock(unsigned short index, unsigned int v) {
  if (operate(semid_, index, -static_cast<short>(v), IPC_NOWAIT) < 0) {
    if (errno == EAGAIN)
      return false;
  }
  return true;
}

void Semaphore::zero(unsigned short index) {
  operate(semid_, index, 0, 0);
}

bool Semaphore::zeroNoblock(unsigned short index) {
  if (operate(semid_, index, 0, IPC_NOWAIT) < 0) {
    if (errno == EAGAIN)
      return false;
  }
  return true;
}

void Semaphore::destroy() {
  if (semctl(semid_, 0, IPC_RMID, 0) < 0) {
    // nop: error
    void(0);
  }
  semid_ = -1;
}

Semaphore::Semaphore(int semid) : semid_(semid) {}
