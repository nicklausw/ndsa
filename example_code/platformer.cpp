#include <NDSA.hh>
using namespace NDSA;

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
    Move(Right, 10);
    if(X >= SCREEN_WIDTH - xDeadSpace() - width) {
      delete this;
    }
  }
};

struct Enemy : Object {
  using ParentConstructors;
  void Step() override {
    Move(Left, .5);
    if(X - xDeadSpace() <= 0.0F) {
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
      Bullet *b = new Bullet(bulletSpritePointer, round(X + 8), round(Y)
                             #ifdef DS
                             , TopScreen
                             #endif
                             );
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

includeTiles(bgTileSet);
includeTiles(spriteSheet);
includeMap(tileMap);

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
  SpriteSheet *spriteSheet = new SpriteSheet(SpriteData(spriteSheet), Size_32x32, Colors256);
  Sprite *manSprite = spriteSheet->getByIndex(3);
  Sprite *enemySprite = spriteSheet->getByIndex(1);
  Sprite *bulletSprite = spriteSheet->getByIndex(2);

  Background.Set(BackgroundData(bgTileSet), MapData(tileMap), BGSize_512x512
                 #ifdef DS
                 , TopScreen
                 #endif
                 );

  Player *player = new Player(manSprite, 50, 50
                              #ifdef DS
                              , TopScreen
                              #endif
                              );
  player->bulletSpritePointer = bulletSprite;
  player->addCollision(32, 32);

  onCollision<Enemy, Bullet>(onEnemyBulletCollide);
  onCollision<Player, Enemy>(onPlayerEnemyCollide);
  
  #ifdef DS
  initConsole();
  #endif
  
  Background.setScrollY(512 - SCREEN_HEIGHT);
  #ifdef DS
	bgUpdate();
  #endif

  int frame=0;
  while(System.Frame()) {
    frame++;
    #ifdef DS
    PrintAt(0,5,"Player health: %d    ",player->health);
    #endif
    if(frame == 250) {
      Enemy *e = new Enemy(enemySprite, SCREEN_WIDTH, SCREEN_HEIGHT - 64
                           #ifdef DS
                           , TopScreen
                           #endif
                           );
      e->addCollision(24, 32);
      frame = 0;
    }
    Object *o = player;
    const MapData* tileMapData = tileMapMap;
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
    #ifdef DS
    PrintAt(0,2,"# of objects: %d    ",Objects.count);
    #endif
  }
}