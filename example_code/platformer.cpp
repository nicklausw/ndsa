#include <NDSA.hh>
using namespace NDSA;

#include "tileMap.h"

enum Status {
  idle,
  falling,
  jumping
};

double movement(double velocity, double acceleration, int step) {
  return ((velocity * step) + ((1.0/2.0) * (acceleration) * pow(step,2.0))) * .001;
}

double movementROC(double velocity, double acceleration, int step) {
  return (velocity + (2.0 * acceleration * step)) * .001;
}

struct Bullet : Object {
  using ParentConstructors;
  void Step() override {
    Move(Right, 5);
    if(X >= 256.0F - 32) {
      delete this;
    }
  }
};

struct Enemy : Object {
  using ParentConstructors;
  void Step() override {
    Move(Left, .5);
    if(X <= 0.0F) {
      delete this;
    }
  }
};

struct Player : Object {
  using ParentConstructors;

  Status yStatus = falling;

  int health = 10;
  int invincFrame = 0;

  float yROC = 0; // y Rate of Change
  int yStep = 0;
  int xStep = 0;
  const float xVelocity = 100;
  const float yVelocity = 1.0;
  const float xAccel = .1;
  const float xMaxStep = 200;
  float yBase = 0;

  Sprite *bulletSpritePointer;

  void Step() override {
    if(invincFrame) invincFrame--;
    if(Buttons.A.Pressed() && yStatus == idle) {
      yStatus = jumping;
      yROC = 1;
      yBase = Y;
      yStep = 0;
    }
    if(Buttons.B.Pressed()) {
      Bullet *b = new Bullet(bulletSpritePointer, round(X + 8), round(Y), TopScreen);
      b->addCollision(8, 8);
    }
    if(Buttons.Left.Held()) {
      if(xStep > -xMaxStep) xStep-=2;
      Move(Right, movement(xVelocity, (xStep > 0 ? xAccel : -xAccel), xStep));
    } else if(Buttons.Right.Held()) {
      if(xStep < xMaxStep) xStep+=2;
      Move(Right, movement(xVelocity, (xStep > 0 ? xAccel : -xAccel), xStep));
    } else if(xStep) {
      Move(Right, movement(xVelocity, (xStep > 0 ? xAccel : -xAccel), xStep));
      if(xStep > 0) xStep-=2; else xStep+=2;
    }
    if(yStatus == jumping) {
      Y = yBase - movement(yVelocity, -9.8, yStep);
      yStep+=2;
      if(Y > yBase) {
        Y = yBase;
        yStatus = idle;
        isFalling = false;
      }
      Update();
    }
    else if(yStatus == falling) {
      yROC = movement(yVelocity, -9.8, yStep);
      Move(Up, movement(yVelocity, -9.8, yStep));
      yStep++;
    }
  }
};

struct emptyObject : Object {
  using ParentConstructors;
};

#include "spriteSheet.h"
#include "bgTileSet.h"

void onEnemyBulletCollide(Enemy *p, Bullet *b) {
  Objects.deleteByInstance<Enemy>(&p);
  Objects.deleteByInstance<Bullet>(&b);
}

void onPlayerEnemyCollide(Player *p, Enemy *e) {
  if(!p->invincFrame) {
    p->health--;
    p->invincFrame = 500;
  }
}

#include <NDSA/Main.hh>
void NDSA::Game() {
  SpriteSet *spriteSheet = new SpriteSet(SpriteData(spriteSheet), SpriteSize_32x32, SpriteColorFormat_256Color);
  Sprite *manSprite = spriteSheet->getByIndex(3);
  Sprite *enemySprite = spriteSheet->getByIndex(1);
  Sprite *bulletSprite = spriteSheet->getByIndex(2);

  Background.Set(bgTileSetTiles, bgTileSetTilesLen, tileMapData, tileMapDataLength, bgTileSetPal, bgTileSetPalLen, BgSize_B8_512x512, TopScreen, true);

  Player *player = new Player(manSprite, 50, 50, TopScreen);
  player->bulletSpritePointer = bulletSprite;
  player->addCollision(32, 32);

  onCollision<Enemy, Bullet>(onEnemyBulletCollide);
  onCollision<Player, Enemy>(onPlayerEnemyCollide);

  consoleDemoInit();

  Background.setScrollY(512 - 192);
	bgUpdate();

  int frame=0;
  while(DS.Frame()) {
    frame++;
    PrintAt(0,5,"Player health: %d    ",player->health);
    if(frame == 250) {
      Enemy *e = new Enemy(enemySprite, 256 + 32, 192 - 64, TopScreen);
      e->addCollision(24, 32);
      frame = 0;
    }
    if(Buttons.X.Held()) {
      emptyObject *o = new emptyObject();
    }
    if(Buttons.B.Pressed()) {
      PointerList<emptyObject> l = Objects.getByType<emptyObject>();
      l.deleteAll();
    }
        Object *o = player;
        float collisionXtop = (o->X / 8.0) - (Background.getScrollX() / 8.0);
        float collisionYtop = (o->Y / 8.0) + (Background.getScrollY() / 8.0);
        float collisionXbottom = ((o->X + o->width) / 8.0) - (Background.getScrollX() / 8.0);
        float collisionYbottom = ((o->Y + o->height) / 8.0) + (Background.getScrollY() / 8.0);
        int collisionNumTop = tileMapData[((int)floor(collisionYtop) * (512 / 8)) + (int)collisionXtop];
        int collisionNumBottom = tileMapData[((int)floor(collisionYbottom) * (512 / 8)) + (int)collisionXbottom];
        if((collisionNumTop || collisionNumBottom) && player->yROC < 0) {
          player->Y = floor(player->Y / 8) * 8;
          player->Update();
          player->yStatus = idle;
        }
    PrintAt(0,2,"# of objects: %d    ",Objects.count);
  }
}