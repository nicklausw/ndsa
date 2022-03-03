namespace NDSA {
  Object *Objects[MAX_OBJECTS];
  BGObj Background;
  ButtonsObj Buttons;
  TSObj TouchScreen;
  RandomObj Random;
  SpritesObj Sprites;
}

int main() {
  NDSA::DS.Initialize();
  NDSA::Game();
  return 0;
}