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
    #ifdef DS
    NDSA_Screen SprScreen;
    #endif

    #ifdef GBA
    OBJATTR *attr;
    #endif
    
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
      Sprites.ObjUpdate(ID, round(X), round(Y), ObjSprite
      #ifdef DS
      , SprScreen
      #endif
      #ifdef GBA
      , attr
      #endif
      ); 
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

    // returns the unused pixels on the object's sides.
    inline int xDeadSpace() {
      return (xSize - width) / 2;
    }

    // returns the unused pixels on the object's top and bottom.
    inline int yDeadSpace() {
      return (ySize - height) / 2;
    }

    // all objects have code run each frame.
    virtual void Step() { }

    // alias to make inherited objects using
    // parent constructors look cleaner.
    #define ParentConstructors Object::Object;
    
    Object(Sprite *nSprite, int nX, int nY
    #ifdef DS
    , NDSA_Screen nSprScreen
    #endif
    ) :  X(nX), Y(nY), ObjSprite(nSprite)
    #ifdef DS
    , SprScreen(nSprScreen)
    #endif
    {
      PublicID = Random.Next();
      ID = Sprites.Find_Slot();
      #ifdef GBA
      attr = OAM + ID;
      #endif
      
      AddToList();


      switch(ObjSprite->SprSize) {
        case Size_16x16: xSize = 16; ySize = 16; break;
        case Size_16x32: xSize = 16; ySize = 32; break;
        case Size_16x8:  xSize = 16; ySize = 8;  break;
        case Size_32x16: xSize = 32; ySize = 16; break;
        case Size_32x32: xSize = 32; ySize = 32; break;
        case Size_32x64: xSize = 32; ySize = 64; break;
        case Size_32x8:  xSize = 32; ySize = 8;  break;
        case Size_64x32: xSize = 64; ySize = 32; break;
        case Size_64x64: xSize = 64; ySize = 64; break;
        case Size_8x16:  xSize = 8;  ySize = 16; break;
        case Size_8x32:  xSize = 8;  ySize = 32; break;
        case Size_8x8:   xSize = 8;  ySize = 8;  break;
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
        #ifdef DS
        oamClear(&oamMain, ID, 1);
        #endif
        #ifdef GBA
        attr->attr0 = 0;
        attr->attr1 = 0;
        attr->attr2 = 0;
        #endif
      }
      if(hasCollision) Colliders.removeByInstance(this);
      Objects.removeByInstance(this);
    }
    
    void Move(Direction Dir, float Number) {
      if(!Sprited) Fatal("Movement is for sprites only");
      
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
      
      if(X - xDeadSpace() < 0) X = 0 - xDeadSpace();
      else if(X + xSize - xDeadSpace() > SCREEN_WIDTH) X = SCREEN_WIDTH - width - xDeadSpace();
      if(Y - yDeadSpace() < 0) Y = 0 - yDeadSpace();
      else if(Y + ySize - yDeadSpace() > SCREEN_HEIGHT) Y = SCREEN_HEIGHT - height - yDeadSpace();
      
      Update();
    }
  };
}