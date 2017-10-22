#include <iostream>
#include <vector>
#include <algorithm>

enum  details {
    wing_1,
    wing_2,
    fuselage,
    chassis,
    tail,
    pilot
};

class Plain {
private:
    int critical_condition;
    std::vector<details> broke_details;

public:
    Plain();
    std::vector<details> getBrokeDetails() { return broke_details; }
    bool destroyDetail(details detail);
    bool repairDetail(details detail);
    bool checkCriticalCondition();
};

Plain::Plain() : critical_condition(3) {

}

// std::vector<details> getBrokeDetails() {
//     return broke_details;
// }

bool Plain::destroyDetail(details detail) {
    std::vector<details>::iterator it;
    it = find(broke_details.begin(), broke_details.end(), detail);

    if (it == broke_details.end()) {
        broke_details.push_back(detail);

        return true;
    }

    return false;
}

bool Plain::repairDetail(details detail) {
    std::vector<details>::iterator it;
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