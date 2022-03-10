namespace NDSA {
  PointerList<Object> Objects;
  PointerList<Object> Colliders;
  BGObj Background;
  ButtonsObj Buttons;
  TSObj TouchScreen;
  RandomObj Random;
  SpritesObj Sprites;
  CollisionEventList CollisionEvents;
}

int main() {
  NDSA::DS.Initialize();
  NDSA::Game();
  return 0;
}