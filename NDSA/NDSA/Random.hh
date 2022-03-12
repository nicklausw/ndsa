namespace NDSA {
  extern struct RandomObj {
    void Seed() {
      srand(time(0));
    }
    
    // C style randomness.
    long Next() {
      return rand();
    }
  } Random;
}
