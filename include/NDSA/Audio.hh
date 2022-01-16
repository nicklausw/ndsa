namespace NDSA {
  // this is very self explanatory,
  // so there's no comments.
  class {
    int Song;
    public:
    void Load(int nSong) {
      Song = nSong;
      mmLoad( Song );
    }
    
    void Play() {
      mmStart( Song, MM_PLAY_LOOP );
    }
    
    void LoadPlay(int nSong) {
      Load(nSong);
      Play();
    }
  } Audio;
}
