#pragma once
#include <map>
#include <string>
#include <iostream>
class MAC_test
{
public:
	MAC_test(void);
	~MAC_test(void);
	void InsertElement(std::string, int iRSSI);
	float ErrorValue(std::map<std::string, int> lMacAddress_testing);
private:
	std::map<std::string, int> lMacAddress;
};

