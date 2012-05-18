// Prove_WLAN.cpp : definisce il punto di ingresso dell'applicazione console.
//
//dannato GIT
//che idea mi è venuta...
#include "stdafx.h"

static void PrintMACaddress(unsigned char MACData[]);

int _tmain(int argc, _TCHAR* argv[])
{
 // Declare and initialize variables.

    HANDLE hClient = NULL;		//Handle del client utilizzato nella sessione corrente
    DWORD dwMaxClient = 2;      //E' una costante che va inserita in WlanOpenHandle (1->Win XP SP2/3 | 2->Win Vista/Server 2008)    
    DWORD dwCurVersion = 0;		//Versione della WLAN API attualmente utilizzata da inserire nella WlanOpenHandle
    DWORD dwResult = 0;
    DWORD dwRetVal = 0;
    int iRet = 0;
    
    WCHAR GuidString[39] = {0};		//Conterrà un GUID sottoforma di stringa di caratteri

    unsigned int i, j, k;

    /* variables used for WlanEnumInterfaces  */

    PWLAN_INTERFACE_INFO_LIST pIfList = NULL;	//Contiene un array di NIC con relative informazioni
    PWLAN_INTERFACE_INFO pIfInfo = NULL;		//Contiene le informazioni relative ad una NIC (ID, Descrizione, Stato)

    PWLAN_AVAILABLE_NETWORK_LIST pBssList = NULL;	//Contiene un array di reti con relative informazioni
    PWLAN_AVAILABLE_NETWORK pBssEntry = NULL;		//Contiene info relative ad una rete raggiungibile (es. SSID, qualità segnale, algoritmo cifratura)
    
    int iRSSI = 0;

    dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient); //Apre una connessione con il server
    if (dwResult != ERROR_SUCCESS)
		{
        wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
        return 1;
        // You can use FormatMessage here to find out why the function failed
		}

    dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList); //Inserisce in pIfList l'elenco delle NIC abilitate sul pc
    if (dwResult != ERROR_SUCCESS) 
		{
        wprintf(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
        return 1;
        // You can use FormatMessage here to find out why the function failed
		} 
	else 
		{
        wprintf(L"Num Entries: %lu\n", pIfList->dwNumberOfItems); //Numero NIC trovate
        wprintf(L"Current Index: %lu\n", pIfList->dwIndex);	//Indice NIC corrente
        for (i = 0; i < (int) pIfList->dwNumberOfItems; i++) 
			{
            pIfInfo = (WLAN_INTERFACE_INFO *) &pIfList->InterfaceInfo[i]; //pIfInfo punta alla struttura con le informazioni relative alla NIC corrente
            wprintf(L"  Interface Index[%u]:\t %lu\n", i, i);
            iRet = StringFromGUID2(pIfInfo->InterfaceGuid, (LPOLESTR) &GuidString,	//Trasforma un GUID in stringa di caratteri 
                sizeof(GuidString)/sizeof(*GuidString));  //Numero di caratteri massimo per la stringa finale
            // For c rather than C++ source code, the above line needs to be
            // iRet = StringFromGUID2(&pIfInfo->InterfaceGuid, (LPOLESTR) &GuidString, 
            //     sizeof(GuidString)/sizeof(*GuidString)); 
            if (iRet == 0)	//Se StringFromGUID2 fallisce ritorna 0
                wprintf(L"StringFromGUID2 failed\n");
            else 
				{		//Altrimenti il numero di caratteri della stringa 
                wprintf(L"  InterfaceGUID[%d]: %ws\n",i, GuidString);
				}    
            wprintf(L"  Interface Description[%d]: %ws", i, pIfInfo->strInterfaceDescription);
            wprintf(L"\n");
            wprintf(L"  Interface State[%d]:\t ", i);
            switch (pIfInfo->isState) 
				{
				case wlan_interface_state_not_ready:
					wprintf(L"Not ready\n");
					break;
				case wlan_interface_state_connected:
					wprintf(L"Connected\n");
					break;
				case wlan_interface_state_ad_hoc_network_formed:
					wprintf(L"First node in a ad hoc network\n");
					break;
				case wlan_interface_state_disconnecting:
					wprintf(L"Disconnecting\n");
					break;
				case wlan_interface_state_disconnected:
					wprintf(L"Not connected\n");
					break;
				case wlan_interface_state_associating:
					wprintf(L"Attempting to associate with a network\n");
					break;
				case wlan_interface_state_discovering:
					wprintf(L"Auto configuration is discovering settings for the network\n");
					break;
				case wlan_interface_state_authenticating:
					wprintf(L"In process of authenticating\n");
					break;
				default:
					wprintf(L"Unknown state %ld\n", pIfInfo->isState);
					break;
				}
            wprintf(L"\n");

			//Inserisce in pBssList i dati delle reti disponibili
            dwResult = WlanGetAvailableNetworkList(hClient,		//Client handle ottenuto all'inizio
                                             &pIfInfo->InterfaceGuid,	//Interface GUID
                                             0, 
                                             NULL, 
                                             &pBssList);	//Puntatore alla lista delle entry trovate

            if (dwResult != ERROR_SUCCESS) 
				{
                wprintf(L"WlanGetAvailableNetworkList failed with error: %u\n", dwResult);
                dwRetVal = 1;
                // You can use FormatMessage to find out why the function failed
				} 
			else 
				{
                wprintf(L"WLAN_AVAILABLE_NETWORK_LIST for this interface\n");

                wprintf(L"  Num Entries: %lu\n\n", pBssList->dwNumberOfItems);

                for (j = 0; j < pBssList->dwNumberOfItems; j++) 
					{
                    pBssEntry = (WLAN_AVAILABLE_NETWORK *) & pBssList->Network[j]; //pBssEntry punta alla struttura di informazioni relative alla rete corrente
                    wprintf(L"  Profile Name[%u]:  %ws\n", j, pBssEntry->strProfileName);
                    wprintf(L"  SSID[%u]:\t\t ", j);
                    if (pBssEntry->dot11Ssid.uSSIDLength == 0) //Se l'SSID ha lunghezza nulla va a capo
                        wprintf(L"\n");
                    else 
						{   
                        for (k = 0; k < pBssEntry->dot11Ssid.uSSIDLength; k++) 
							{
                            wprintf(L"%c", (int) pBssEntry->dot11Ssid.ucSSID[k]); //Scrittura a video dell SSID
							}
                        wprintf(L"\n");
						}
					
					//Prova per ottenere il MAC Address-----------------------------------------------
					//--------------------------------------------------------------------------------
					PWLAN_BSS_LIST pWlanBssList=NULL;
					DWORD uscita=0;
					DOT11_SSID Dot11Ssid=pBssEntry->dot11Ssid;
					
					WlanGetNetworkBssList(
						hClient,		//Client inizializzato precedentemente
						&(pIfInfo->InterfaceGuid),	//Interfaccia sul quale si fanno le misurazioni
						&(pBssEntry->dot11Ssid),	//Struttura del SSID correntemente analizzato
						dot11_BSS_type_infrastructure, //Cerca tra le interfacce di infrastruttura
						pBssEntry->bSecurityEnabled,	//Security policy dell'interfaccia corrente
						NULL,	//DEVE ESSERE NULL
						&pWlanBssList	//Struttura che sarà riempita con info opportune
						);
					if(uscita==ERROR_SUCCESS)
					wprintf(L"\n  GOOD Exit status Bss\n");
					wprintf(L"\n  Number of Bss entries: %u\n", pWlanBssList->dwNumberOfItems);
					for(int z=0; z<(int)pWlanBssList->dwNumberOfItems;z++){
					PWLAN_BSS_ENTRY px = (WLAN_BSS_ENTRY *) & pWlanBssList->wlanBssEntries[z];
					PrintMACaddress(px->dot11Bssid);}
					/*----------------------------------------------------------------------------------*/
					/*----------------------------------------------------------------------------------*/
                    wprintf(L"  BSS Network type[%u]:\t ", j);
                    switch (pBssEntry->dot11BssType) 
						{
						case dot11_BSS_type_infrastructure   :
							wprintf(L"Infrastructure (%u)\n", pBssEntry->dot11BssType);
							break;
						case dot11_BSS_type_independent:
							wprintf(L"Infrastructure (%u)\n", pBssEntry->dot11BssType);
							break;
						default:
							wprintf(L"Other (%lu)\n", pBssEntry->dot11BssType);
							break;
						}
                                
                    wprintf(L"  Number of BSSIDs[%u]:\t %u\n", j, pBssEntry->uNumberOfBssids);	
                    wprintf(L"  Connectable[%u]:\t ", j);
                    if (pBssEntry->bNetworkConnectable)
                        wprintf(L"Yes\n");
                    else 
						{
                        wprintf(L"No\n");
                        wprintf(L"  Not connectable WLAN_REASON_CODE value[%u]:\t %u\n", j, pBssEntry->wlanNotConnectableReason);
						}        
                    wprintf(L"  Number of PHY types supported[%u]:\t %u\n", j, pBssEntry->uNumberOfPhyTypes);

                    if (pBssEntry->wlanSignalQuality == 0)
                        iRSSI = -100;
                    else if (pBssEntry->wlanSignalQuality == 100)   
                        iRSSI = -50;
                    else
                        iRSSI = -100 + (pBssEntry->wlanSignalQuality/2);    
                        
                    wprintf(L"  Signal Quality[%u]:\t %u (RSSI: %i dBm)\n", j, pBssEntry->wlanSignalQuality, iRSSI);

                    wprintf(L"  Security Enabled[%u]:\t ", j);
                    if (pBssEntry->bSecurityEnabled)
                        wprintf(L"Yes\n");
                    else
                        wprintf(L"No\n");
         
                    wprintf(L"  Default AuthAlgorithm[%u]: ", j);
                    switch (pBssEntry->dot11DefaultAuthAlgorithm) 
						{
						case DOT11_AUTH_ALGO_80211_OPEN:
							wprintf(L"802.11 Open (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
							break;
						case DOT11_AUTH_ALGO_80211_SHARED_KEY:
							wprintf(L"802.11 Shared (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
							break;
						case DOT11_AUTH_ALGO_WPA:
							wprintf(L"WPA (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
							break;
						case DOT11_AUTH_ALGO_WPA_PSK:
							wprintf(L"WPA-PSK (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
							break;
						case DOT11_AUTH_ALGO_WPA_NONE:
							wprintf(L"WPA-None (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
							break;
						case DOT11_AUTH_ALGO_RSNA:
							wprintf(L"RSNA (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
							break;
						case DOT11_AUTH_ALGO_RSNA_PSK:
							wprintf(L"RSNA with PSK(%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
							break;
						default:
							wprintf(L"Other (%lu)\n", pBssEntry->dot11DefaultAuthAlgorithm);
							break;
						}
                        
                    wprintf(L"  Default CipherAlgorithm[%u]: ", j);
                    switch (pBssEntry->dot11DefaultCipherAlgorithm) 
						{
						case DOT11_CIPHER_ALGO_NONE:
							wprintf(L"None (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
							break;
						case DOT11_CIPHER_ALGO_WEP40:
							wprintf(L"WEP-40 (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
							break;
						case DOT11_CIPHER_ALGO_TKIP:
							wprintf(L"TKIP (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
							break;
						case DOT11_CIPHER_ALGO_CCMP:
							wprintf(L"CCMP (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
							break;
						case DOT11_CIPHER_ALGO_WEP104:
							wprintf(L"WEP-104 (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
							break;
						case DOT11_CIPHER_ALGO_WEP:
							wprintf(L"WEP (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
							break;
						default:
							wprintf(L"Other (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
							break;
						}

                    wprintf(L"  Flags[%u]:\t 0x%x", j, pBssEntry->dwFlags);
                    if (pBssEntry->dwFlags) 
						{
                        if (pBssEntry->dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED)
                            wprintf(L" - Currently connected");
                        if (pBssEntry->dwFlags & WLAN_AVAILABLE_NETWORK_HAS_PROFILE)
                            wprintf(L" - Has profile");
						}   
                    wprintf(L"\n");
                    
                    wprintf(L"\n");
                }
            }
        }

    }
    if (pBssList != NULL) 
		{
        WlanFreeMemory(pBssList);	//Libera la zona di memoria preposta alla lista di reti disponibili appena utilizzata
        pBssList = NULL;
		}

    if (pIfList != NULL) 
		{
        WlanFreeMemory(pIfList);	//Libera la zona di memoria preposta alla lista di NIC appena utilizzata
        pIfList = NULL;
		}
	system("pause");
    return dwRetVal;
}




/*PROVA FUNZIONE PER STAMPA MAC ADDRESS*/
static void PrintMACaddress(unsigned char MACData[])
{
    wprintf(L"  MAC Address: %02X-%02X-%02X-%02X-%02X-%02X\n", 
        MACData[0], MACData[1], MACData[2], MACData[3], MACData[4], MACData[5]);
}