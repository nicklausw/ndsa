namespace NDSA {
  struct CollisionEvent {
    const char* object1;
    const char* object2;
    void *function;
    CollisionEvent(const char *a, const char *b, void *c) {
      object1 = a;
      object2 = b;
      function = c;
    }
  };

  extern class CollisionEventList : public PointerList<CollisionEvent> {
    public:
    void *getFunction(const char *a, const char *b) {
      for(int c = 0; c < count; c++) {
        if(!strcmp(getByIndex(c)->object1, a) && !strcmp(getByIndex(c)->object2, b)) {
          return getByIndex(c)->function;
        }
      }
      return 0;
    }
  } CollisionEvents;
}