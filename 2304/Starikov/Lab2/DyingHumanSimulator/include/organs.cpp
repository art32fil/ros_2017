static const char* organsNames[] = {
    "heart",
    "liver",
    "brain",
    "kidneys",
    "lungs",
    "arms"
};


bool organs[6];

bool checkHuman()
{
    for (int i = 0; i < 6; i++) {
        if (organs[i] == true)
            return true;
    }
    return false;
}

char findAnyAlive()
{
    char cur;
    do {
        cur = rand() % 8;
    } while (organs[cur] == false);
    return cur;
}

void initHuman()
{
    for (int i = 0; i < 6; i++) {
        organs[i] = true;
    }
}
