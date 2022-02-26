namespace NDSA {
  // objects have directions to move
  enum Direction {
    Up, Down,
    Left, Right,
    UpLeft, UpRight,
    DownLeft, DownRight
  };
  
  class Object {
    // identifier on object table.
    int objectID;
    // identifier for sprite.
    int ID;
    
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
      for(unsigned int c = 0; c < 2048; c++) {
        if(!Lists.Objects[c]) {
          Lists.Objects[c] = this;
          objectID = c;
          break;
        }
      }
      Lists.objectCount++;
    }
    
    public:
    // a unique random number for every instance.
    long PublicID;

    float X, Y;

    // all objects have code run each frame.
    virtual void Step() { }

    // alias to make inherited objects using
    // parent constructors look cleaner.
    #define ParentConstructors Object::Object;
    
    Object(Sprite *nSprite, int nX, int nY, NDSA_Screen nSprScreen)
     :  X(nX), Y(nY), ObjSprite(nSprite), SprScreen(nSprScreen) {
      PublicID = Random.Next();
      ID = Sprites.Find_Slot();
      
      AddToList();
      
      Sprited = true;
      Update();
    }
    
    Object() { 
      // simple!
      AddToList();
    }
    
    virtual ~Object() {
      if (Sprited == true) {
        Sprites.Empty_Slot(ID);
        oamClear(&oamMain, ID, 1);
      }
      Lists.Objects[objectID] = 0;
      Lists.objectCount--;
    }
    
    void Move(Direction Dir, float Number) {
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