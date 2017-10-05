#include <iostream>
#include <cstdlib>

class Maze {
private:
    int statex_, statey_;
    static const int N = 15;
    static char a[][N];
    void checkVictory(int x, int y);
    void drawState();
    void finish();

public:
    Maze();
    bool goLeft();
    bool goUp();
    bool goDown();
    bool goRight();
};

char Maze::a[][N]=
{
    {'-','-','-','-','-','-','-','-','-','-','-','-','-','-','-'},
    {'|',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|'},
    {'|',' ','|',' ','-','-','-','|','-','-','-','-','|',' ','|'},
    {'|',' ','|',' ','|','#','-','|',' ',' ',' ',' ','|',' ','|'},
    {'|',' ','|',' ','|',' ',' ','|',' ','-','|',' ','|',' ','|'},
    {'|',' ','|',' ','|','-',' ','|',' ',' ','|',' ','|',' ','|'},
    {'|',' ','|',' ',' ',' ',' ','|','-',' ','|',' ','|',' ','|'},
    {'|',' ','|','-','-','-',' ','-',' ',' ','|',' ','|',' ','|'},
    {'|',' ','|',' ',' ',' ',' ','-',' ','-',' ',' ',' ',' ','|'},
    {'|',' ','|',' ','-','-','-','-',' ',' ','-','-','|',' ','|'},
    {'|',' ','|',' ',' ',' ',' ',' ','-',' ','-',' ','|',' ','|'},
    {'|',' ','|',' ',' ','-',' ',' ',' ',' ',' ',' ','|',' ','|'},
    {'|',' ',' ','-','-','-','-','-','-','-','-','-','-',' ','|'},
    {'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|'},
    {'-','-','-','-','-','-','-','-','-','-','-','-','-','-','-'}
};

Maze::Maze():statex_(1),statey_(1)
{
    drawState();
}


void Maze::checkVictory(int x, int y) {
    if(a[x][y] == '#')
    {
        finish();
    }
}

void Maze::drawState()
{
    std::system("clear");
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(i!=statex_ || j!=statey_)
                std::cout<<a[i][j]<<" ";
            else
                std::cout<<"$ ";
        }
        std::cout<<std::endl;
    }
}


void Maze::finish()
{
    statex_ = statey_ = 1;
    std::system("clear");
    std::cout<< "Victory! Congratulations!" <<std::endl;
    sleep(3);
    drawState();
}

bool Maze::goLeft()
{
    if(a[statex_][statey_-1] == ' ')
    {
        statey_--;
        drawState();
        return true;
    }
    checkVictory(statex_, statey_-1);
    return false;
}

bool Maze::goUp()
{
    if(a[statex_-1][statey_] == ' ')
    {
        statex_--;
        drawState();
        return true;
    }
    checkVictory(statex_-1, statey_);
    return false;
}

bool Maze::goDown()
{
    if(a[statex_+1][statey_] == ' ')
    {
        statex_++;
        drawState();
        return true;
    }
    checkVictory(statex_+1, statey_);
    return false;
}

bool Maze::goRight()
{
    if(a[statex_][statey_+1] == ' ')
    {
        statey_++;
        drawState();
        return true;
    }
    checkVictory(statex_, statey_+1);
    return false;
}
