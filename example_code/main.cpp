#include <NDSA.hh>
using namespace NDSA;

#include "testImage.h"
#include "man.h"

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

struct Player : Object {
  using ParentConstructors;
  void Run() override {
    if(Buttons.Up.Held()) {
      Move(Up, .1);
    } else if(Buttons.Down.Held()) {
      Move(Down, .1);
    }
    if(Buttons.Left.Held()) {
      Move(Left, .1);
    } else if(Buttons.Right.Held()) {
      Move(Right, .1);
    }
  }
};

#include <NDSA/Main.hh>
void NDSA::Game() {
  Custom *custom = new Custom();
  Sprite *manSprite = new Sprite(SpriteData(man), SpriteSize_32x32, SpriteColorFormat_256Color);
  Player *player = new Player(manSprite, 150, 50, TopScreen);

  consoleDemoInit();
  Background.Set(BackgroundData(testImage), TopScreen);

  std::cout << SCREEN(0,5) << "NDSA is awesome!";
  std::cout << SCREEN(0,6) << "Move the guy with the D-pad.";

  while(DS.Frame());
}