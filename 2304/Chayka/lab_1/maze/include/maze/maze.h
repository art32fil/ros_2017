
class Maze{
private:
  int Sx, Sy;
  static const int N = 10;
  static char a[][N];
  static char cat[][N];
  void drawFin();
  void fin();

public:
  Maze();
  void draw();
  bool moveLeft();
  bool moveUp();
  bool moveDown();
  bool moveRight();
};
