#pragma once

#include <nds.h>
#include <maxmod9.h>

#ifdef NDSA_AUDIO
  #include "mmsolution.h"    // solution definitions
  #include "mmsolution_bin.h"  // solution binary reference
#endif
 
#include <iostream>
#include <cstdlib>
#include <random>
#include <chrono>

// these names are too long...
typedef const unsigned int TileData;
typedef const unsigned short PaletteData;
  
#include <NDSA/Screen.hh>
#include <NDSA/Audio.hh>
#include <NDSA/Input.hh>
#include <NDSA/Random.hh>
#include <NDSA/Background.hh>
#include <NDSA/Sprite.hh>
#include <NDSA/Object.hh>

namespace NDSA {
  extern void Game();
  
  struct {
    void Initialize() {
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
      for (Object *ThisObject : Lists.Objects) {
        ThisObject->Run();
      }
      
      return true;
    }
  } DS;
}
