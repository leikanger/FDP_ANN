/*
** PLAN: Skal lage en interface-variant av aktivitetsObj, og en subklasse for kvar av SANN og KANN. Det er kva aktivitetsObj som brukes som blir forskjellen mellom de.
**
**   Tenker først å lage UML over dette i helga, så progge til neste veke..
**
*/

#include <math.h>


#ifndef AKTIVITETSOBJ_H_
#define AKTIVITETSOBJ_H_

#include "main.h"
#include "tid.h"


/******************************************
***    class abstract_aktivitetsOvj     ***
***       - interface for aktivitetsObj.***
***                                     ***
***       Upåbegynnt                    ***
***                                     ***
******************************************/
class abstract_aktivitetsOvj{
	void tulleFunk(){}
};

//XXX Foreløpig lager eg bare denne for SANN. Heiletida i tankene at den skal lages <<interface>> av, og lage en for KANN også.

// Dersom nødvendig å ha fleire doTask() for auron: legg en av de her.. (I såfall treng eg å utvide til class aktivitetsObj : public tidInterface{ }
class spiking_aktivitetsObj : public abstract_aktivitetsOvj
{
	float depolarisasjon;
	unsigned long ulForrigeOppdatering_tidspkt;

	// aktivitetsObj skal tilhøre enkelte greiner av neuronet. Kan kanskje dermed ha det slik at medlemAv er peiker til tidInterface?
	const tidInterface* pAktivitetsObj_for_objekt;

	// Oppdater neuron. Lekkasje i "leaky integrator"-modellen av neuron.
	void updateDepol()
	{
		static unsigned long ulTidsstegSidenForrigeOppdatering;

		ulTidsstegSidenForrigeOppdatering =  tid::getTid()-ulForrigeOppdatering_tidspkt;

		double faktor = pow( LEKKASJEFAKTOR_FOR_DEPOL, ulTidsstegSidenForrigeOppdatering);
		// Gjennomfør lekkasje på depolarisasjonsverdien.
		depolarisasjon *= faktor;


	 	ulForrigeOppdatering_tidspkt = tid::getTid();

		cout<<"Oppdaterer aktivitetsObj: \tDet er " <<ulTidsstegSidenForrigeOppdatering <<" tidssteg siden sist. Dette gir faktor " <<faktor <<endl;
	}

	public:
	/******** Constructor *********/
	spiking_aktivitetsObj(tidInterface* pTidInterfaceObj_arg) : pAktivitetsObj_for_objekt(pTidInterfaceObj_arg){
		depolarisasjon = 0;
		ulForrigeOppdatering_tidspkt = tid::getTid();
	}

	const float getDepol(){
		return depolarisasjon;
	}
	inline void incomingSignal(float signalSize)
	{
		updateDepol();
		depolarisasjon += signalSize;
	}

	const void sendVidareSignal()
	{
		// Mest for kann:
	 	/* trenger stuktur for å sende ut signal for både neuron og dendritt. 
					- synapse  sender til dendritt
					- dendritt sender vidare til neuronet
					- neuronet sender vidare til axon (aksjonspotensial)
					- axon 		sender til synapse.

		Trenger egentlig bare Kappa for dendritt og neuron. Axon og synapse trenger bare perioden for å gi postsyn dendritt mulighet til å kalkulere kappa.
		*/

		int slett = 0;
		slett ++;

	}

	friend void testFunksjon_slett(auron* );
};

#endif
