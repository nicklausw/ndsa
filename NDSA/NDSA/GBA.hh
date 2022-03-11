// libgba doesn't do a whole lot.
// so this file does things instead.
namespace NDSA {
  extern class GBATilesObj {
    bool openTiles[16][32];
    public:
    GBATilesObj() {
      for(int c = 0; c < 16; c++) {
        for(int d = 0; d < 32; d++) {
          openTiles[c][d] = false;
        }
      }
    }
    
    bool squareGood(int x, int y, int xWidth, int yHeight) {
      for(int c = x; c < x + xWidth; c++) {
        if(c >= 16) return false;
        for(int d = y; d < y + yHeight; d++) {
          if(d >= 32) return false;
          if(openTiles[c][d] == true) return false;
        }
      }
      return true;
    }

    void fillSpaces(int x, int y, int xWidth, int yHeight, TileData *data) {
      for(int d = y; d < y + yHeight; d++) {
        for(int c = x; c < x + xWidth; c++) {
          openTiles[c][d] = true;
          dmaCopy((void*)data, PATRAM8(4,c + (d * 16)), 64);
          data += 64;
        }
      }
    }

    int allocate(int x, int y, TileData *data) {
      for(int c = 0; c < 16; c++) {
        for(int d = 0; d < 32; d++) {
          if(squareGood(c,d,x,y)) {
            fillSpaces(c,d,x,y,data);
            return c + (d * 16);
          }
        }
      }
      Fatal("Could not allocate tiles.");
      return -1;
    }

    void free(int tileOffset, int tileX, int tileY) {
      int x = tileOffset;
      int y = tileOffset;
      while(x >= 16) x -= 16;
      y /= 16;
      for(int c = x; c < x + tileX; c++) {
        for(int d = y; d < y + tileY; d++) {
          openTiles[c][d] = false;
        }
      }
    }
  } GBATiles;
}