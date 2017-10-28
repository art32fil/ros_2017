#include <iostream>
#include <stdlib.h>

class MazeFrame {
private:
    int hunter_x, hunter_y;
    static int maze_data[][21];

protected:
    void drawMaze();
    bool checkTreasure(const int x, const int y);
    void finalize();
public:
    MazeFrame(); 
    bool moveUp();
    bool moveRight();
    bool moveDown();
    bool moveLeft();
};


int MazeFrame::maze_data[][21] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1},
    {1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1},
    {1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1},
    {1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1},
    {1,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1},
    {1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1},
    {1,0,1,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,1},
    {1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1},
    {1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1},
    {1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1},
    {1,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,1,0,1},
    {1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,2,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

MazeFrame::MazeFrame() : hunter_x(1), hunter_y(1) {
    drawMaze();
}

void MazeFrame::drawMaze() {
    std::system("clear");

    for (int i = 0; i < 21; ++i) {
        for (int j = 0; j < 21; ++j) {
            if (i == hunter_x && j == hunter_y) {
                std::cout << "$";
            }
            else if (maze_data[i][j] == 1) {
                std::cout << "▓";
            }
            else if (maze_data[i][j] == 2) {
                std::cout << "*";
            }
            else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

bool MazeFrame::checkTreasure(const int x, const int y) {
    if (maze_data[x][y] == 2) {
        finalize();
        return true;
    }
    return false;
}

void MazeFrame::finalize() {
    hunter_x = hunter_y = 1;
    std::cout << ("Good Job! Take the patty from the shelf") << std::endl;
    sleep(3);
    std::system("clear");
    drawMaze();
}

bool MazeFrame::moveUp() {
    if (maze_data[hunter_x - 1][hunter_y] == 0) {
        hunter_x = hunter_x - 1;
        drawMaze();
        return true;
    }

    if (checkTreasure(hunter_x - 1, hunter_y)) {
        return true;
    }

    return false;
}

bool MazeFrame::moveRight() {
    if (maze_data[hunter_x][hunter_y + 1] == 0) {
        hunter_y = hunter_y + 1;
        drawMaze();
        return true;
    }

    if (checkTreasure(hunter_x, hunter_y + 1)) {
        return true;
    }

    return false;
}

bool MazeFrame::moveDown() {
    if (maze_data[hunter_x + 1][hunter_y] == 0) {
        hunter_x = hunter_x + 1;
        drawMaze();
        return true;
    }
    
    if (checkTreasure(hunter_x + 1, hunter_y)) {
        return true;
    }

    return false;
}

bool MazeFrame::moveLeft() {
    if (maze_data[hunter_x][hunter_y - 1] == 0) {
        hunter_y = hunter_y - 1;
        drawMaze();
        return true;
    }

    if (checkTreasure(hunter_x, hunter_y - 1)) {
        return true;
    }
    
    return false;
}