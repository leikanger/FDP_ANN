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
#ifndef DENDRITE_H_
#define DENDRITE_H_


#include "../andreKildefiler/time.h"
#include "../andreKildefiler/main.h"
#include "../neuroElements/synapse.h"
#include "../neuroElements/auron.h"
//#include "../andreKildefiler/aktivitetsObj.h"


//Deklarasjoner:
class i_synapse;
class s_synapse;
class s_auron;

/*****************************************************************
** class dendritt 			-  	Notater:
** 	
** 		- dendritt må ha aktivitetsVariabel.
**
*****************************************************************/
class i_dendrite : public timeInterface{
	i_auron* pElementAvAuron;
	std::list<i_synapse*> pInnSynapser;

 	virtual inline void doTask() =0;
	// XXX XXX XXX Utsetter doCalculation() for alle dendrite: (definerer den til å gjøre ingenting her for å unngå at klassene under blir abstract..)
	virtual void doCalculation() {}

	virtual inline void newInputSignal( double ) =0;
	//virtual inline void feedbackToDendrite() =0;

	public:
	i_dendrite(std::string sNavn /*="dendrite"*/);
	~i_dendrite();

	friend int main(int, char**);
	friend std::ostream & operator<< (std::ostream & ut, s_axon* pAxonArg );

	friend class i_synapse;
	friend class i_axon;
	friend class i_auron;
	//friend class i_dendrite;

	friend class s_synapse;
	friend class s_axon;
	friend class s_auron;
	friend class s_dendrite;

	friend class K_synapse;
	friend class K_axon;
	friend class K_auron;
	friend class K_dendrite;
};


class s_dendrite : public i_dendrite{
	inline void doTask();
	inline void feedbackToDendrite();

	// Overlagrer i_dendrite::pElementAvAuron og i_dendrite::pInnSynapser, slik at desse blir modellspesifikke.
	// 	Ligger også i i_dendrite, slik at i_dendrite* også kan kalle pElementAvAuron og pInnSynapser.
	s_auron* pElementAvAuron;
	std::list<s_synapse*> pInnSynapser;

	inline void newInputSignal( double /*nNewSignal*/ );
	inline void calculateLeakage(); 		//Bare for SANN

	bool bBlockInput_refractionTime; 		//Bare for SANN 	//Blokkere input når refraction period eller når depol er over terskel.
												// XXX Flytta til s_auron.

	public:
	//Constructor: 	
	s_dendrite( s_auron* pPostSynAuron_Arg ); // : pElementAvAuron(pPostSynAuron_Arg)  		Definisjon i neuroElement.cpp
	~s_dendrite();


	//TODO LEGG TIL dendrite::leggTilSynapse(synapse*) ! Gjør saker lettere, og bedre innkapsling!
	
	//Destructor:
	/* kommenterer ut mens s_synapser ikkje er laga enda.. //{
	~sDendritt()
	{
		// destruerer alle innsynapser:
	 	while( !pInnSynapser.empty() )
		{
	 		cout<<"~dendritt: fjerner innsyn fra " <<pInnSynapser.back()->pPreNode->sNavn <<" til " <<pInnSynapser.back()->pPostNode->sNavn <<"( " <<pInnSynapser.size() <<" synapser igjen)."	<<endl;
			pInnSynapser.pop_back(); 	//pop_back() fjærner siste ledd i std::vector. Kaller også destructor for dette elementet.
			// ~synapse tar hånd om (fjærner) presynaptisk kobling til denne synapsen. TODO
		}
	}
	*/ //}

	friend class s_auron;
	friend class s_axon;
	// Skal vekk: ? :
	friend class s_synapse;
	//friend std::ostream & operator<< (std::ostream & ut, axon  );
	friend std::ostream & operator<< (std::ostream & ut, s_axon* );

	friend int main(int, char**); //TODO SLETT
};

class K_dendrite : public i_dendrite{
 	inline void doTask();
	inline void feedbackToDendrite();

	// Overlagrer i_dendrite::pElementAvAuron og i_dendrite::pInnSynapser, slik at desse blir modellspesifikke.
	// 	Ligger også i i_dendrite, slik at i_dendrite* også kan kalle pElementAvAuron og pInnSynapser.
	K_auron* pElementAvAuron;
	std::list<K_synapse*> pInnSynapser;
	
	// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
	inline double recalculateKappa()
	{
		double dKappa_temp = 0;
	 	std::list<K_synapse*>::iterator iter = pInnSynapser.begin();
		while(iter != pInnSynapser.end() )
		{
	 		//dKappa_temp += iter->dPresynPeriodINVERSE*dSynapticWeight;
		}
		return dKappa_temp;
	}

	
	inline void newInputSignal(double);

	public:
	K_dendrite( K_auron* );
	~K_dendrite();

	// Skal vekk: ? :
	friend class K_synapse;
	friend class K_auron;
};

#endif
// vim:fdm=marker:fmr=//{,//}
