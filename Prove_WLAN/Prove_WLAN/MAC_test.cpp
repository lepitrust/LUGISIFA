#include "StdAfx.h"
#include "MAC_test.h"


MAC_test::MAC_test(void)
{
}


MAC_test::~MAC_test(void)
{
}

void MAC_test::InsertElement(std::string MacAddress, int iRSSI)
{
	lMacAddress.insert(std::make_pair(MacAddress,iRSSI));
}
float MAC_test::ErrorValue(std::map<std::string, int> lMacAddress_testing)
{
	/*Utilizza una funzione del average mean square (media quadratica)*/ 
	float error_value=0;
	int nValue=0;
	for(std::map<std::string, int>::iterator ii=lMacAddress_testing.begin(); ii!=lMacAddress_testing.end(); ++ii)
	{
		float diff=(lMacAddress[(*ii).first])-(*ii).second;
		std::cout << "[MAC_scan:Memory-iRSSI=diff] " << (*ii).first << ": (" << (lMacAddress[(*ii).first]) << ")-(" << (*ii).second << ")=(" << diff << ")" << std::endl;
		if(abs(diff)!=abs((*ii).second))
		{
			//Esiste nella mappa così dovrei avere solo quelli comuni
			error_value+=pow(diff, 2);
			nValue++;
		}
	}
	std::cout << "[errorvalue:nvalue] "<< error_value << ":" << nValue << std::endl;
	error_value=error_value / nValue;
	return error_value;
}