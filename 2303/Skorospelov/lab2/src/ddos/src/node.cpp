#include "ros/ros.h"
#include "ddos/msgs.h"
#include <vector>
#include <time.h>

int total = 0;
std::vector<int> diff({0, 3, 10, 20});
std::vector<int> rate({1000, 800, 700, 600});
std::vector<int> devCount({3, 4, 5, 6});
std::vector<std::string> difficulties({"nooby", "normal", "hard", "nightmare"});

std::vector<std::string> types({ "Router", "Switch", "Wireless" });
std::vector<std::string> routers({ "Linksys WRT3200ACM", "Nighthawk X4S", "Asus RT-AC88U", "TP-Link Deco M5", "TRENDnet TEW-828DRU", "Tri-Band AC3200" });
std::vector<std::string> switches({ "D-Link DGS-1210-52", "TP-LINK TL-SG1048", "Cisco SG200-26", "TRENDnet TEG-424WS", "TP-LINK TL-SG1024", "NETGEAR JGS516 ProSafe", "TP-LINK TL-SG1016" });
std::vector<std::string> wireless({ "Asus RT-AC88U", "D-Link AC3200", "Asus RT-AC5300", "Asus RT-AC3200", "Linksys WRT1900ACS", "Netgear Nighthawk R7000", "Asus RT-N66U"});
std::vector<std::string> firewalls({ "OpenWrt", "DD-WRT", "LEDE", "Roofnet", "ROOter", "Zeroshell", "FreeBSD", "Tomato" });
std::vector<std::string> activity({"play DOTA2", "read a book", "drink tea", "surf net", "go for a walk", "take a shower", "do 10 push-ups", "do 10 squats", "call mom", "draw a picture", "read sources", "fill in calendar"});

int getDiff() {
	for (int i = diff.size() - 1; i >= 0; i--) {
		if (total >= diff[i]) {
			return i;
		}
	}
	return 0;
}

int getRate() {
	return rate[getDiff()];
}

// <mask, Name>
struct part {
	bool isOk;
	//diff: 0
	int type; // 0 - router, 1 - switch, 2 - wireless
	//diff: 1
	int partnumber;
	//diff: 2
	int firewall;
	//diff: 3
	int revision;
	std::string str() {
		std::string res = "";
		if (!isOk) {
			res += "(broken) ";
		}
		res += types[this->type] + " ";
		if (getDiff() > 0) {
			if (this->type == 0) {
				res += routers[this->partnumber];
			}
			else if (this->type == 1) {
				res += switches[this->partnumber];
			}
			else if (this->type == 2) {
				res += wireless[this->partnumber];
			}
		}
		if (getDiff() > 1) {
			res += ", wirewall: ";
			res += firewalls[this->firewall];
		}
		if (getDiff() > 2) {
			res += ", revision: ";
			res += this->revision;
		}
		return res;
	}
	std::string getName() {
		if (this->type == 0) {
			return routers[partnumber];
		}
		else if (this->type == 1) {
			return switches[partnumber];
		}
		else if (this->type == 2) {
			return wireless[partnumber];
		}
	}
	std::string getFirewall() {
		return firewalls[firewall];
	}
};

part gen() {
	part res;
	res.isOk = true;
	res.type = rand() % types.size();
	res.firewall = rand() % firewalls.size();
	res.partnumber = 0;
	if (res.type == 0) {
		res.partnumber = rand() % routers.size();
	}
	else if (res.type == 1) {
		res.partnumber = rand() % switches.size();
	}
	else if (res.type == 2) {
		res.partnumber = rand() % wireless.size();
	}
	res.revision = rand() % 10;
	return res;
}
std::vector<part> devices;

bool fixPart(ddos::msgs::Request &req, ddos::msgs::Response &resp) {
	std::string type = req.type;
	std::string name = req.name;
	std::string firewall = req.firewall;
	int revision = req.revision;
	for (int i = 0; i < devices.size(); i++) {
		bool typeok = (strcmp(types[devices[i].type].c_str(), type.c_str()) == 0);
		bool nameok = (strcmp(devices[i].getName().c_str(), name.c_str()) == 0 || (getDiff() < 1));
		bool firewallok = (strcmp(devices[i].getFirewall().c_str(), firewall.c_str()) == 0 || (getDiff() < 2));
		bool revisionok = (devices[i].revision == revision || (getDiff() < 3));
		if (!devices[i].isOk && typeok && nameok && firewallok && revisionok) {
			devices[i].isOk = true;
			resp.result = "Ok, you fixed it!";
			total++;
			return true;
		}
	}
	resp.result = "No, wrong device";
	return false;
}

int countBroken() {
	int res = 0;
	for (int i = 0; i < devices.size(); i++) {
		res += (!devices[i].isOk);
	}
	return res;
}

int countAlive() {
	return devices.size() - countBroken();
}

void breakAlive(int idx) {
	std::vector<int> aliveIdx;
	for (int i = 0; i < devices.size(); i++) {
		if (devices[i].isOk) {
			aliveIdx.push_back(i);
		}
	}
	devices[aliveIdx[idx]].isOk = false;
	std::cout << "You have new device broken.\n";
}

// p=10 => 0.1% of "true" results
bool isP(int p, int max = 10000) {
	return (rand() % max) < p;
}

void showCurrentState() {
	int broken = countBroken();
	std::cout << "Current state: level " << getDiff() << " (" << difficulties[getDiff()] << "), you have " << devices.size() << " devices, " << broken << " are broken.\n";
	if (broken == 0) {
		std::cout << "Have a rest now and " << activity[total % activity.size()] << ".\n";
		return;
	}
	for (int i = 0; i < devices.size(); i++) {
		std::cout << i + 1 << ". " << devices[i].str() << std::endl;
	}
}

void checkDiff() {
	if (devices.size() < devCount[getDiff()]) {
		part p = gen();
		devices.push_back(p);
		std::cout << "You have a new device! Weeehaaaa! It is: " << p.str() << std::endl;
	}
}

int main(int argc, char **argv)
{
	srand(time(0));
	ros::init(argc, argv, "server");
	ros::NodeHandle n;
	ros::ServiceServer service = n.advertiseService("update_firmware", fixPart);
	std::cout << "\nYou are system administrator and hackers are gonna break into your system. " <<
		"You don't want them to do it, do you? " <<
		"So, they will find a lot of exploits in you router's hardware and you need to update it in time.\n" <<
		"Game has different difficulty, so don't go away early :)\n" <<
		"We will start in about 15 seconds, get ready:\n" <<
		"1. Open another terminal, type in the following line and double press tab:\n" <<
		"rosservice call update_firmware {\n" <<
		"2. You will need to specify parameters according to the given problems in network.\n" <<
		"3. You will lose only if you have all your devices broken.\n";
	ros::Duration(15).sleep();

	ros::Rate loop_rate(1);

	while (ros::ok()) {
		checkDiff();
		if (isP(getRate())) {
			int idx = rand() % countAlive();
			breakAlive(idx);
		}
		showCurrentState();
		if (countAlive() == 0 && countBroken() > 2) {
			std::cout << "Hackers broke into your system and now you are fired! Sorry, it was a nice job.\n" << 
				"Level: " << getDiff() << " (" << difficulties[getDiff()] << "), total score: " << total << std::endl;
			ros::shutdown();
		}
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
