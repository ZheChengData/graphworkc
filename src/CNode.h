#include <iostream>
#include <vector>

#pragma once

using namespace std;

class CNode {
public:
	int ID;
	double PositionX = 0;
	double PositionY = 0;
	vector<int> IncomingLink;
	vector<int> OutgoingLink;
	
	bool operator==(const CNode& other) const {
		return this->ID == other.ID;
	}
	
	CNode(int id = 0, double x = 0.0, double y = 0.0, int originId = -1, int zone_ID = -1)
		: ID(id), PositionX(x), PositionY(y) {}
};

