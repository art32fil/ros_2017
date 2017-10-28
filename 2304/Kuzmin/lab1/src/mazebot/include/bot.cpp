#include <iostream>
#include <stdlib.h>

using namespace std;

enum Directions
{
  dirLeft,
  dirRight,
  dirUp,
  dirDown
};

class Bot
{
private:
  bool** field;
  int x;
  int y;
  int size;
  int walls_mul;

  void GenField()
  {
    field = new bool*[size];
    for (int i = 0; i < size; i++)
      {
        field[i]  = new bool[size];

        //set all empty
        for (int k = 0; k < size; k++)
          field[i][k] = false;

        //left and right walls
        field[i][0] = true;
        field[i][size - 1] = true;
      }

    //up wall
    for (int i = 0; i < size - 2; i++)
      field[0][i] = true;

    //down wall
    for (int i = 2; i < size - 1; i++)
      field[size - 1][i] = true;

    //random walls
    srand(time(NULL));
    for (int i = 0; i < size * walls_mul; i++)
      field[rand() % size][rand() % size] = true;

    //free start and finish
    field[1][size] = false;
    field[1][size - 2] = false;
    field[size - 2][0] = false;
    field[size - 2][1] = false;
  }

public:

  Bot(int size = 20, int walls_mul = 5)
  {
    this->size = size;
    this->walls_mul = walls_mul;
    GenField();
    y = size - 1;
    x = 1;
  }
  ~Bot()
  {
    for (int i = 0; i < size; i++)
      delete field[i];
    delete field;
  }

  void DrawAll()
  {
    system("clear");
    for (int i = 0; i < size; i++)
      {
        for (int k = 0; k < size; k++)
          {
            if (y == i && x == k)
              cout << "ᴥ";
            else if (field[i][k])
              cout << "█";
            else
              cout << " ";
          }
        cout << endl;
      }
  }

  void MoveLeft()
  {
    if (x - 1 < 0)
      return;
    if (!field[y][x - 1])
      x--;
  }
  void MoveRight()
  {
    if (x + 1 > size - 1)
      return;
    if (!field[y][x + 1])
      x++;
  }

  void MoveUp()
  {
    if (y - 1 < 0)
      return;
    if (!field[y - 1][x])
      y--;
  }
  void MoveDown()
  {
    if (y + 1 > size - 1)
      return;
    if (!field[y + 1][x])
      y++;
  }

  bool CheckIfWon()
  {
    if (x == size - 2 && y == 0)
      return true;
    else
      return false;
  }
};
