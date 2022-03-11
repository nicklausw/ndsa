namespace NDSA {
  enum BackgroundSize {
    BGSize_256x256,
    BGSize_256x512,
    BGSize_512x256,
    BGSize_512x512
  };

  extern class BGObj {
    int BGData = 0, BGDataSub = 0;
    int scrollX = 0, scrollY = 0;
    
    inline void updateScroll() {
      #ifdef DS
      bgSetScroll(0, scrollX, scrollY);
      #endif
      #ifdef GBA
      BG_OFFSET[0].x = scrollX; BG_OFFSET[0].y = scrollY;
      #endif
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
             BackgroundSize bgSize,
             #ifdef DS
             NDSA_Screen BGScreen,
             #endif
             bool unsortedMap = false) {
      unsigned short *newMap;   
      if(unsortedMap) {
        newMap = (unsigned short*)malloc(MapLen);
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
        }
      }

      #ifdef DS
      BgSize dsBgSize;
      switch(bgSize) {
        case BGSize_256x256: dsBgSize = BgSize_B8_256x256; break;
        case BGSize_256x512: dsBgSize = BgSize_T_256x512; break;
        case BGSize_512x256: dsBgSize = BgSize_B8_512x256; break;
        case BGSize_512x512: dsBgSize = BgSize_B8_512x512; break;
      }
      BGData = bgInit(0, BgType_Text8bpp, dsBgSize, 0,1);
      BGDataSub = bgInitSub(0, BgType_Text8bpp, dsBgSize, 0,1);
      int pointer = BGScreen == TopScreen ? BGData : BGDataSub;
      if(unsortedMap) {
        dmaCopy(newMap, bgGetMapPtr(pointer), MapLen);
      } else {
        dmaCopy(Map, bgGetMapPtr(pointer), MapLen);
      }
      dmaCopy(Tiles, bgGetGfxPtr(pointer), TilesLen);
      dmaCopy(Palette, BG_PALETTE, PaletteLen);
      #endif

      #ifdef GBA
      dmaCopy((void*)Palette, BG_PALETTE, PaletteLen);
      dmaCopy((void*)Tiles, PATRAM8(0,0), TilesLen);
      BGCTRL[0] = SCREEN_BASE(30) | BG_256_COLOR | BG_SIZE_3;
      if(unsortedMap) {
        dmaCopy((void*)newMap, MAP_BASE_ADR(30), MapLen);
      } else {
        dmaCopy((void*)Map, MAP_BASE_ADR(30), MapLen);
      }
      #endif
    };
  } Background;
}
