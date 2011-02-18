/*
 * iSynapse.h
 *
 *  Created on: 14. feb. 2011
 *      Author: kybpc1036
 */

#ifndef ISYNAPSE_H_
#define ISYNAPSE_H_


/*
 * class iSynapse 		interface class
 *
 * Interface for the classes
 * 		s_synapse 	synapse for spiking ANN
 * 		k_synapse 	synapse for kappa-based ANN
 * 		synSkilleElement
 *
 */
class iSynapse{
	/* public variable*/
	auron* pPreNode;
	auron* pPostNode;

	/* public functions*/
	//iSynapse();
	//iSynapse(auron* pPre, auron* pPost);
	// ~iSynapse();

protected:
	/* protected variables */
	const bool inhibEffekt;
	unsigned long timestampForrigeOppdatering;

	/* protected functions */
	virtual void synPlast(float postSynDepol) =0;
	virtual void oppdaterSyn() 						=0;
	virtual void regnUtOgOverfoer() 				=0;

	friend class auron;
	friend class synSkilleElement;
};


class tidsSkilleElement : public iSynapse{
	// Arbeidsliste -k�
	static list<synapse*> pNesteSynapseUtregningsKoe;

	void synPlast(float){/*tom*/};
	void regnUtOgOverfoer();

};




#endif /* ISYNAPSE_H_ */
