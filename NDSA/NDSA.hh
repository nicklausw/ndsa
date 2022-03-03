#pragma once

#define MAX_OBJECTS 128

#define SCREEN(x,y) "\x1b["#y";"#x"H"
#define PrintAt(x,y,str,...) printf(SCREEN(x,y) str, ##__VA_ARGS__)

#include <nds.h>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstdarg>

#ifdef NDSA_AUDIO
  #include <maxmod9.h>
  #include "mmsolution.h" // solution definitions
  #include "mmsolution_bin.h" // solution binary reference
  #include <NDSA/Audio.hh>
#endif

// these names are too long...
typedef const unsigned int TileData;
typedef const unsigned short PaletteData;

namespace NDSA {
  inline void Fatal(const char *s,...) {
    consoleDemoInit();
    va_list argptr;
    va_start(argptr,s);
    fprintf(stdout, SCREEN(0,10) "ERROR: ");
    vfprintf(stdout, s, argptr);
    va_end(argptr);
    while(1);
  }

  class Object;
  extern Object *Objects[MAX_OBJECTS];
}

#include <NDSA/Screen.hh>
#include <NDSA/Input.hh>
#include <NDSA/Random.hh>
#include <NDSA/Background.hh>
#include <NDSA/Sprite.hh>
#include <NDSA/Object.hh>

namespace NDSA {
  extern void Game();
  
  struct {
    void Initialize() {
      for(int c = 0; c < MAX_OBJECTS; c++) {
        Objects[c] = 0;
      }

      videoSetMode(MODE_5_2D);
      videoSetModeSub(MODE_5_2D);
      
      vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
      vramSetBankB(VRAM_B_MAIN_SPRITE);
      vramSetBankD(VRAM_D_SUB_SPRITE);
      
      oamInit(&oamMain, SpriteMapping_1D_32, false);
      oamInit(&oamSub, SpriteMapping_1D_32, false);
      
      #ifdef NDSA_AUDIO
        mmInitDefaultMem( (mm_addr)mmsolution_bin );
      #endif
      
      Background.Initialize();
      
      Random.Seed();
    }
    
    bool Frame() {
      swiWaitForVBlank();
      
      scanKeys();
      
      TouchScreen.Update();
      
      oamUpdate(&oamMain);
      oamUpdate(&oamSub);
      
      // run through object code
      for (unsigned int c = 0; c < MAX_OBJECTS; c++) {
        if(Objects[c]) {
          Objects[c]->Step();
        }
      }
      
      return true;
    }
  } DS;
}
