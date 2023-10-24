//
#define rows 


#ifndef LOCMANAGER
#define LOCMANAGER

struct location {
  int x;
  int y;
  bool available;
}

//
class locationManager {
  public:
    location nearest_location(int x, int y);
    
    
  private:
    
}
#endif
