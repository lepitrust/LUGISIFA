#pragma once
#include <map>
#include <string>
#include <iostream>
using namespace std;
class MAC_test
{
public:
	MAC_test(void);
	~MAC_test(void);
	void InsertElement(string MacAddress, int iRSSI);
	float ErrorValue(map<string, int> lMacAddress_testing);
private:
	map<string, int> lMacAddress;
};

