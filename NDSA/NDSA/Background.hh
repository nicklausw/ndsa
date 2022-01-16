namespace NDSA {
  class {
    int BGData, BGDataSub;
    
    public:
    void Initialize() {
      BGData = bgInit(3, BgType_Bmp16, BgSize_B8_256x256, 0,0);
      BGDataSub = bgInitSub(3, BgType_Bmp16, BgSize_B8_256x256, 0,0);
    }
    
    void Set(TileData *Bitmap, PaletteData *Palette, int BitmapSize, int PalSize, NDSA_Screen BGScreen) {
      if (BGScreen == TopScreen) {
        dmaCopy(Bitmap, bgGetGfxPtr(BGData), BitmapSize);
        dmaCopy(Palette, BG_PALETTE, PalSize);
      } else {
        dmaCopy(Bitmap, bgGetGfxPtr(BGDataSub), BitmapSize);
        dmaCopy(Palette, BG_PALETTE_SUB, PalSize);
      }
    };
  } Background;
}
