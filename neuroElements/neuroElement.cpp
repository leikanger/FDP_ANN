
#include <sstream> //For skriving til fil: logg.

#include "synapse.h"
#include "../andreKildefiler/main.h"



/*************************************************************
****** 													******
******  		CONTRUCTORS OG DESCTRUCTORS 			******
****** 													******
*************************************************************/

/**
*** 	i_auron   
***********************/

// i_auron
i_auron::i_auron(std::string sNavn_Arg /*="unnamed"*/, int nStartDepol /*=0*/) : timeInterface("i_auron"), nAktivitetsVariabel(nStartDepol), sNavn(sNavn_Arg)
{ //{
	cout<<"CONSTRUCTOR: Lager auron med navn " <<sNavn <<endl;

	pOutputAxon = new axon(this);

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
i_auron::~i_auron()
{ //{
	cout<<"DESTRUCTOR: \ti_auron::~i_auron() : \t" <<sNavn <<"\t * * * * * * * * * * * * * * * * * * * * * * * * * \n";

	delete pOutputAxon;
	
	//{ Rett slutt på utskriftsfil-logg:
	// no er data slik: [time, synWeight ] i synapse-logg
	aktivitetsVar_loggFil<<time_class::getTid() <<"\t" <<nAktivitetsVariabel <<"];\n"
					<<"plot( data([1:end],1), data([1:end],2), \"@;Activity variable;\");\n"

					<<"title \"Activity variable for auron " <<sNavn <<"\"\n"
					<<"xlabel Time\n" <<"ylabel \"Activity variable\"\n"
					//<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
					//<<"print(\'eps_" <<pPreNodeAxon->pElementAvAuron->sNavn <<"->" <<pPostNodeDendrite->pElementAvAuron->sNavn <<".eps\', \'-deps\');\"\n"
					<<"sleep(9); ";
	aktivitetsVar_loggFil.close();
	//}
} //}
/*** s_auron ***/
s_auron::s_auron(std::string sNavn_Arg /*="unnamed"*/, int nStartDepol /*=0*/) : i_auron(sNavn_Arg, nStartDepol){
	ulTimestampForrigeFyring = time_class::getTid();
	ulTimestampForrigeInput  = time_class::getTid();
	// Osv.
	
	//pOutputAxon = new axon(this); LIGGER i class i_auron
 	pInputDendrite = new s_dendrite(this); //	Skal ligge i s_auron og k_auron


} 	
s_auron::~s_auron()
{
	cout<<"DESTRUCTOR: s_auron::~s_auron() : \t" <<sNavn <<"\t * * * * * * * * * * * * * * * * * * * * * * * * * \n";
	
	//delete pOutputAxon;  // OBS Flytta til i_auron.
	delete pInputDendrite; //	Skal ligge i både s_auron og k_auron


	//Kalkulerer lekkasje før utskrift av aktivitetsVar.
	//cout<<"DESTRUCTOR: TODO \t TODO \t calculateLeakage();\n";
	//pInputDendrite->calculateLeakage();
	
	//cout<<"\nFERDIG destruert: s_auron \t" <<sNavn <<"\t X * X * X * X * X * X * X * X * X * X * X * X X * \n\n";
	
	//Etter ~s_auron() kalles ~i_auron()

	 
}



/**
***  s_synapse
*****************/
s_synapse::s_synapse(s_auron* pPresynAuron_arg, s_auron* pPostsynAuron_arg, float fSynVekt_Arg /*=1*/, bool bInhibEffekt_Arg /*=false*/) 
			: 	timeInterface("s_synapse"), pPreNodeAxon(pPresynAuron_arg->pOutputAxon), pPostNodeDendrite(pPostsynAuron_arg->pInputDendrite), bInhibitorisk_effekt(bInhibEffekt_Arg), uSynaptiskVekt(fSynVekt_Arg)
{//{	
	// XXX 07.04 SEGFAULTEN FRA pElementAvAuron -- kall? 
	cout<<"OK starter test\n";
	//cout<<pPostNodeDendrite->pElementAvAuron->sNavn <<endl; 		XXX HER ER SEGFAULT!
	cout<<pPostsynAuron_arg->sNavn <<endl;
	cout<<"OK-jeah\n";
	cout<<"Kaller s_synapse::s_synapse(" <<pPreNodeAxon->pElementAvAuron->sNavn <<".pOutputAxon, " <<pPostNodeDendrite->pElementAvAuron->sNavn <<".pInputDendrite, ...)\n";

	pPreNodeAxon->pUtSynapser.push_back(this);
	pPostNodeDendrite->pInnSynapser.push_back(this);
	

	cout 	<<"\tCONSTRUCTOR : s_synapse::s_synapse(a*, a*) \tEtterpå får vi:\n" 
			<<(pPreNodeAxon) <<endl;



	// lag ei .oct - fil, og gjør klar for å kjøres i octave:
	//{ Utskrift til logg. LOGG-initiering
	std::ostringstream tempFilAdr;
	tempFilAdr<<"./datafiler_for_utskrift/s_synapse_" <<pPresynAuron_arg->sNavn <<"-"  <<pPostsynAuron_arg->sNavn ;
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
s_synapse::~s_synapse()
{ //{ ... }    --DESTRUCTOR  	TODO TEST: denne er utesta..
	bool bPreOk  = false;
	bool bPostOk = false;


	cout<<"DESTRUCTOR: s_synapse::~s_synapse() : \t";


	//TODO Sjekk ut std::find() istedenfor. Stroustrup anbefaler å bruke dette!.
	if( !bPreOk ){ 	//Redundant test. Kanskje eg skal skrive while(!bPreOk)?
		//fjærner seg sjølv fra prenode:
		for( std::list<s_synapse*>::iterator iter = pPreNodeAxon->pUtSynapser.begin(); iter != pPreNodeAxon->pUtSynapser.end() ; iter++ ){
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
		for( std::list<s_synapse*>::iterator iter = pPostNodeDendrite->pInnSynapser.begin(); iter != pPostNodeDendrite->pInnSynapser.end() ; iter++ ){
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
		std::cerr<<"\n\n\n\n\nFEIL FEIL FEIL!\nSjekk neuroEnhet.cpp #asdf250. Feil i s_synapse destruksjon. (~s_synapse )\n";
		std::cerr<<"I test om eg vart sletta  fra presyn. neuron og postsyn. neuron: failed\t\t" 
			<<"bPreOk (" <<pPreNodeAxon->pElementAvAuron->sNavn <<"):" <<bPreOk <<"  ->  bPostOk (" <<pPostNodeDendrite->pElementAvAuron->sNavn <<"): " <<bPostOk 
			<<"\n(Eg er ikkje sletta fra det aktuelle neuronet (dei med verdi 0)\n";
		std::cerr<<"Eg ligger mellom (neuron: presyn - postsyn): " <<pPreNodeAxon->pElementAvAuron->sNavn <<" - " <<pPostNodeDendrite->pElementAvAuron->sNavn <<endl;
		exit(-9);	
	}
	//cout<<"\tO.K.  -  syn. " <<pPreNodeAxon->pElementAvAuron->sNavn <<" til " <<pPostNodeDendrite->pElementAvAuron->sNavn <<" er ikkje lenger\n";
	cout<<endl;


	//{ Rett slutt på utskriftsfil-logg:

	// no er data slik: [tid, synWeight ] i s_synapse-logg
	synWeight_loggFil<<"];\n"
					<<"plot( data([1:end],1), data([1:end],2), \";Synaptic weight;\");\n"

					<<"title \"Synaptic weight for s_synapse: " <<pPreNodeAxon->pElementAvAuron->sNavn <<" -> " <<pPostNodeDendrite->pElementAvAuron->sNavn <<"\"\n"
					<<"xlabel Time\n" <<"ylabel syn.w.\n"
					//<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
					//<<"print(\'eps_" <<pPreNodeAxon->pElementAvAuron->sNavn <<"->" <<pPostNodeDendrite->pElementAvAuron->sNavn <<".eps\', \'-deps\');\"\n"
					<<"sleep(9); ";
	synWeight_loggFil.close();
	//}

} //}

/**
***  axon
****************/
axon::axon(i_auron* pAuronArg) : timeInterface("axon"), pElementAvAuron(pAuronArg)
{ //{ tanke er at axon må tilhøre eit auron. Difor auronpeiker.
	cout<<"\tlager axon\n";//for \tauron " <<pAuronArg->sNavn <<endl;		
} //}
/*
axon::axon(const i_auron* p_iAuronArg) : timeInterface("axon"), pElementAvAuronInterface(p_iAuronArg)
{ //{ tanke er at axon må tilhøre eit auron. Difor auronpeiker.
	cout<<"\tlager axon\n";//for \tauron " <<pAuronArg->sNavn <<endl;		
} //}
*/
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

/**
*** 	s_dendrite
**************************/
s_dendrite::s_dendrite( s_auron* pPostSynAuron_Arg ) : timeInterface("s_dendrite"), pElementAvAuron(pPostSynAuron_Arg), bBlokkerInput_refractionTime(false)
{ //{
	cout<<"\tLager s_dendrite\n";// for \tauron " <<pElementAvAuron->sNavn <<endl;
} //}
s_dendrite::~s_dendrite()
{ //{
	cout<<"\tDestruerer s_dendrite\n";

	// Destruerer alle innsynapser.
	while( !pInnSynapser.empty() ){
	 	delete (*pInnSynapser.begin() );
	}
} //}





/*************************************************************
****** 													******
******  		ANDRE FUNKSJONER:           			******
****** 													******
*************************************************************/

inline void s_dendrite::newInputSignal( int nNewSignal_arg )
{ //{

	// Dersom input er blokkert er enten noden i 'refraction period' eller så fyrer det allerede for denne iterasjonen. (trenger ikkje kalkulere meir på depol..)
	if( bBlokkerInput_refractionTime ) return;
	else{ cout<<"forrige fyring på tid: " <<pElementAvAuron->ulTimestampForrigeFyring <<endl; }

	// beregner lekkasje av depol siden sist:
	calculateLeakage();

	pElementAvAuron->nAktivitetsVariabel += nNewSignal_arg;
	cout<<"s_dendrite::newInputSignal( " <<nNewSignal_arg <<" ); gir depol. :  " <<pElementAvAuron->nAktivitetsVariabel <<"\n";
	
	pElementAvAuron->ulTimestampForrigeInput = time_class::getTid();


	// Dersom auron går over fyringsterskel: fyr A.P.
	if( pElementAvAuron->nAktivitetsVariabel > FYRINGSTERSKEL )
	{
		// Blokkerer videre input (etter at vi allerede har nådd terskel)
		bBlokkerInput_refractionTime = true;

		//cout<<"Postsyn. kom over FYRINGSTERSKEL.\nLegger til i arbeidskø.\n\n";
		
	 	
		//Ekstra time-delay:
		time_class::leggTilTask( this );
		// for ekstra time delay. Evt kan vi kjøre auron som neste ledd:
		// Legger til neste ledd i signal-path (soma == auron).
	 	//time_class::leggTilTask( pElementAvAuron );

	}

	// Skriver til log for aktivitetsVar:
	pElementAvAuron->aktivitetsVar_loggFil 	<<time_class::getTid() <<"\t" <<pElementAvAuron->nAktivitetsVariabel <<"; \t #Depolarization\n" ;
	pElementAvAuron->aktivitetsVar_loggFil.flush();
} //}

/** TODO Bare for SANN:  ***/
inline void s_dendrite::calculateLeakage()
{ //{
	/*
	* 	Har testa verdiane i octave. Loggen følgte lå på kurva for [initDepol]*LEKKASJEFAKTOR_FOR_DEPOL^x, selv om eg bare sjekka calculateLeakage() for siste ledd 
	* 		(f.eks. etter 30 iterasjoner uten noko input anna enn init-verdien for depool. for auronet). Hurra!
	*/

	int slettDebugGammelDepolverdi = pElementAvAuron->nAktivitetsVariabel;

	if( pElementAvAuron->ulTimestampForrigeInput != time_class::getTid() )
	{
		// regner ut, og trekker fra lekkasje av depol til postsyn neuron.
		unsigned long sulTidSidenSist = time_class::getTid()-pElementAvAuron->ulTimestampForrigeInput;

		cout<<"XXX Tid siden sist: " <<sulTidSidenSist <<endl;

		// SKRIVER TIL nAktivitetsVariabel. XXX
	 	pElementAvAuron->nAktivitetsVariabel *= (double)pow( LEKKASJEFAKTOR_FOR_DEPOL, sulTidSidenSist );
		
		cout 	<<"\n\n\t\t\t\t\tLEKKASJEfaktor: " <<(double)pow( LEKKASJEFAKTOR_FOR_DEPOL, sulTidSidenSist ) <<" [gammel => ny depol.]: [" <<slettDebugGammelDepolverdi <<"=>" <<pElementAvAuron->nAktivitetsVariabel
				<<"]. (for auron " <<pElementAvAuron->sNavn <<")\n";
		// Bruker heller auron::ulTimestampForrigeInput..
		//ulTimestampForrigeOppdatering = time_class::getTid(); 
	}
	// Dersom den allerede har regna ut lekkasje: ikkje gjør det igjen. Returner.
} //}





/*************************************************************
****** 													******
******  		doTask() -- samla på en plass. 			******
****** 													******
*************************************************************/
inline void i_auron::doTask()
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

	// BARE for s_auron
	/*if( ulTimestampForrigeFyring == time_class::ulTidsiterasjoner )
	{
		cout<<"\n\n************************\nFeil?\nTo fyringer på en iterasjon? \nFeilmelding au#103 @ auron.h\n************************\n\n";
		return;
	}*/

	// Registrerer fyringstid (for feisjekk (over) osv.) XXX SKAL DETTE BARE VÆRE I SANN-NEURON?
	//ulTimestampForrigeFyring = time_class::ulTidsiterasjoner;

	// Skriver til log for aktivitetsVar:
	aktivitetsVar_loggFil 	<<time_class::getTid() <<"\t" <<nAktivitetsVariabel <<"; \t #Activity variable\n" ;
	aktivitetsVar_loggFil.flush();
		

} //}
inline void s_auron::doTask()
{ //{
	i_auron::doTask();

	
	if( ulTimestampForrigeFyring == time_class::getTid() )
	{
		cout<<"\n\n************************\nFeil?\nTo fyringer på en iterasjon? \nFeilmelding au#103 @ auron.h\n************************\n\n";
		return;
	}

	// Registrerer fyringstid (for feisjekk (over) osv.) 
	ulTimestampForrigeFyring = time_class::getTid();


	//Resetter depol.verdi 
	nAktivitetsVariabel = 0; 
	loggAktivitetsVar_i_AktivitetsVarLoggFil();
} //}
//XXX inline void K_auron::doTask() osv.


inline void s_synapse::doTask()
{ //{ .. }

	// FØR  VAR DET :  inline void synapse::transmission()
	cout<<"Overføring i s_synapse.\n";

	// Dersom synapsen har inhibitorisk effekt: send inhibitorisk signal (subtraksjon). Ellers: eksiter postsyn. auron.
	// Dendrite lagrer tidspunk for overføring.
	if( bInhibitorisk_effekt ){
 		pPostNodeDendrite->newInputSignal(  -uSynaptiskVekt );
	}else{
		pPostNodeDendrite->newInputSignal(   uSynaptiskVekt ); //TODO gjør uSynaptiskVekt om til uSynaptiskVekt
	}
//( dendrite legges til i arbeidskø dersom den kommer over fyringsterskel.) --Dette er feilaktig, men meir effektivt.
		


	// XXX Kalkulere syn.p.?



	// TODO: synaptisk plastisitet: HER.
	//{ Loggfører syn.weight
	synWeight_loggFil 	<<"\t" <<time_class::getTid() <<"\t" <<uSynaptiskVekt
						<<" ;   \t#Synpaptic weight\n" ;
	synWeight_loggFil.flush();
			//} 

	// Logg for aktivitetsVar for postsyn auron skjer i pPostNodeDendrite->newInputSignal(-);

} //}
inline void axon::doTask()
{ //{ // initierAksjonspotensial()
	cout<<"Legger inn alle outputsynapser i arbeidskø. Mdl. av auron: " <<pElementAvAuron->sNavn <<" - - - - - - - - - - - - - - - \n";

	// For meir nøyaktig simulering av tid kan alle synaper få verdi for 'time lag' før fyring. No fokuserer eg heller på effektivitet. 
 	for( std::list<s_synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end(); iter++ )
	{ // Legger alle pUtSynapser inn i arbeidskø: (FIFO-kø)
		
		//time_class::pTaskArbeidsKoe_List.push_back( (*iter) );
		// Legger til ut-synapser i time_class::arbeidskø
		time_class::leggTilTask( *iter );
	}

	// Avblokkerer dendritt. Opner den for meir input. Foreløpig er dette måten 'refraction time' funker på.. (etter 2 ms - dendrite og auron overføring..)
	pElementAvAuron->pInputDendrite->bBlokkerInput_refractionTime = false; 
	pElementAvAuron->loggAktivitetsVar_i_AktivitetsVarLoggFil();
} //}
inline void s_dendrite::doTask()
{ //{ 
	// Kva skal dendrite::doTask() gjøre? 
	//cout<<pElementAvAuron->sNavn <<"->[dendrite]::doTask(). Postsyn. depol (" <<pElementAvAuron->sNavn <<") etter overføring: " <<pElementAvAuron->nAktivitetsVariabel <<".\n";

	time_class::leggTilTask( pElementAvAuron );


} //}









