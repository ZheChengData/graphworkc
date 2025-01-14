#include "CNode.h"
#pragma once
class CLink {
public:
	int ID; 
	double FreeFlowTravelTime; 
	double TravelTime; 
	double Capacity;
	double Alpha = 0.15; 
	double Power = 4.0; 
	int InNodeIndex;  
	int OutNodeIndex; 

	CLink()
		:
		ID(0),
		FreeFlowTravelTime(0.0),
		Capacity(0.0)
		{}
};
