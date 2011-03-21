//XXX Veit ikkje kvifor, men med neste linje avkommentert får eg helvete av feilmeldinger..
#ifndef SYNAPSE_H_
#include "synapse.h"
#endif



synapse::synapse(auron* pPresynAuron_arg, auron* pPostsynAuron_arg, bool bInhibEffekt_Arg =false, float fSynVekt_Arg =1) 
			: 	pPreNode(pPresynAuron_arg), pPostNode(pPostsynAuron_arg), bInhibitorisk_effekt(bInhibEffekt_Arg), fSynaptiskVekt(fSynVekt_Arg)
{	
	//cout<<"Inne i constructor for synapse mellom presyn. auron " <<pPreNode->sNavn <<" og " <<pPostNode->sNavn <<endl;
}

/*

// constructor:
//{ Kommentert ut
synapse::synapse(auron* pPresynAuron, auron* pPostsynAuron, bool inhibitoriskEffekt =false, float vekt =1) :
		bInhibitorisk_effekt(argInhibitorisk_effekt),
	 	fSynaptiskVekt( vekt ),
		pPreNode(pPreN_arg), pPostNode(pPostN_arg)
{	//{
	pPreNode->pUtSynapser.push_back(  this );
	pPostNode->pInnSynapser.push_back(this );
	

	/ * //{ Utskrift til fil for å lage plot i etterkant. (Kommentert ut foreløpig)
	// lag ei .oct - fil, og gjør klar for å kjøres i octave:
	std::ostringstream tempFilAdr;
	tempFilAdr<<"./datafiler_for_utskrift/synapse_" <<pPreNode->navn <<"-"  <<pPostNode->navn ;
	if(bInhibitorisk_effekt){ tempFilAdr<<"_inhi"; }
	else{ 			  tempFilAdr<<"_eksi"; }
	tempFilAdr<<".oct";

	std::string tempStr( tempFilAdr.str() );
	// trenger c-style string for open():
	utskriftsFilLogg.open( tempStr.c_str() );
	
	utskriftsFilLogg<<"data=[\n";
	utskriftsFilLogg.flush();
	* / //}
	
}  	//}
//}
*/
