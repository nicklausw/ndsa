namespace NDSA {
  ListsObj Lists;
  DSObj DS;
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