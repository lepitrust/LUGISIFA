#include "StdAfx.h"
#include "MAC_test.h"
using namespace std;

MAC_test::MAC_test(void)
{
}


MAC_test::~MAC_test(void)
{
}

void MAC_test::InsertElement(string MacAddress, int iRSSI)
{
	lMacAddress.insert(make_pair(MacAddress,iRSSI));
}
float MAC_test::ErrorValue(map<string, int> lMacAddress_testing)
{
	/*Funzione di valutazione dell'errore tra le reti memorizzate e quelle rilevate*/ 
	using namespace std;
	float error_value=0;
	int nValue=0;
	for(map<string, int>::iterator ii=lMacAddress_testing.begin(); ii!=lMacAddress_testing.end(); ++ii)
	{
		float diff=(lMacAddress[(*ii).first])-(*ii).second;
		cout << "[MAC_scan:Memory-iRSSI=diff] " << (*ii).first << ": (" << (lMacAddress[(*ii).first]) << ")-(" << (*ii).second << ")=(" << diff << ")" << std::endl;
		if(abs(diff)!=abs((*ii).second))
		{
			//Il termine differenziale viene sommato solo se esiste nella mappa, in pratica considero solo i termini comuni.
			error_value+=pow(diff, 2);
			nValue++;
		}
	}
	cout << "[errorvalue:nvalue] " << error_value << ":" << nValue << endl;
	
	/**********media quadratica********************/
	//error_value=error_value / nValue;
	
	/**********distanza euclidea a N dimensioni****/
	error_value = sqrt(error_value);
	
	return error_value;
}