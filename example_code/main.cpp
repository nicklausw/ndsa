#include <NDSA.hh>
using namespace NDSA;

#include "testImage.h"

#define SCREEN(x,y) "\x1b["#y";"#x"H"

struct Custom : Object {
  int frame;
  Custom() {
    frame = 0;
  }
  void Run() override {
    if(!TouchScreen.Pressed()) {
      frame++;
    } else {
      frame -= 2;
    }
    std::cout << SCREEN(0,10) << "Frame = " << frame;
  }
};

#include <NDSA/Main.hh>
void NDSA::Game() {
  Custom *custom = new Custom();

  consoleDemoInit();
  Background.Set(testImageBitmap, testImagePal, TopScreen);

  std::cout << SCREEN(0,5) << "NDSA is awesome!";

  while(DS.Frame());
}