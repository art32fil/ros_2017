#include <iostream>
#include <cstdlib>

class Labirint{
private:
    int statex_, statey_;
    static const int N = 15;
    static char a[][N];
    void drawState();
    void finish();

public:
    Labirint();
    bool goLeft();
    bool goUp();
    bool goDown();
    bool goRight();
    void cheeterWin();
};

char Labirint::a[][N]=
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

Labirint::Labirint():statex_(1),statey_(1)
{
    drawState();
}

void Labirint::drawState()
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

void Labirint::finish()
{
    statex_ = statey_ = 1;
    std::system("clear");
    std::cout<< "YOU WIN" <<std::endl;
    sleep(3);
    drawState();
}

void Labirint::cheeterWin()
{
    statex_ = statey_ = 1;
    std::system("clear");
    std::cout<< "YOU CHEETER, BUT YOU WIN" <<std::endl;
    sleep(3);
    drawState();
}

bool Labirint::goLeft()
{
    if(a[statex_][statey_-1] == ' ')
    {
        statey_--;
        drawState();
        return true;
    }
    if(a[statex_][statey_-1] == '#')
    {
        finish();
        return true;
    }
    return false;
}

bool Labirint::goUp()
{
    if(a[statex_-1][statey_] == ' ')
    {
        statex_--;
        drawState();
        return true;
    }
    if(a[statex_-1][statey_] == '#')
    {
        finish();
        return true;
    }
    return false;
}

bool Labirint::goDown()
{
    if(a[statex_+1][statey_] == ' ')
    {
        statex_++;
        drawState();
        return true;
    }
    if(a[statex_+1][statey_] == '#')
    {
        finish();
        return true;
    }
    return false;
}

bool Labirint::goRight()
{
    if(a[statex_][statey_+1] == ' ')
    {
        statey_++;
        drawState();
        return true;
    }
    if(a[statex_][statey_+1] == '#')
    {
        finish();
        return true;
    }
    return false;
}
