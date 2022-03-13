# NDSA - Nintendo DS Advanced Engine
## Write for NDS and GBA in one codebase!
A project from 2016 that I'd like to bring back. It's a header library which wraps libnds/libgba functions, adds an object system, so on. NOTE: Requires [gritn](https://github.com/nicklausw/gritn). Other than that, if you have devkitPro installed, you should be able to clone the repo, run GNU make and have fun.

## Objects
Declare an object like so:
```c++
struct Player : Object {
  using ParentConstructors;
  void Step() override {
    // code run once per frame
  }
};
```
Run it like so:
```c++
// this object uses the sprite manSprite at (150, 50)
Player *player = new Player(manSprite, 150, 50, TopScreen);
```
Don't need a sprite? Don't give the object one.
```c++
Player *player = new Player();
```

## Sprites
For now, use 8-bit paletted images. See [example_code/images](example_code/images) directory for examples of proper `.grit` files to go with them.
```c++
includeTiles(man);
Sprite *manSprite = new Sprite(SpriteData(man), Size_32x32, Colors256);
```
### Spritesheets
```c++
includeTiles(spriteSheet);
SpriteSheet *spriteSheet = new SpriteSheet(SpriteData(spriteSheet), Size_32x32, Colors256);
Sprite *manSprite = spriteSheet->getByIndex(3);
```

## Input
```c++
if(Buttons.A.Pressed()) {
  // do something...
} else if(Buttons.Up.Held()) {
  // do something else...
}
```

## Backgrounds
Same deal as sprites; use 8-bit paletted images.
```c++
includeTiles(bg);
includeMap(bg); // this can be from bg.png or bg.tmx (tilemap)
Background.Set(BackgroundData(bg), MapData(bg), TopScreen);
```

## Random Numbers
```c++
// get random number
return Random.Next();
```

## The Main Function
Use this in favor of your own `int main`:
```c++
#include <NDSA/Main.hh>
void NDSA::Game() {
  while(System.Frame()) {
    // this code runs once every frame
  }
}
```