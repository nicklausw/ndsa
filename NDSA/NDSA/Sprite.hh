namespace NDSA {
  struct Sprite {
    u16 *Graphics;
    u16 *GraphicsSub;
    SpriteSize SprSize;
    SpriteColorFormat ColorFormat;
    
    #define SpriteData(x) x ## Tiles, x ## TilesLen, x ## Pal, x ## PalLen

    Sprite(TileData *Tiles, int TileLen, PaletteData *Palette, int PalLen,
     SpriteSize nSprSize, SpriteColorFormat nColorFormat) : SprSize(nSprSize), ColorFormat(nColorFormat) {
      
      // we have to allocate everything twice to maintain dual screen compatibility.
      
      Graphics = oamAllocateGfx(&oamMain, SprSize, ColorFormat);
      GraphicsSub = oamAllocateGfx(&oamSub, SprSize, ColorFormat);
      if(!Graphics || !GraphicsSub) {
        Fatal("Out of OAM memory.");
      }

      dmaCopy(Tiles, Graphics, TileLen);
      dmaCopy(Tiles, GraphicsSub, TileLen);
     
      if(Palette) {
        dmaCopy(Palette, SPRITE_PALETTE, PalLen);
        dmaCopy(Palette, SPRITE_PALETTE_SUB, PalLen);
      }
    }
    
    ~Sprite() {
      oamFreeGfx(&oamMain, Graphics);
      oamFreeGfx(&oamSub, GraphicsSub);
    }
  };
  
  extern class SpritesObj {
    bool Open_Sprites[SPRITE_COUNT];
    
    public:   
    void ObjUpdate(int ID, int X, int Y, Sprite *ObjSprite, NDSA_Screen SprScreen) {
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
        ObjSprite->SprSize,
        ObjSprite->ColorFormat,
        SpriteGraphics,  // pointer to the loaded graphics
        -1,              // sprite rotation data  
        false,           // double the size when rotating?
        false,           // hide the sprite?
        false, false,    // vflip, hflip
        false            // apply mosaic
        );
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

  class SpriteSet : public PointerList<Sprite> {
    public:
    SpriteSet(TileData *Tiles, int TileLen, PaletteData *Palette, int PalLen,
     SpriteSize nSprSize, SpriteColorFormat nColorFormat) {
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