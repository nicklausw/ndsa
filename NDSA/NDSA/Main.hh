namespace NDSA {
  PointerList<Object> Objects;
  PointerList<Object> Colliders;
  BGObj Background;
  ButtonsObj Buttons;

  #ifdef DS
  TSObj TouchScreen;
  #endif

  #ifdef GBA
  GBATilesObj GBATiles;
  #endif
  
  RandomObj Random;
  SpritesObj Sprites;
  CollisionEventList CollisionEvents;
  KeysObj Keys;
}

int main() {
  NDSA::System.Initialize();
  NDSA::Game();
  return 0;
}