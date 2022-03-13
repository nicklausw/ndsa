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
             BackgroundSize bgSize
             #ifdef DS
             , NDSA_Screen BGScreen = TopScreen
             #endif
             ) {

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

      dmaCopy(Map, bgGetMapPtr(pointer), MapLen);
      dmaCopy(Tiles, bgGetGfxPtr(pointer), TilesLen);
      dmaCopy(Palette, BG_PALETTE, PaletteLen);
      #endif

      #ifdef GBA
      dmaCopy((void*)Palette, BG_PALETTE, PaletteLen);
      dmaCopy((void*)Tiles, PATRAM8(0,0), TilesLen);
      BGCTRL[0] = SCREEN_BASE(30) | BG_256_COLOR | BG_SIZE_3;
      dmaCopy((void*)Map, MAP_BASE_ADR(30), MapLen);
      #endif
    };
  } Background;
}
