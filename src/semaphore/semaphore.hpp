
class Semaphore {
 public:
  static Semaphore create(key_t key, unsigned short num);
  static Semaphore open(key_t key, unsigned short num);
  ~Semaphore();

  void up(unsigned short index, unsigned int v);
  void up(unsigned short index) { up(index, 1); }
  void down(unsigned short index, unsigned int v);
  void down(unsigned short index) { down(index, 1); };
  bool downNoblock(unsigned short index, unsigned int v);
  bool downNoblock(unsigned short index) { return downNoblock(index, 1); };
  void zero(unsigned short index);
  bool zeroNoblock(unsigned short index);

  void destroy();

 private:
  Semaphore(int semid);
  int semid_;
};
