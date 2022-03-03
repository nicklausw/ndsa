namespace NDSA {
  extern class RandomObj {
    bool Seeded = false;
    
    public:
    void Seed() {
      srand(time(0));
      Seeded = true;
    }
    
    // C style randomness.
    long Next() {
      return rand();
    }
  } Random;
}
