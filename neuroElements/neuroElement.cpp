#include <sstream> //For skriving til fil: logg.

#include "synapse.h"
#include "../andreKildefiler/main.h"

/******************
*** 	auron   ***
******************/
auron::auron(std::string sNavn_Arg /*="unnamed"*/, int nStartDepol /*=0*/) : timeInterface("auron"), sNavn(sNavn_Arg), nAktivitetsVariabel(nStartDepol)
{ //{
	cout<<"Lager auron med navn " <<sNavn <<endl;


	pOutputAxon = new axon(this); 						//  Husk destructor. Husk å også destruere dette axon (fra det frie lageret).
 	pInputDendrite = new dendrite(this); 				//  Husk destructor. Husk å også destruere dette axon (fra det frie lageret). 


	// lag ei .oct - fil, og gjør klar for å kjøres i octave:
	//{ Utskrift til logg. LOGG-initiering
	std::ostringstream tempFilAdr;
	tempFilAdr<<"./datafiler_for_utskrift/auron" <<sNavn <<"-aktivitetsVar" <<".oct";

	std::string tempStr( tempFilAdr.str() );

	// trenger c-style string for open():
	aktivitetsVar_loggFil.open( tempStr.c_str() );
	aktivitetsVar_loggFil<<"data=[" <<time_class::getTid() <<"\t" <<nAktivitetsVariabel <<";\n";
	aktivitetsVar_loggFil.flush();

	//}
}  //}

auron::~auron()
{ //{
	cout<<"DESTRUCTOR: auron::~auron() : \t" <<sNavn <<"\t * * * * * * * * * * * * * * * * * * * * * * * * * \n";

	// Legge inn sjekk for om den er sletta allerede? XXX :
	delete pOutputAxon;
	delete pInputDendrite;

	//Kalkulerer lekkasje før utskrift av aktivitetsVar.
	cout<<"DESTRUCTOR: calculateLeakage();\n";
	pInputDendrite->calculateLeakage();

	//{ Rett slutt på utskriftsfil-logg:
	// no er data slik: [time, synWeight ] i synapse-logg
	aktivitetsVar_loggFil<<time_class::getTid() <<"\t" <<nAktivitetsVariabel <<"];\n"
					<<"plot( data([1:end],1), data([1:end],2), \";Activity variable;\");\n"

					<<"title \"Activity variable for auron " <<sNavn <<"\"\n"
					<<"xlabel Time\n" <<"ylabel \"Activity variable\"\n"
					//<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
					//<<"print(\'eps_" <<pPreNodeAxon->pElementAvAuron->sNavn <<"->" <<pPostNodeDendrite->pElementAvAuron->sNavn <<".eps\', \'-deps\');\"\n"
					<<"sleep(9); ";
	aktivitetsVar_loggFil.close();
	//}



	cout<<"\nFERDIG destruert: auron \t" <<sNavn <<"\t X * X * X * X * X * X * X * X * X * X * X * X X * \n\n";
} //}


/*****************
***  synapse   ***
*****************/

synapse::synapse(auron* pPresynAuron_arg, auron* pPostsynAuron_arg, float fSynVekt_Arg /*=1*/, bool bInhibEffekt_Arg /*=false*/) 
			: 	timeInterface("synapse"), pPreNodeAxon(pPresynAuron_arg->pOutputAxon), pPostNodeDendrite(pPostsynAuron_arg->pInputDendrite), bInhibitorisk_effekt(bInhibEffekt_Arg), uSynaptiskVekt(fSynVekt_Arg)
{//{	
	cout<<"Kaller synapse::synapse(" <<pPreNodeAxon->pElementAvAuron->sNavn <<".pOutputAxon, " <<pPostNodeDendrite->pElementAvAuron->sNavn <<".pInputDendrite, ...)\n";

	pPreNodeAxon->pUtSynapser.push_back(this);
	pPostNodeDendrite->pInnSynapser.push_back(this);
	

	cout 	<<"\tCONSTRUCTOR : synapse::synapse(a*, a*) \tEtterpå får vi:\n" 
			<<(pPreNodeAxon) <<endl;



	// lag ei .oct - fil, og gjør klar for å kjøres i octave:
	//{ Utskrift til logg. LOGG-initiering
	std::ostringstream tempFilAdr;
	tempFilAdr<<"./datafiler_for_utskrift/synapse_" <<pPresynAuron_arg->sNavn <<"-"  <<pPostsynAuron_arg->sNavn ;
	if(bInhibitorisk_effekt){ tempFilAdr<<"_inhi"; }
	else{ 			  tempFilAdr<<"_eksi"; }
	tempFilAdr<<".oct";

	std::string tempStr( tempFilAdr.str() );

	// trenger c-style string for open():
	synWeight_loggFil.open( tempStr.c_str() );
	synWeight_loggFil<<"data=[\n";
	synWeight_loggFil.flush();

	//}


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


	//{ Rett slutt på utskriftsfil-logg:

	// no er data slik: [tid, synWeight ] i synapse-logg
	synWeight_loggFil<<"];\n"
					<<"plot( data([1:end],1), data([1:end],2), \";Synaptic weight;\");\n"

					<<"title \"Synaptic weight for synapse: " <<pPreNodeAxon->pElementAvAuron->sNavn <<" -> " <<pPostNodeDendrite->pElementAvAuron->sNavn <<"\"\n"
					<<"xlabel Time\n" <<"ylabel syn.w.\n"
					//<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
					//<<"print(\'eps_" <<pPreNodeAxon->pElementAvAuron->sNavn <<"->" <<pPostNodeDendrite->pElementAvAuron->sNavn <<".eps\', \'-deps\');\"\n"
					<<"sleep(9); ";
	synWeight_loggFil.close();
	//}

} //}

/************ Denne kan vel egentlig ligge i synapse::doTask() ****************/
inline void synapse::transmission()
{ //{
	cout<<"Overføring i synapse.\n";

	// plasserer all depol-endring i dendrite: Kaller dendrite::calculateLeakage()
	pPostNodeDendrite->calculateLeakage();

	// Dersom synapsen har inhibitorisk effekt: send inhibitorisk signal (subtraksjon). Ellers: eksiter postsyn. auron.
	// Dendrite lagrer tidspunk for overføring.
	if( bInhibitorisk_effekt ){
 		pPostNodeDendrite->newInputSignal(  -uSynaptiskVekt );
	}else{
		pPostNodeDendrite->newInputSignal(   uSynaptiskVekt ); //TODO gjør uSynaptiskVekt om til uSynaptiskVekt
	}

		


	// XXX Kalkulere syn.p.?

	//cout<<" - - - - - - - - - - - - - - - - - - - legger til dendrite i arbeidsliste. - - - - - - - - - - - - - - - - - - - - - - - - - \n";
	// Legger til postsyn. dendrite i arbeidskø: Kva den skal gjøre (neste iter) er definert i dendrite::doTask();
	time_class::leggTilTask( pPostNodeDendrite );


	// TODO: synaptisk plastisitet: HER.
	//{ Loggfører syn.weight
	synWeight_loggFil 	<<"\t" <<time_class::getTid() <<"\t" <<uSynaptiskVekt
						<<" ;   \t#Synpaptic weight\n" ;
	synWeight_loggFil.flush();
			//} 

	// Logg for aktivitetsVar for postsyn auron skjer i pPostNodeDendrite->newInputSignal(-);
} //}
	


/*************
***  axon  ***
*************/
axon::axon(const auron* pAuronArg) : timeInterface("axon"), pElementAvAuron(pAuronArg)
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




/**************************
*** 	dendrite 		***
**************************/

dendrite::dendrite( auron* pPostSynAuron_Arg ) : timeInterface("dendrite"), pElementAvAuron(pPostSynAuron_Arg)
{ //{
	cout<<"\tLager dendrite\n";// for \tauron " <<pElementAvAuron->sNavn <<endl;
} //}

dendrite::~dendrite()
{ //{
	cout<<"\tDestruerer dendrite\n";

	// Destruerer alle innsynapser.
	while( !pInnSynapser.empty() ){
	 	delete (*pInnSynapser.begin() );
	}
} //}

inline void dendrite::newInputSignal( int nNewSignal_arg )
{ //{
	pElementAvAuron->nAktivitetsVariabel += nNewSignal_arg;
	cout<<"dendrite::newInputSignal( " <<nNewSignal_arg <<"); gir depol. :  " <<pElementAvAuron->nAktivitetsVariabel <<"\n";

	pElementAvAuron->ulTimestampForrigeInput = time_class::getTid();

	// Skriver til log for aktivitetsVar:
	pElementAvAuron->aktivitetsVar_loggFil 	<<time_class::getTid() <<"\t" <<pElementAvAuron->nAktivitetsVariabel <<"; \t #Activity variable\n" ;
	pElementAvAuron->aktivitetsVar_loggFil.flush();
} //}

inline void dendrite::calculateLeakage()
{ //{
	
	int slettDebugGammelDepolverdi = pElementAvAuron->nAktivitetsVariabel;

	if( pElementAvAuron->ulTimestampForrigeInput != time_class::getTid() )
	{
		// regner ut, og trekker fra lekkasje av depol til postsyn neuron.
		static unsigned long sulTidSidenSist;
		sulTidSidenSist = time_class::getTid()-pElementAvAuron->ulTimestampForrigeInput;

		cout<<"XXX Tid siden sist: " <<sulTidSidenSist <<endl;

	 	pElementAvAuron->nAktivitetsVariabel *= (double)pow( LEKKASJEFAKTOR_FOR_DEPOL, sulTidSidenSist );
		
		cout 	<<"\n\n\t\t\t\t\tLEKKASJEfaktor: " <<(double)pow( LEKKASJEFAKTOR_FOR_DEPOL, sulTidSidenSist ) <<" [gammel => ny depol.]: [" <<slettDebugGammelDepolverdi <<"=>" <<pElementAvAuron->nAktivitetsVariabel
				<<"]. (for auron " <<pElementAvAuron->sNavn <<")\n";
	}
	// Dersom den allerede har regna ut lekkasje: ikkje gjør det igjen. Returner.
} //}





/*********************************************************
*****  			doTask() -- samla på en plass.		 *****
*********************************************************/
inline void auron::doTask()
{ //{ ... }
	//XXX Kva skal skje for auron::doTask() ?   // Initiere A.P. ? 
												// Lekkasje? 
												// `backpropagating action potential' (syn.plast.)? (type sjekke overføringstid for sy napser og gjennomføre syn.plast.)

	
	/************************** ???
	** Genialt! Gjennomfør lekkasje kvar gang auron::doTask() kalles: Kvar gang eit A.P. er bestillt, gjennomføres lekkasje.
	************************/// ???
		//Skal bl.a. oppdatere DA-nivå i dendritter, osv.
		//oppdaterNeuron() 
	/* Foreløpig ligger det i dendrite.calculateLeakage() som kalles fra synapse::transmission() som kalles fra synapse::doTask()*/


	cout<<"\t" <<sNavn <<".doTask()\tFYRER Action Potential for neuron " <<sNavn <<".\t\t| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | \ttid: " <<time_class::getTid() <<"\n";

	//Axon hillock: send aksjonspotensial 	-- innkapsling gir at axon skal ta hånd om all output. // bestiller at axon skal fyre NESTE tidsiterasjon. Simulerer tidsdelay i axonet.
	time_class::leggTilTask( pOutputAxon );

	// Evt legg til dendrite i tillegg, for å kjøre syn.plast. 

	// TODO 'Refraction time' skal være implementert i overføring-funk (i sy napse)..
	if( ulTimestampForrigeFyring == time_class::ulTidsiterasjoner )
	{
		cout<<"\n\n************************\nFeil?\nTo fyringer på en iterasjon? \nFeilmelding au#103 @ auron.h\n************************\n\n";
		return;
	}

	// Registrerer fyringstid (for feisjekk (over) osv.)
	ulTimestampForrigeFyring = time_class::ulTidsiterasjoner;

	//Resetter depol.verdi 
	nAktivitetsVariabel = 0;
	
	// Skriver til log for aktivitetsVar:
	aktivitetsVar_loggFil 	<<time_class::getTid() <<"\t" <<nAktivitetsVariabel <<"; \t #Activity variable\n" ;
	aktivitetsVar_loggFil.flush();
		

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
inline void axon::doTask()
{ //{ // initierAksjonspotensial()
	cout<<"Legger inn alle outputsynapser i arbeidskø. Mdl. av auron: " <<pElementAvAuron->sNavn <<" - - - - - - - - - - - - - - - \n";

	// For meir nøyaktig simulering av tid kan alle synaper få verdi for 'time lag' før fyring. No fokuserer eg heller på effektivitet. 
 	for( std::list<synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end(); iter++ )
	{ // Legger alle pUtSynapser inn i arbeidskø: (FIFO-kø)
		
		//time_class::pTaskArbeidsKoe_List.push_back( (*iter) );
		// Legger til ut-synapser i time_class::arbeidskø
		time_class::leggTilTask( *iter );
	}
} //}
inline void dendrite::doTask()
{ //{ 
	// Kva skal dendrite::doTask() gjøre? 
	cout<<pElementAvAuron->sNavn <<"->[dendrite]::doTask(). Postsyn. depol (" <<pElementAvAuron->sNavn <<") etter overføring: " <<pElementAvAuron->nAktivitetsVariabel <<".\n";

	
	//TODO Først: gjennomfør lekkasje: pow(lekkasjeFaktor, [tid siden sist]) 

	// Dersom auron går over fyringsterskel: fyr A.P.
	if( pElementAvAuron->nAktivitetsVariabel > FYRINGSTERSKEL )
	{
		//cout<<"Postsyn. kom over FYRINGSTERSKEL.\nLegger til i arbeidskø.\n\n";
		
		// Legger til neste ledd i signal-path (soma == auron).
	 	time_class::leggTilTask( pElementAvAuron );
	}
	

} //}



