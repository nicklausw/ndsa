#include <NDSA.hh>
using namespace NDSA;

int objectCount() {
  int objectCount = 0;
  for(int c = 0; c < MAX_OBJECTS; c++) {
    if(Lists.Objects[c]) {
      objectCount++;
    }
  }
  return objectCount;
}

void printFrame(int frame) {
  PrintAt(0, 10, "Frame = %d", frame);
}