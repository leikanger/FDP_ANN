//{ GNU GPL v.3
/***************************************************************************
*           auronNet - Implementation of event-driven spiking ANN          *
*                           -------------------                            *
* copyright            : (C) 2011 by Per R. Leikanger                      *
* email                : leikange@gmail.com                                *
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/
//}
#ifndef AXON_H_
#define AXON_H_

#include "../andreKildefiler/time.h"
#include "../andreKildefiler/main.h"
#include "../neuroElements/synapse.h"
#include "../neuroElements/auron.h"
//#include "../andreKildefiler/aktivitetsObj.h"


//Deklarasjoner:
class i_synapse;
class s_synapse;
class K_synapse;
class i_auron;
class s_auron;
class K_auron;

class s_axon;
class K_axon;

/* <<interface>> */
class i_axon : public timeInterface{
	// XXX XXX XXX Utsetter doCalculation() for alle axon: (definerer den til å gjøre ingenting her for å unngå at klassene under blir abstract..)
	virtual void doCalculation() {}
	
	// Desse overlagres i de modell-speisfikke elementa (s_axon og K_axon), for å gi mulighet til å kalle modellspesifikke funksjoner og variabler for s_axon/K_axon.
	i_auron* pElementOfAuron; 		 // Flytta til i_axon
	std::list<i_synapse*> pUtSynapser; // Flytta til i_axon

	//SLETT:
	virtual void SLETTtypeId(){ cout<<"Dette er i_axon\n"; }
	
//	protected:
//	inline void doTask();

	public:
	i_axon(std::string sKlasseNavn /*="dendrite"*/);
	~i_axon();
	
	
	
	friend void testFunksjon_slett(s_auron*);
	//friend std::ostream & operator<< (std::ostream & ut, axon );
	friend std::ostream & operator<< (std::ostream & ut, s_axon* ); //XXX XXX denne skal ha argument (..., i_axon* ); XXX
	friend int main(int, char**); //TODO SLETT

	friend class i_synapse;
//	friend class i_axon;
	friend class i_auron;
	friend class i_dendrite;

	friend class K_auron;
	friend class K_synapse;
	friend class K_axon;
	

	friend class s_synapse;
	friend class s_axon;
	friend class s_auron;
	friend class s_dendrite;
};

class s_axon : public i_axon{
	inline void doTask();

	// Desse overlagres i de modell-speisfikke elementa (s_axon og K_axon), for å gi mulighet til å kalle modellspesifikke funksjoner og variabler for s_axon/K_axon.
	s_auron* pElementOfAuron;
	std::list<s_synapse*> pUtSynapser;

	public:
	/**************************************************
	****     axon::axon()  - constructor           ****
	****        -pElementOfAuron(pAuronArg)        ****
	****        -timeInterface("axon")              ****
	****                                           ****
	**************************************************/
	s_axon(s_auron* pAuronArg);// : timeInterface("axon"), pElementOfAuron(pAuronArg) 		//XXX Bare mens eg utvikler i_auron. Kan fjærne etterkvart..
	//axon(const i_auron* p_iAuronArg);
	~s_axon();




	friend class i_auron;
	//friend class s_auron;
	friend class s_synapse;
	friend void testFunksjon_slett(s_auron*);
	//friend std::ostream & operator<< (std::ostream & ut, axon );
	friend std::ostream & operator<< (std::ostream & ut, s_axon* ); //XXX XXX denne skal ha argument (..., i_axon* ); XXX
	friend int main(int, char**); //TODO SLETT
};



/*****************************
**  TODO TODO 
**  Har oppdaga at det ikkje er naudsynt med oppdeling av nodene, med mindre man vil ha mulighet for auka spatiell oppløsning. 
**  Dersom denne koden skal brukes pragmatisk er det difor mulig å fjærne class K_axon (og føre alt over i K_auron). Dette gjelder også for K_dendrite..
*****************************/
class K_axon : public i_axon{
	inline void doTask();

	// Desse overlagres i de modell-speisfikke elementa (s_axon og K_axon), for å gi mulighet til å kalle modellspesifikke funksjoner og variabler for s_axon/K_axon.
	K_auron* pElementOfAuron;
	std::list<K_synapse*> pUtSynapser;

	
	protected:
	void doTransmission();

	public:
	K_axon(K_auron*);
	~K_axon();

	//SLETT:
	virtual void SLETTtypeId(){ cout<<"Dette er i_axon\n"; }

	//Skal vekk:
	friend class K_synapse;
	friend class K_auron;
};
#endif
// vim:fdm=marker:fmr=//{,//}
