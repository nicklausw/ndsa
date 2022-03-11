namespace NDSA {
  enum ColorFormat {
    Colors256,
    Colors16
    #ifdef DS
    , Bitmap
    #endif
  };

  enum SpriteDimensions {
    Size_8x8,
    Size_16x16,
    Size_32x32,
    Size_64x64,
    Size_16x8,
    Size_32x8,
    Size_32x16,
    Size_64x32,
    Size_8x16,
    Size_8x32,
    Size_16x32,
    Size_32x64
  };

  struct Sprite {
    #ifdef GBA
    int tileOffset = 0;
    int tileX = 0, tileY = 0;
    #endif

    #ifdef DS
    SpriteSize dsSprSize;
    SpriteColorFormat dsColorFormat;
    u16 *Graphics;
    u16 *GraphicsSub;
    #endif

    SpriteDimensions SprSize;
    ColorFormat colorFormat;
    
    #define SpriteData(x) x ## Tiles, x ## TilesLen, x ## Pal, x ## PalLen

    Sprite(TileData *Tiles, int TileLen, PaletteData *Palette, int PalLen, SpriteDimensions nSprSize, ColorFormat nColorFormat
     ) : SprSize(nSprSize) , colorFormat(nColorFormat) {

      
      
      // we have to allocate everything twice to maintain dual screen compatibility.

      #ifdef DS
      switch(SprSize) {
        case Size_16x16: dsSprSize = SpriteSize_16x16; break;
        case Size_16x32: dsSprSize = SpriteSize_16x32; break;
        case Size_16x8:  dsSprSize = SpriteSize_16x8;  break;
        case Size_32x16: dsSprSize = SpriteSize_32x16; break;
        case Size_32x32: dsSprSize = SpriteSize_32x32; break;
        case Size_32x64: dsSprSize = SpriteSize_32x64; break;
        case Size_32x8:  dsSprSize = SpriteSize_32x8;  break;
        case Size_64x32: dsSprSize = SpriteSize_64x32; break;
        case Size_64x64: dsSprSize = SpriteSize_64x64; break;
        case Size_8x16:  dsSprSize = SpriteSize_8x16;  break;
        case Size_8x32:  dsSprSize = SpriteSize_8x32;  break;
        case Size_8x8:   dsSprSize = SpriteSize_8x8;   break;
      }
      switch(colorFormat) {
        case Colors16:  dsColorFormat = SpriteColorFormat_16Color;  break;
        case Colors256: dsColorFormat = SpriteColorFormat_256Color; break;
        case Bitmap:    dsColorFormat = SpriteColorFormat_Bmp;      break;
      }
      
      Graphics = oamAllocateGfx(&oamMain, dsSprSize, dsColorFormat);
      GraphicsSub = oamAllocateGfx(&oamSub, dsSprSize, dsColorFormat);
      if(!Graphics || !GraphicsSub) {
        Fatal("Out of OAM memory.");
      }

      dmaCopy(Tiles, Graphics, TileLen);
      dmaCopy(Tiles, GraphicsSub, TileLen);
     
      if(Palette) {
        dmaCopy(Palette, SPRITE_PALETTE, PalLen);
        dmaCopy(Palette, SPRITE_PALETTE_SUB, PalLen);
      }
      #endif

      #ifdef GBA
      switch(nSprSize) {
        case Size_16x16: tileX = 2; tileY = 2; break;
        case Size_16x32: tileX = 2; tileY = 4; break;
        case Size_16x8:  tileX = 2; tileY = 1; break;
        case Size_32x16: tileX = 4; tileY = 2; break;
        case Size_32x32: tileX = 4; tileY = 4; break;
        case Size_32x64: tileX = 4; tileY = 8; break;
        case Size_32x8:  tileX = 4; tileY = 1; break;
        case Size_64x32: tileX = 8; tileY = 4; break;
        case Size_64x64: tileX = 8; tileY = 8; break;
        case Size_8x16:  tileX = 1; tileY = 2; break;
        case Size_8x32:  tileX = 1; tileY = 4; break;
        case Size_8x8:   tileX = 1; tileY = 1; break;
      }
      tileOffset = GBATiles.allocate(tileX, tileY, Tiles) * 2;
      
      if(Palette) {
        dmaCopy((void*)Palette, SPRITE_PALETTE, PalLen);
      }
      #endif
    }
    
    ~Sprite() {
      #ifdef DS
      oamFreeGfx(&oamMain, Graphics);
      oamFreeGfx(&oamSub, GraphicsSub);
      #endif
      #ifdef GBA
      GBATiles.free(tileOffset / 2, tileX, tileY);
      #endif
    }
  };
  
  extern class SpritesObj {
    bool Open_Sprites[128];
    
    public:
    void ObjUpdate(int ID, int X, int Y, Sprite *ObjSprite
    #ifdef DS
    , NDSA_Screen SprScreen
    #endif
    #ifdef GBA
    , OBJATTR *attr
    #endif
    ) {
      #ifdef DS
      OamState *ThisState;
      u16 *SpriteGraphics;
      
      if (SprScreen == TopScreen) {
        ThisState = &oamMain;
        SpriteGraphics = ObjSprite->Graphics;
      } else {
        ThisState = &oamSub;
        SpriteGraphics = ObjSprite->GraphicsSub;
      }
        
      oamSet(ThisState,  // main graphics engine context
        ID,              // oam index (0 to 127)  
        X,               // x and y pixel location of the sprite
        Y,    
        0,               // priority, lower renders last (on top)
        0,               // palette index if multiple palettes or the alpha value if bmp sprite 
        ObjSprite->dsSprSize,
        ObjSprite->dsColorFormat,
        SpriteGraphics,  // pointer to the loaded graphics
        -1,              // sprite rotation data  
        false,           // double the size when rotating?
        false,           // hide the sprite?
        false, false,    // vflip, hflip
        false            // apply mosaic
        );
        #endif

        #ifdef GBA
        u16 size = 0, shape = 0;
        switch(ObjSprite->SprSize) {
          case Size_8x8:   size = 0;  shape = 0;  break;
          case Size_16x8:  size = 0;  shape = 1;  break;
          case Size_8x16:  size = 0;  shape = 10; break;
          case Size_16x16: size = 1;  shape = 0;  break;
          case Size_32x8:  size = 1;  shape = 1;  break;
          case Size_8x32:  size = 1;  shape = 10; break;
          case Size_32x32: size = 10; shape = 0;  break;
          case Size_32x16: size = 10; shape = 1;  break;
          case Size_16x32: size = 10; shape = 10; break;
          case Size_64x64: size = 11; shape = 0;  break;
          case Size_64x32: size = 11; shape = 1;  break;
          case Size_32x64: size = 11; shape = 10; break;
        }
        attr->attr0 = (shape << 14) | (1 << 13) | ((int)round(Y) & 0xFF);
        attr->attr1 = (size << 14) | ((int)round(X) & 0xFF);
        attr->attr2 = ObjSprite->tileOffset;
        #endif
    }
    
    void Empty_Slot(int ID) {
      // true to false, false to true!
      Open_Sprites[ID] ^= 1;
    }
    
    int Find_Slot() {
      int Number = 0;
      for (bool Slot : Open_Sprites) {
        if (Slot == false) {
          Open_Sprites[Number] = true;
          return Number;
        } else {
          Number++;
        }
      }
      
      Fatal("All sprite slots taken");
      return -1;
    }
  } Sprites;

  class SpriteSheet : public PointerList<Sprite> {
    public:
    SpriteSheet(TileData *Tiles, int TileLen, PaletteData *Palette, int PalLen,
     SpriteDimensions nSprSize, ColorFormat nColorFormat
     ) {
      // we have to allocate everything twice to maintain dual screen compatibility.
      PointerList<Sprite>();
      int dataSize = TileLen / 1024;
      for(int c = 0; c < dataSize; c++) {
        Sprite *s = new Sprite(Tiles, 1024, (c == dataSize - 1 ? Palette : 0), PalLen, nSprSize, nColorFormat);
        add(s);
        Tiles += 1024;
      }
    }
  };
}