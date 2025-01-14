#include "CNode.h"
#pragma once
class CLink {
public:
	int ID; 
	double TravelTime; 
	double Capacity;
	double Alpha = 0.15; 
	double Beta = 4.0;
	int InNodeIndex;  
	int OutNodeIndex; 

	CLink()
		:
		ID(0),
		Capacity(0.0)
		{}
};
