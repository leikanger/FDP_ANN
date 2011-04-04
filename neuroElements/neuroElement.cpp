#include "synapse.h"
#include "../andreKildefiler/main.h"

/******************
*** 	auron   ***
******************/
auron::auron(std::string sNavn_Arg ="unnamed") : tidInterface("auron"), sNavn(sNavn_Arg)
{ //{
	cout<<"Lager auron med navn " <<sNavn <<endl;


	pOutputAxon = new axon(this); 						//  Husk destructor. Husk å også destruere dette axon (fra det frie lageret).
 	pInputDendrite = new dendrite(this); 				//  Husk destructor. Husk å også destruere dette axon (fra det frie lageret). 

 	// Må kanskje flytte ned her. XXX Dersom eg får rar utskrift på kjøringa av kjør.sh: 	ao_AuronetsAktivitet(this)
}  //}

auron::~auron()
{ //{
	cout<<"DESTRUCTOR: auron::~auron() : \t" <<sNavn <<"\t * * * * * * * * * * * * * * * * * * * * * * * * * \n";

	// Legge inn sjekk for om den er sletta allerede? XXX :
	delete pOutputAxon;
	delete pInputDendrite;

	cout<<"\nFERDIG destruert: auron \t" <<sNavn <<"\t X * X * X * X * X * X * X * X * X * X * X * X X * \n\n";
} //}

inline void auron::doTask()
{ //{ ... }
	//XXX Kva skal skje for auron::doTask() ?   // Initiere A.P. ? 
												// Lekkasje? 
												// `backpropagating action potential' (syn.plast.)? (type sjekke overføringstid for sy napser og gjennomføre syn.plast.)

	cout<<"FYRER Action Potential for neuron " <<sNavn <<".doTask(); \n";

	// auron::doTask() kalles kun fra dendrite, og kun dersom auronet fyrer. Kan dermed sende action potential i axon:
	tid::leggTilTask( pOutputAxon );

	// Evt legg til dendrite i tillegg, for å kjøre syn.plast. 
} //}

/*****************
***  synapse   ***
*****************/

synapse::synapse(auron* pPresynAuron_arg, auron* pPostsynAuron_arg, float fSynVekt_Arg /*=1*/, bool bInhibEffekt_Arg /*=false*/) 
			: 	tidInterface("synapse"), pPreNodeAxon(pPresynAuron_arg->pOutputAxon), pPostNodeDendrite(pPostsynAuron_arg->pInputDendrite), bInhibitorisk_effekt(bInhibEffekt_Arg), nSynaptiskVekt(fSynVekt_Arg)
{//{	
	cout<<"Kaller synapse::synapse(" <<pPreNodeAxon->pElementAvAuron->sNavn <<".pOutputAxon, " <<pPostNodeDendrite->pElementAvAuron->sNavn <<".pInputDendrite, ...)\n";

	pPreNodeAxon->pUtSynapser.push_back(this);
	pPostNodeDendrite->pInnSynapser.push_back(this);
	

	cout 	<<"\tCONSTRUCTOR : synapse::synapse(a*, a*) \tEtterpå får vi:\n" 
			<<(pPreNodeAxon) <<endl;

} //}

synapse::~synapse()
{ //{ ... }    --DESTRUCTOR  	TODO TEST: denne er utesta..
	bool bPreOk  = false;
	bool bPostOk = false;


	cout<<"DESTRUCTOR: synapse::~synapse() : \t";


	//TODO Sjekk ut std::find() istedenfor. Stroustrup anbefaler å bruke dette!.
	if( !bPreOk ){ 	//Redundant test. Kanskje eg skal skrive while(!bPreOk)?
		//fjærner seg sjølv fra prenode:
		for( std::list<synapse*>::iterator iter = pPreNodeAxon->pUtSynapser.begin(); iter != pPreNodeAxon->pUtSynapser.end() ; iter++ ){
			if( *iter == this ){
				cout<<"\t~( [" <<pPreNodeAxon->pElementAvAuron->sNavn <<"] -> "; 	// utskrift del 1
				(pPreNodeAxon->pUtSynapser).erase( iter );
				bPreOk = true;
				break;
			}
		}
	}
	if( !bPostOk ){
		//fjærner seg sjølv fra postnode:
		for( std::list<synapse*>::iterator iter = pPostNodeDendrite->pInnSynapser.begin(); iter != pPostNodeDendrite->pInnSynapser.end() ; iter++ ){
			if( *iter == this ){ 
				cout<<"[" <<pPostNodeDendrite->pElementAvAuron->sNavn <<"] )\t"; 										// utskrift del 2
				(pPostNodeDendrite->pInnSynapser).erase( iter );
				bPostOk = true;
				break;
			}
		}
	}

	if( (!bPreOk) || (!bPostOk) ){
		/// FEIL:
		std::cerr<<"\n\n\n\n\nFEIL FEIL FEIL!\nSjekk neuroEnhet.cpp #asdf250. Feil i synapse destruksjon. (~synapse )\n";
		std::cerr<<"I test om eg vart sletta  fra presyn. neuron og postsyn. neuron: failed\t\t" 
			<<"bPreOk (" <<pPreNodeAxon->pElementAvAuron->sNavn <<"):" <<bPreOk <<"  ->  bPostOk (" <<pPostNodeDendrite->pElementAvAuron->sNavn <<"): " <<bPostOk 
			<<"\n(Eg er ikkje sletta fra det aktuelle neuronet (dei med verdi 0)\n";
		std::cerr<<"Eg ligger mellom (neuron: presyn - postsyn): " <<pPreNodeAxon->pElementAvAuron->sNavn <<" - " <<pPostNodeDendrite->pElementAvAuron->sNavn <<endl;
		exit(-9);	
	}
	//cout<<"\tO.K.  -  syn. " <<pPreNodeAxon->pElementAvAuron->sNavn <<" til " <<pPostNodeDendrite->pElementAvAuron->sNavn <<" er ikkje lenger\n";
	cout<<endl;

} //}

inline void synapse::transmission()
{ //{
	cout<<"Overføring i synapse.\n";

	// Dersom synapsen har inhibitorisk effekt: send inhibitorisk signal (subtraksjon). Ellers: eksiter postsyn. auron.
	if( bInhibitorisk_effekt ){
 		pPostNodeDendrite->newInputSignal(  -nSynaptiskVekt );
	}else{
		pPostNodeDendrite->newInputSignal(   nSynaptiskVekt ); //TODO gjør nSynaptiskVekt om til nSynaptiskVekt
	}

	// plasserer all depol-endring i dendrite: Kaller dendrite::calculateLeakage()
	pPostNodeDendrite->calculateLeakage();
		


	// XXX Kalkulere syn.p.?

	cout<<" - - - - - - - - - - - - - - - - - - - legger til dendrite i arbeidsliste. - - - - - - - - - - - - - - - - - - - - - - - - - \n";
	// Legger til postsyn. dendrite i arbeidskø: Kva den skal gjøre (neste iter) er definert i dendrite::doTask();
	tid::leggTilTask( pPostNodeDendrite );
} //}
	
inline void synapse::doTask()
{ //{ .. }
	// SANN: Kjør synaptisk overføring:
	transmission();

	//Skal handteres i aktivitetsObj:
	// aktivitetsObj.kall-rette-funksjoner();
	// For SANN: summer input.
	// For KANN: oppdater kappa.
} //}



/*************
***  axon  ***
*************/
axon::axon(const auron* pAuronArg) : tidInterface("axon"), pElementAvAuron(pAuronArg)
{ //{ tanke er at axon må tilhøre eit auron. Difor auronpeiker.
	cout<<"\tlager axon\n";//for \tauron " <<pAuronArg->sNavn <<endl;		
} //}

axon::~axon()
{ //{
	cout<<"axon::~axon() : 	\t\t(tilhører auron " <<pElementAvAuron->sNavn <<")\n";
	//delete pElementAvAuron->pOutputAxon; // XXX LAGER HELVETE ? JEPP

	cout<<"SKRIV først ut axonet: " <<this <<"\n";

	// pUtSynapser inneholder bare peikere, så pUtSynapser.clear() vil ikkje føre til destruksjon av synapsene.
	// 		Sletter synapsene eksplisitt med delete-operatoren på alle element som list<synapse*> pUtSynapser) peiker på.
	while( !pUtSynapser.empty() ){
		delete (*pUtSynapser.begin()); //Kaller destruktoren til første gjenværende synapse. Dette fører også til at synapsa fjærnes fra pUtSynapser (og dendrite.pInnSynapser)
	}

} //}

inline void axon::doTask()
{ //{ // initierAksjonspotensial()
	cout<<"\n\n\n\n\n\nLegger inn alle outputsynapser i arbeidskø.\n\n";

	// For meir nøyaktig simulering av tid kan alle synaper få verdi for 'time lag' før fyring. No fokuserer eg heller på effektivitet. 
 	for( std::list<synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end(); iter++ )
	{ // Legger alle pUtSynapser inn i arbeidskø: (FIFO-kø)
		
		//tid::pTaskArbeidsKoe_List.push_back( (*iter) );
		// Legger til ut-synapser i tid::arbeidskø
		tid::leggTilTask( *iter );

		cout<<"\tinne i loop for å legge inn outputsynapser i arbeidskø. Mdl. av auron: " <<pElementAvAuron->sNavn <<" - - - - - - - - - - - - - - - \n";
		//TODO ordne dette:
		cout<<(*iter) <<endl;
	}
} //}




/**************************
*** 	dendrite 		***
**************************/

dendrite::dendrite( auron* pPostSynAuron_Arg ) : tidInterface("dendrite"), pElementAvAuron(pPostSynAuron_Arg)
{ //{
	cout<<"\tLager dendrite\n";// for \tauron " <<pElementAvAuron->sNavn <<endl;
} //}

inline void dendrite::newInputSignal( int nNewSignal_arg )
{ //{
	pElementAvAuron->nAktivitetsVariabel += nNewSignal_arg;
	cout<<"dendrite::newInputSignal( " <<nNewSignal_arg <<"); gir depol. :  " <<pElementAvAuron->nAktivitetsVariabel <<"\n";
} //}

inline void dendrite::calculateLeakage()
{ //{
	if( pElementAvAuron->ulTimestampForrigeInput != tid::getTid() )
	{
		// regner ut, og trekker fra lekkasje av depol til postsyn neuron.
	 	pElementAvAuron->nAktivitetsVariabel -= pow( LEKKASJEFAKTOR_FOR_DEPOL, (pElementAvAuron->ulTimestampForrigeInput - tid::getTid() ) );
	}
	// Dersom den allerede har regna ut lekkasje: ikkje gjør det igjen. Returner.
} //}

inline void dendrite::doTask()
{ //{ 
	// Kva skal dendrite::doTask() gjøre? 
	cout<<"\n\ndendrite.doTask(). Postsyn. depol (" <<pElementAvAuron->sNavn <<") etter overføring: " <<pElementAvAuron->nAktivitetsVariabel <<".\n\n";

	
	//TODO Først: gjennomfør lekkasje: pow(lekkasjeFaktor, [tid siden sist]) 

	// Dersom auron går over fyringsterskel: fyr A.P.
	if( pElementAvAuron->nAktivitetsVariabel > FYRINGSTERSKEL )
	{
		cout<<"Postsyn. kom over FYRINGSTERSKEL. Legger til i arbeidskø. - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n\n";
		
		// Legger til neste ledd i signal-path (soma == auron).
	 	tid::leggTilTask( pElementAvAuron );
	}
	

} //}
