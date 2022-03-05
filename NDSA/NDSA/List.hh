// The PointerList does exactly what its name implies.
// Anything you can point to, you can make a dynamic list of.

namespace NDSA {
  template<typename T>
  class PointerList {
    T **data;

    public:
    int listSize;
    int count;

    PointerList() {
      listSize = 2;
      data = (T**)calloc(listSize * sizeof(T*), 1);
      count = 0;
    }

    ~PointerList() {
      free(data);
    }

    inline bool found(T *o) {
      for(int c = 0; c < count; c++) {
        if(data[c] == o) return true;
      }
      return false;
    }

    inline void add(T *o) {
      if(count + 1 == listSize) {
        listSize *= 2;
        data = (T**)realloc(data, listSize * sizeof(T*));
      }
      data[count] = o;
      count++;
    }

    inline T *getByIndex(int i) {
      return data[i];
    }

    inline void setByIndex(int i, T* t) {
      data[i] = t;
    }

    inline void deleteAll() {
      int d = count;
      for(int c = 0; c < d; c++) {
        delete getByIndex(0);
        removeByIndex(0);
      }
    }

    inline void removeByIndex(int i) {
      data[i] = 0;
      for(int c = i; c < count - 1; c++) {
        data[c] = data[c + 1];
      }
      data[count - 1] = 0;
      count--;
      if(count < listSize / 2 && count > 0) {
        listSize /= 2;
        data = (T**)realloc(data, listSize * sizeof(T*));
      }
    }

    inline void removeByInstance(T *o) {
      for(int c = 0; c < count; c++) {
        if(getByIndex(c) == o) {
          removeByIndex(c);
          return;
        }
      }
    }
  
    template <typename Tcasted>
    inline void deleteByInstance(Tcasted**o) {
        for(int c = 0; c < count; c++) {
          if(getByIndex(c) == *o) {
            delete getByIndex(c);
            *o = 0;
            return;
          }
        }
      }
  
    template<typename Tcasted>
    inline void deleteByType() {
      for(int c = 0; c < count; c++) {
        if(dynamic_cast<Tcasted>(getByIndex(c))) {
          delete getByIndex(c);
        c--;
        }
      }
    }
  
    template<typename Tcasted>
    inline PointerList<Tcasted> getByType() {
      PointerList<Tcasted> list;
      for(int c = 0; c < count; c++) {
        Tcasted *t = dynamic_cast<Tcasted*>(getByIndex(c));
        if(t) {
          list.add(t);
        }
      }
      return list;
    }
  };
}