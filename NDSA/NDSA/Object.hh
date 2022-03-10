namespace NDSA {
  // objects have directions to move
  enum Direction {
    Up, Down,
    Left, Right,
    UpLeft, UpRight,
    DownLeft, DownRight
  };

  class Object {
    // identifier for sprite.
    int ID;
    
    // and the sprite itself!
    Sprite *ObjSprite;
    
    // DUAL SCREEN SUPPORT!
    NDSA_Screen SprScreen;
    
    // not all objects are sprites.
    bool Sprited = false;

    bool hasCollision = false;
    
    void AddToList() {
      Objects.add(this);
    }
    
    public:
    bool isFalling = false;

    void Update() {
      // Sprite.hh is unaware of the internals
      // of the object class, so we pass manually.
      Sprites.ObjUpdate(ID, round(X), round(Y), ObjSprite, SprScreen); 
    }

    void addCollision(int x, int y) {
      hasCollision = true;
      width = x;
      height = y;
      Colliders.add(this);
    }

    // a unique random number for every instance.
    long PublicID;

    float X, Y;

    int xSize = 0, ySize = 0;
    int width = 0, height = 0;

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


      switch(ObjSprite->SprSize) {
        case SpriteSize_16x16: xSize = 16; ySize = 16; break;
        case SpriteSize_16x32: xSize = 16; ySize = 32; break;
        case SpriteSize_16x8: xSize = 16; ySize = 8; break;
        case SpriteSize_32x16: xSize = 32; ySize = 16; break;
        case SpriteSize_32x32: xSize = 32; ySize = 32; break;
        case SpriteSize_32x64: xSize = 32; ySize = 64; break;
        case SpriteSize_32x8: xSize = 32; ySize = 8; break;
        case SpriteSize_64x32: xSize = 64; ySize = 32; break;
        case SpriteSize_64x64: xSize = 64; ySize = 64; break;
        case SpriteSize_8x16: xSize = 8; ySize = 16; break;
        case SpriteSize_8x32: xSize = 8; ySize = 32; break;
        case SpriteSize_8x8: xSize = 8; ySize = 8; break;
      }
      
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
      if(hasCollision) Colliders.removeByInstance(this);
      Objects.removeByInstance(this);
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

      if(X < 0) X = 0;
      else if(X > 256 - xSize) X = 256 - xSize;
      if(Y < 0) Y = 0;
      else if(Y > 192 - ySize) Y = 192 - ySize;
      
      Update();
    }
  };
}