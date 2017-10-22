static const char* detailsNames[] = {
    "engine",
    "fuel_tank",
    "oxide_tank",
    "sabilizers",
    "antennas",
    "sensors",
    "spoilers",
    "rudder"
};

const int detailscount = 8;

bool details[detailscount];

void repairAll()
{
    for (int i = 0; i < detailscount; i++) {
        details[i] = true;
    }
}

bool checkRocket()
{
    for (int i = 0; i < detailscount; i++) {
        if (details[i] == true)
            return true;
    }
    return false;
}

char findAnyWorking()
{
    char cur;
    do {
        cur = rand() % 8;
    } while (details[cur] == false);
    return cur;
}
