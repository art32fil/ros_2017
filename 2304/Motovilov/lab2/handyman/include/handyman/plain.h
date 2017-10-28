#include <iostream>
#include <vector>
#include <algorithm>

char* details[] {
    "wing_1",
    "wing_2",
    "fuselage",
    "chassis",
    "tail",
    "pilot"
};

class Plain {
private:
    int critical_condition;
    std::vector<char*> broke_details;

public:
    Plain();
    std::vector<char*> getBrokeDetails() { return broke_details; }
    bool destroyDetail(char* detail);
    bool repairDetail(char* detail);
    bool checkCriticalCondition();
};

Plain::Plain() : critical_condition(3) {

}

bool Plain::destroyDetail(char* detail) {
    std::vector<char*>::iterator it;
    it = find(broke_details.begin(), broke_details.end(), detail);

    if (it == broke_details.end()) {
        broke_details.push_back(detail);

        return true;
    }

    return false;
}

bool Plain::repairDetail(char* detail) {
    std::vector<char*>::iterator it;
    it = find(broke_details.begin(), broke_details.end(), detail);

    if (it != broke_details.end()) {
        broke_details.erase(it);

        return true;
    }

    return false;
}

bool Plain::checkCriticalCondition() {
    if (broke_details.size() < critical_condition) {
        return true;
    }
    
    return false;
}