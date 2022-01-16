/* nicklausw DS Advance Engine!

More like c++ wrappers for making DS games. */

/* Be careful when including individual files from the library!
Some of the files rely on each other. Be wary of bug.
My recommendation: just include ndsa.hh and get it over with. */

/* changelog

6/25/16 - v1.0
    * the initial release.
    * button system, only pressed so far.
    * pretty frames (with vblank and buttons!).

6/26/16 - v1.1
    * gave NDSA_Keys a struct home for multi-module dev.
    * added button Held() function.
    * added clean touch screen support.

6/29/16 - v1.2
    * added random numbers, Mersenne Twister and C-style.
    * added not-quite-working sprites
    * added basically working objects?

7/3/16 - v1.3
    * added properly working sprites
    * fixed objects
    * added basic audio playback
    * DS.Initialize() is a thing, remove if possible

7/6/16 - v1.4
    * made audio optional
    * added backgrounds
    * added dual-screen support!
    * todo: documentation.....:(

*/

/* what does ndsa currently bring you?

well, first off, buttons are much simpler. Rather than
having to scanKeys() and AND keysDown() with KEY_A,
KEY_B, etc, you can just do this:
        if (Buttons.A.Pressed()) {
            // it's pressed!
        }
For touch screen usage, it's just as simple:
        if (TouchScreen.Pressed()) {
            // the stylus is in contact!
        }
Sweet, right? And for infinite vblank loops (which handle
button and touch screen input for you), just use:
        while (DS.Frame()) {
            // an infinite loop!
        }

Quick development means some of the changelog dates may be the
same or very close. I'm in a hurry to make NDSA awesome.

Planned features:

* music management?
* making existing libnds functionality look prettier. */

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
#include <list>

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
