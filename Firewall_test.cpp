#include "stdafx.h"
#include <stdarg.h>
#include <stdio.h>

#include "netfw.h"

HRESULT Firewall_on(IN INetFwProfile* fwProfile);
HRESULT Firewall_off(IN INetFwProfile* fwProfile);
void Deall(INetFwProfile* fwProfile, INetFwPolicy* Policy, INetFwMgr* Mgr);

int _tmain(int argc, _TCHAR* argv[])
{
	INetFwProfile* fwProfile = NULL;
	INetFwMgr* Mgr = NULL;
	INetFwPolicy* Policy = NULL;
	HRESULT ris = S_OK;
	VARIANT_BOOL fwEnabled;
	BOOLEAN acceso=FALSE;



	ris = CoInitializeEx(
                0,
                COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE 
				| COINIT_SPEED_OVER_MEMORY
                );

//crea istanza del firewall manager
        ris = CoCreateInstance(
                __uuidof(NetFwMgr),
                NULL,
                CLSCTX_INPROC_SERVER,
                __uuidof(INetFwMgr),
                (void**)&Mgr
                );

		if(SUCCEEDED(ris))
         {
			
            ris=Mgr->get_LocalPolicy(&Policy);
			if(SUCCEEDED(ris))
			{
			    fwProfile = NULL;
                ris = Policy->get_CurrentProfile(&fwProfile);
			}
         }
		  if(SUCCEEDED(ris))
		  {
			 Policy->Release();
             Mgr->Release();
		  }

//controlla se il firewall è abilitato

    ris = fwProfile->get_FirewallEnabled(&fwEnabled);

	 //spegne il firewall se acceso e lo accende se spento, per motivi di testing
	 //nel progetto si dovrebbe salvare l'informazione acceso/spento in un flag

	 if (fwEnabled != VARIANT_FALSE)
	 {
		 acceso=TRUE;
		 ris = Firewall_off(fwProfile);
         if (FAILED(ris))
             {
                 return 0;
             }
	 }
	 else
	 {
         ris = Firewall_on(fwProfile);
		 acceso=FALSE;
         if (FAILED(ris))
         {
            return 0;
		 }
     }

	 Deall(fwProfile, Policy, Mgr);
	 return ris;
}


HRESULT Firewall_on(IN INetFwProfile* Profilo)
{
	HRESULT ris = S_OK;
    ris  = Profilo->put_FirewallEnabled(VARIANT_TRUE);
    if (SUCCEEDED(ris))
        {
            return ris;
        }
	//messaggio di errore
	return 0;
}

HRESULT Firewall_off(IN INetFwProfile* Profilo)
{
	HRESULT ris = S_OK;
   ris  = Profilo->put_FirewallEnabled(VARIANT_FALSE);
    if (SUCCEEDED(ris))
        {
            return ris;
        }
	//messaggio di errore
	return 0;
}

void Deall(INetFwProfile* fwProfile, INetFwPolicy* Policy, INetFwMgr* Mgr)
{

    if (fwProfile != NULL)
    {  fwProfile->Release(); }

    if(Policy != NULL)
    { //Policy->Release(); 
	  Policy=NULL; }

    if(Mgr != NULL)
    { //Mgr->Release; 
	   Policy=NULL; }

    CoUninitialize();
}