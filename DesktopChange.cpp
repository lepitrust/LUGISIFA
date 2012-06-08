// Pr6.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <wininet.h> //metodo active desktop
#include <ShlObj.h> //metodo active desktop
#include <netfw.h>

using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	char a,b;
	int ris;
	OPENFILENAME ofn;
	TCHAR nomeFile[50];

	ZeroMemory(&ofn,sizeof(ofn));
    ofn.lStructSize=sizeof(ofn);
    ofn.lpstrFilter=_T("File immagine (*.jpg)\0*.jpg\0");
	nomeFile[0]='\0';
	ofn.lpstrFile=nomeFile;
    ofn.nMaxFile=MAX_PATH; 
    ofn.Flags=OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
    ofn.lpstrTitle=_T("Seleziona il file");

	CoInitializeEx(0,COINIT_APARTMENTTHREADED);
    IActiveDesktop* iADesktop;
    HRESULT status = CoCreateInstance(CLSID_ActiveDesktop,NULL,CLSCTX_INPROC_SERVER,IID_IActiveDesktop,(void**)&iADesktop);
    WALLPAPEROPT wOption;
    ZeroMemory(&wOption, sizeof(WALLPAPEROPT));
    wOption.dwSize=sizeof(WALLPAPEROPT);

    wOption.dwStyle = WPSTYLE_CENTER; //ce ne sono diverse

	cout << "Cambiare sfondo del desktop? [y/n]" << endl;
	cin >> a;
	switch(a)
	{
	  case 'y':
		      cout << "Sfondo di default o personalizzato? [d/p]" << endl;
			  cin >> b;
			  switch(b)
			  {
			  case 'p':
				        //Qui apro il file, ma nella versione finale basta andare a prendere la stringa 
				        //relativa allo sfondo corrente nel registro di sistema
				        //HKEY_CURRENT_USER\Control Panel\Desktop, oppure usare
				        //string WallCorrente = new string('\0', 255);
				        // Win32.SystemParametersInfo(SPI_GETDESKWALLPAPER, WallCorrente.Length, WallCorrente, 0);
				        //e salvarla nella struct relativa al profilo corrente
				        GetOpenFileName(&ofn);
                        status = iADesktop->SetWallpaper(ofn.lpstrFile,0);
                        status = iADesktop->SetWallpaperOptions(&wOption,0);
                        status = iADesktop->ApplyChanges(AD_APPLY_ALL);
						//alternativa con SystemParametersInfo, ma funziona a seconda del SO
                        //if(ris = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, ofn.lpstrFile, SPIF_UPDATEINIFILE|SPIF_SENDCHANGE)!=1)
						//{ cout << "Impossibile cambiare lo sfondo\n" << endl; return 0; } cout << "Sfondo cambiato" << endl;
			            //system("PAUSE");
			            if(status!=NULL)
						{ cout << "Impossibile cambiare lo sfondo\n" << endl; return 0; } cout << "Sfondo cambiato" << endl;
			            system("PAUSE");
						iADesktop->Release();
                        CoUninitialize();
			  break;
			  case 'd':
                        status = iADesktop->SetWallpaper((LPCWSTR)"",0);
                        status = iADesktop->SetWallpaperOptions(&wOption,0);
                        status = iADesktop->ApplyChanges(AD_APPLY_ALL);
						if(status!=NULL)
						{ cout << "Impossibile cambiare lo sfondo\n" << endl; return 0; } cout << "Sfondo cambiato" << endl;
			            system("PAUSE");
						iADesktop->Release();
                        CoUninitialize();
			  break;
			  default:
				  cout << "Comando non riconosciuto" << endl;
			      system("PAUSE");
			  break;
			  }
	  break;
	  case 'n':
			  cout << "Sfondo non cambiato" << endl;
			  system("PAUSE");
	  break;
	  default:
		      cout << "Comando non riconosciuto" << endl;
			  system("PAUSE");
	  break;
	}


	//impostazioni firewall
	
return 0;
}