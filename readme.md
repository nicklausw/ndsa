# NDSA - Nintendo DS Advanced Engine
## Write for NDS and GBA in one codebase!
A project from 2016 that I'd like to bring back. It's a header library which wraps libnds/libgba functions, adds an object system, so on. If you have devkitPro installed, you should be able to clone the repo, run GNU make and have fun.

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
For now, use 8-bit paletted images. See [example_code](example_code) directory for examples of proper `.grit` files to go with them.
```c++
#include "man.h"
Sprite *manSprite = new Sprite(SpriteData(man), Size_32x32, Colors256);
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
#include "testImage.h"
Background.Set(BackgroundData(testImage), TopScreen);
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
  while(DS.Frame()) {
    // this code runs once every frame
  }
}
```