
#ifndef DENDRITE_H_
#define DENDRITE_H_


#include "../andreKildefiler/tid.h"
#include "../andreKildefiler/main.h"
#include "../neuroElements/synapse.h"
#include "../neuroElements/auron.h"
//#include "../andreKildefiler/aktivitetsObj.h"


//Deklarasjoner:
//class synapse;
//class auron;
//class tidInterface;

/*****************************************************************
** class dendritt 			-  	Notater:
** 	
** 		- dendritt må ha aktivitetsVariabel.
**
*****************************************************************/
class dendrite : public tidInterface{
	inline void doTask()
	{ // { 
		cout<<"\n\n\n\n\n\nJADA DENTRITE.doTask()\n\n\n\n";

	} // }

	const auron* pElementAvAuron;
	std::list<synapse*> pInnSynapser;

	public:
	//Constructor: 	
	dendrite(auron* pPostNodeArg) : pElementAvAuron(pPostNodeArg){} //constuctor
	
	//Destructor:
	/* kommenterer ut mens synapser ikkje er laga enda.. //{
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

	friend class auron;
};

/* //{ UTKOMMENTERT




};

	*/ //}


#endif