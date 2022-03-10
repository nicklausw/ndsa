namespace NDSA {
  extern class BGObj {
    int BGData = 0, BGDataSub = 0;
    int scrollX = 0, scrollY = 0;
    
    inline void updateScroll() {
      bgSetScroll(0, scrollX, scrollY);
    }

    public:
    inline int getScrollX() { return scrollX; }
    inline int getScrollY() { return scrollY; }
    inline void setScrollX(int newX) {
      scrollX = newX;
      updateScroll();
    }
    inline void setScrollY(int newY) {
      scrollY = newY;
      updateScroll();
    }
    
    #define BackgroundData(x) x ## Tiles, x ## TilesLen, x ## Map, x ## MapLen, x ## Pal, x ## PalLen
    
    void Set(TileData *Tiles, int TilesLen, 
             MapData *Map, int MapLen,
             PaletteData *Palette, int PaletteLen,
             BgSize bgSize, NDSA_Screen BGScreen,
             bool unsortedMap = false) {
      BGData = bgInit(0, BgType_Text8bpp, bgSize, 0,1);
      BGDataSub = bgInitSub(0, BgType_Text8bpp, bgSize, 0,1);
      int pointer = BGScreen == TopScreen ? BGData : BGDataSub;
      if(unsortedMap) {
        unsigned short *newMap = (unsigned short*)malloc(MapLen);
        // right now we can only handle 512 x 512
        int counter = 0;
        int mapPoint = 0;
        for(int c = 0; c < 4; c++) {
          counter = 0;
          if(c == 1)
            counter = 32;
          else if(c == 2)
            counter = 64 * 32;
          else if(c == 3)
            counter = 64 * 32 + 32;
          for(int d = counter; d < counter + (32 * 64); d += 64) {
            for(int e = 0; e < 32; e++) {
              newMap[mapPoint] = Map[d + e];
              mapPoint++;
            }
          }
          dmaCopy(newMap, bgGetMapPtr(pointer), MapLen);
        }
      } else {
        dmaCopy(Map, bgGetMapPtr(pointer), MapLen);
      }
      dmaCopy(Tiles, bgGetGfxPtr(pointer), TilesLen);
      dmaCopy(Palette, BG_PALETTE, PaletteLen);
    };
  } Background;
}
