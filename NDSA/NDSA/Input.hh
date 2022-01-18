namespace NDSA {
  class Button {
    short int Key;
    
    public:
    Button(int nKey) : Key(nKey) { }
    
    inline int Pressed() {
      return keysDown() & Key;
    }
    
    inline int Held() {
      return keysHeld() & Key;
    }
  };

  struct {
    Button A      {KEYPAD_BITS::KEY_A},
           B      {KEYPAD_BITS::KEY_B},
           Up     {KEYPAD_BITS::KEY_UP},
           Down   {KEYPAD_BITS::KEY_DOWN},
           Left   {KEYPAD_BITS::KEY_LEFT},
           Right  {KEYPAD_BITS::KEY_RIGHT},
           Start  {KEYPAD_BITS::KEY_START},
           Select {KEYPAD_BITS::KEY_SELECT},
           L      {KEYPAD_BITS::KEY_L},
           R      {KEYPAD_BITS::KEY_R},
           X      {KEYPAD_BITS::KEY_X},
           Y      {KEYPAD_BITS::KEY_Y},
           Touch  {KEYPAD_BITS::KEY_TOUCH};
  } Buttons;
  
  class {
    touchPosition TPos;
    public:
    void Update() { touchRead(&TPos); }
    
    int Held() { return Buttons.Touch.Held(); }
    int Tapped() { return Buttons.Touch.Pressed(); }
    
    int Raw_X() { return TPos.rawx; }
    int X()     { return TPos.px; }
    
    int Raw_Y() { return TPos.rawy; }
    int Y()     { return TPos.py; }
  } TouchScreen;
}
