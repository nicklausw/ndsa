#include <NDSA.hh>
using namespace NDSA;

#include "testImage.h"
#include "man.h"
#include "enemy.h"

#define SCREEN(x,y) "\x1b["#y";"#x"H"

struct Custom : Object {
  int frame;
  Custom() {
    frame = 0;
  }
  void Run() override {
    if(!TouchScreen.Held()) {
      frame++;
    } else {
      frame -= 2;
    }
    std::cout << SCREEN(0,10) << "Frame = " << frame;
  }
};

struct Player : Object {
  using ParentConstructors;
  ~Player() {
    std::cout << SCREEN(0,6) << "You killed him! Touch again to  "
                                "make another.";
  }
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

struct Enemy : Object {
  using ParentConstructors;
};

#include <NDSA/Main.hh>
void NDSA::Game() {
  Custom *custom = new Custom();
  Sprite *manSprite = new Sprite(SpriteData(man), SpriteSize_32x32, SpriteColorFormat_256Color);
  Sprite *enemySprite = new Sprite(SpriteData(enemy), SpriteSize_32x32, SpriteColorFormat_256Color);
  Player *player = new Player(manSprite, 150, 50, TopScreen);
  Enemy *enemy = new Enemy(enemySprite, 50, 50, TopScreen);


  consoleDemoInit();
  Background.Set(BackgroundData(testImage), TopScreen);

  std::cout << SCREEN(0,3) << "NDSA is awesome!";
  std::cout << SCREEN(0,4) << "Move the guy with the D-pad.";
  std::cout << SCREEN(0,6) << "Touch the screen to delete him.";

  bool deleted = false;
  while(DS.Frame()) {
    if(TouchScreen.Tapped()) {
      if(deleted == false) {
        deleted = true;
        delete player;
      } else {
        deleted = false;
        player = new Player(manSprite, 150, 50, TopScreen);
        std::cout << SCREEN(0,6) << "Hey, he's back!                 "
                                    "                                ";
      }
    }
  };
}