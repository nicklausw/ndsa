namespace NDSA {
  extern class BGObj {
    int BGData, BGDataSub;
    
    public:
    void Initialize() {
      BGData = bgInit(3, BgType_Bmp16, BgSize_B8_256x256, 0,0);
      BGDataSub = bgInitSub(3, BgType_Bmp16, BgSize_B8_256x256, 0,0);
    }
    
    #define BackgroundData(x) x ## Bitmap, x ## Pal
    
    void Set(TileData *Bitmap, PaletteData *Palette, NDSA_Screen BGScreen) {
      if (BGScreen == TopScreen) {
        dmaCopy(Bitmap, bgGetGfxPtr(BGData), 256*256);
        dmaCopy(Palette, BG_PALETTE, 256*2);
      } else {
        dmaCopy(Bitmap, bgGetGfxPtr(BGDataSub), 256*256);
        dmaCopy(Palette, BG_PALETTE_SUB, 256*2);
      }
    };
  } Background;
}
