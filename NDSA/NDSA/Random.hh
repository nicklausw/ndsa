// time isn't that complicated, C++...c'mon.
#define CURRENT_TIME_SECS (std::chrono::high_resolution_clock::now().time_since_epoch().count())

namespace NDSA {
  class {
    std::mt19937 Mersenne;
    bool Seeded = false;
    
    public:
    void Seed() {
      Mersenne.seed(CURRENT_TIME_SECS);
      srand(CURRENT_TIME_SECS);
      Seeded = true;
    }
    
    // mersenne twister (recommended).
    long MT() {
      return Mersenne();
    }
    
    // C style randomness.
    long Classic() {
      return rand();
    }
  } Random;
}
