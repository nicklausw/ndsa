#pragma once

#define SCREEN(x,y) "\x1b["#y";"#x"H"
#define PrintAt(x,y,str,...) printf(SCREEN(x,y) str, ##__VA_ARGS__)

#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstdarg>
#include <cstring>

#include <typeinfo>

#include <NDSA/List.hh>
#include <NDSA/Collision.hh>

#ifdef NDSA_AUDIO
  #include <maxmod9.h>
  #include "mmsolution.h" // solution definitions
  #include "mmsolution_bin.h" // solution binary reference
  #include <NDSA/Audio.hh>
#endif

// these names are too long...
typedef const unsigned char TileData;
typedef const unsigned short MapData;
typedef const unsigned short PaletteData;

#define includeTilemap(x) \
  extern const short unsigned int x ## Data[]; \
  extern const unsigned long x ## DataLength;

#define includeGraphics(x) \
  extern const unsigned int   x ## TilesLen; \
  extern const unsigned int   x ## MapLen; \
  extern const unsigned int   x ## PalLen; \
  extern const unsigned char  x ## Tiles[]; \
  extern const unsigned short  x ## Map[]; \
  extern const unsigned short  x ## Pal[];

namespace NDSA {
  inline void Fatal(const char*,...);
}

#ifdef DS
#include <nds.h>
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 192
#endif

#ifdef GBA
#include <gba.h>
#include <NDSA/GBA.hh>
#endif

namespace NDSA {
  inline void initConsole() {
    consoleDemoInit();
    #ifdef GBA
    // black background
    BG_PALETTE[0] = 0;
    BGCTRL[0] = BG_16_COLOR | SCREEN_BASE(4);
    BG_OFFSET[0].x = 0; BG_OFFSET[0].y = 0;
    #endif
  }
  
  inline void Fatal(const char *s,...) {
    initConsole();
    va_list argptr;
    va_start(argptr,s);
    fprintf(stdout, SCREEN(0,9) "ERROR: ");
    vfprintf(stdout, s, argptr);
    va_end(argptr);
    while(1);
  }

  class Object;
  extern PointerList<Object> Objects;
  extern PointerList<Object> Colliders;

  template<typename P, typename T>
  inline void onCollision(void function(P*, T*)) {
    P *p = new P();
    T *t = new T();
    CollisionEvent *c = new CollisionEvent(typeid(*p).name(), typeid(*t).name(), (void*)function);
    CollisionEvents.add(c);
    Objects.deleteByInstance<P>(&p);
    Objects.deleteByInstance<T>(&t);
  }
}

#ifdef DS
namespace NDSA {
  enum NDSA_Screen {
    TopScreen, BottomScreen
  };
}
#endif

#include <NDSA/Input.hh>
#include <NDSA/Random.hh>
#include <NDSA/Background.hh>
#include <NDSA/Sprite.hh>
#include <NDSA/Object.hh>

namespace NDSA {
  extern void Game();
  
  struct {
    void Initialize() {
      #ifdef DS
      videoSetMode(MODE_0_2D);
      videoSetModeSub(MODE_0_2D);
      
      vramSetBankA(VRAM_A_MAIN_BG);
      vramSetBankB(VRAM_B_MAIN_SPRITE);
      vramSetBankD(VRAM_D_SUB_SPRITE);
      
      oamInit(&oamMain, SpriteMapping_1D_128, false);
      oamInit(&oamSub, SpriteMapping_1D_128, false);
      
      #ifdef NDSA_AUDIO
        mmInitDefaultMem( (mm_addr)mmsolution_bin );
      #endif
      #endif

      #ifdef GBA
      irqInit();
      irqEnable(IRQ_VBLANK);
      REG_IME = 1; // interrupts on
      SetMode( OBJ_ON | BG0_ON );
      #endif
      
      Random.Seed();
    }
    
    bool Frame() {
      #ifdef DS
      swiWaitForVBlank();
      
      TouchScreen.Update();
      
      oamUpdate(&oamMain);
      oamUpdate(&oamSub);
      #endif

      #ifdef GBA
      setRepeat(1, 1);
      VBlankIntrWait();
      #endif

      scanKeys();
      Keys.keysDown = keysDown();
      Keys.keysHeld = keysHeld();

      // run through object code. the list can change as we go along.
      // so we only run the current set of objects from the start of frame.
      PointerList<Object> currentObjects = Objects.getCopy();
      for(int c = 0; c < currentObjects.count; c++) {
        Object *o = currentObjects.getByIndex(c);
        if(Objects.found(o)) { // if it's still on the list
          o->Step();
        }
      }
      
      for(int c = 0; c < Colliders.count; c++) {
        for(int d = c + 1; d < Colliders.count; d++) {
          Object *o = Colliders.getByIndex(c);
          Object *p = Colliders.getByIndex(d);

          float ox1 = o->X + ((o->xSize - o->width) / 2), ox2 = ox1 + o->width;
          float px1 = p->X + ((p->xSize - p->width) / 2), px2 = px1 + p->width;
          float oy1 = o->Y + ((o->ySize - o->height) / 2), oy2 = oy1 + o->height;
          float py1 = p->Y + ((p->ySize - p->height) / 2), py2 = py1 + p->height;

          if (ox1 <= px2 && ox2 >= px1 &&
            oy1 <= py2 && oy2 >= py1) 
           {
            void (*function)(void*, void*) = (void(*)(void*,void*))CollisionEvents.getFunction(typeid(*o).name(), typeid(*p).name());
            if(function) {
              function(o, p);
            } else {
              function = (void(*)(void*,void*))CollisionEvents.getFunction(typeid(*p).name(), typeid(*o).name());
              if(function) {
                function(p, o);
              }
            }
          }
        }
      }

      
      return true;
    }
  } System;
}
