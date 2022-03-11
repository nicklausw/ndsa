namespace NDSA {
  extern struct KeysObj {
    u16 keysDown;
    u16 keysHeld;
  } Keys;

  class Button {
    short int Key;
    
    public:
    Button(int nKey) : Key(nKey) { }
    
    inline int Pressed() {
      return Keys.keysDown & Key;
    }
    
    inline int Held() {
      return Keys.keysHeld & Key;
    }
  };

  extern struct ButtonsObj {
    Button A      {KEYPAD_BITS::KEY_A},
           B      {KEYPAD_BITS::KEY_B},
           Up     {KEYPAD_BITS::KEY_UP},
           Down   {KEYPAD_BITS::KEY_DOWN},
           Left   {KEYPAD_BITS::KEY_LEFT},
           Right  {KEYPAD_BITS::KEY_RIGHT},
           Start  {KEYPAD_BITS::KEY_START},
           Select {KEYPAD_BITS::KEY_SELECT},
           L      {KEYPAD_BITS::KEY_L},
           R      {KEYPAD_BITS::KEY_R}
           #ifdef DS
           ,X      {KEYPAD_BITS::KEY_X},
           Y      {KEYPAD_BITS::KEY_Y},
           Touch  {KEYPAD_BITS::KEY_TOUCH}
           #endif
           ;

  } Buttons;
  
  #ifdef DS
  extern class TSObj {
    touchPosition TPos;
    public:
    inline void Update() { touchRead(&TPos); }
    
    inline int Held() { return Buttons.Touch.Held(); }
    inline int Tapped() { return Buttons.Touch.Pressed(); }
    
    inline int Raw_X() { return TPos.rawx; }
    inline int X()     { return TPos.px; }
    
    inline int Raw_Y() { return TPos.rawy; }
    inline int Y()     { return TPos.py; }
  } TouchScreen;
  #endif
}
