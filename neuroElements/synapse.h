
#ifndef SYNAPSE_H_
#define SYNAPSE_H_

#include "../neuroElements/auron.h"
//#include "../andreKildefiler/main.h"
#include "../andreKildefiler/tid.h"

//#include "../andreKildefiler/aktivitetsObj.h"
class axon;

class synapse : public tidInterface{
	
	auron* pPreNode;
	auron* pPostNode;

	const bool bInhibitorisk_effekt;
	float fSynaptiskVekt;


	void doTask(){
		//Handteres i aktivitetsObj:
		// For SANN: summer input.
		// For KANN: oppdater kappa.
		//aktivitetsObj. TODO
	}

	public:
	//synapse() : tidInterface("synapse"), bInhibitorisk_effekt(false){} 		//XXX SKAL VEKK XXX

	// Lag også axon.nySynapse(auron* pOutputAuron){  new synapse(this, pOutputAuron); } XXX
	synapse(auron* pPresynAuron_arg, auron* pPostsynAuron_arg, bool bInhibEffekt_Arg =false, float fSynVekt_Arg =1) ; // : pPreNode(), pPostNode(), bInhibitorisk_effekt(), fSynaptiskVekt()
	//XXX hugs destructor


	//friend std::ostream & operator<< (std::ostream & ut, synapse*); //TODO
	friend std::ostream & operator<< (std::ostream & ut, axon axonArg );
};

/* Kommentert ut: 
//{ constructor:
synapse::synapse(auron* pPresynAuron_arg, auron* pPostsynAuron_arg, bool argInhibitorisk_effekt =false, float arg_fVekt =1) :
		bInhibitorisk_effekt(argInhibitorisk_effekt),
	 	fSynaptiskVekt( arg_fVekt ),
		pPreNode(pPresynAuron_arg), pPostNode(pPostsynAuron_arg)
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


#endif
