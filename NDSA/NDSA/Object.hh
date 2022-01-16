namespace NDSA {
  // objects have directions to move
  enum Directions {
    Up, Down,
    Left, Right,
    UpLeft, UpRight,
    DownLeft, DownRight
  };
  
  class Object {
    // identifier for sprite.
    int ID;
    
    // identifier for dynamic object list.
    std::list<Object*>::iterator ListID;
    
    // and the sprite itself!
    Sprite *ObjSprite;
    
    // DUAL SCREEN SUPPORT!
    NDSA_Screen SprScreen;
    
    // not all objects are sprites.
    bool Sprited = false;
    
    void Update() {
      // Sprite.hh is unaware of the internals
      // of the object class, so we pass manually.
      Sprites.ObjUpdate(ID, round(X), round(Y), ObjSprite, SprScreen); 
    }
    
    void AddToList() {
      Lists.Objects.push_back(this);
      ++Lists.ObjectsIt;
      ListID = Lists.ObjectsIt;
    }
    
    public:
    float X, Y;
    
    // object movement magic
    int MovementStep = 0;
    Directions MovementDir;

    // all objects have code.
    virtual void Run() { }
    
    Object(Sprite *nSprite, int nX, int nY, NDSA_Screen nSprScreen)
     :  X(nX), Y(nY), ObjSprite(nSprite), SprScreen(nSprScreen) {
      ID = Sprites.Find_Slot();
      
      AddToList();
      
      
      Sprited = true;
      Update();
      Run();
    }
    
    Object() { 
      // simple!
      AddToList();
      Run();
    }
    
    ~Object() {
      if (Sprited == true) {
        Sprites.Empty_Slot(ID);
        oamClear(&oamMain, ID, 1);
      }
      Lists.Objects.erase(ListID);
    }
    
    void Move(Directions Dir, float Number) {
      sassert(Sprited == true, "Movement is for sprites only");
      
      switch (Dir) {
        case Up: Y -= Number; break;
        case Down: Y += Number; break;
        case Left: X -= Number; break;
        case Right: X += Number; break;
        case UpLeft: X -= Number; Y -= Number; break;
        case UpRight: X += Number; Y -= Number; break;
        case DownLeft: X -= Number; Y += Number; break;
        case DownRight: X += Number; Y += Number; break;
      }
      
      Update();
    }
  };
}