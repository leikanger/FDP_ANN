//TODO Flytt denne inn i tid.cpp :
/*****************************************************************
** 	void* taskSchedulerFunction(void*)							**
** 																**
** 		- Har ansvaret for å schedule arbeidet. 				**
** 			- kaller nesteJobb.doTask()							**
** 			- For nesteJobb==tid vil tid::doTask() kalles 		**
** 				Dette itererer tid, og gjør anna tidsrelevant 	**
** 					arbeid										**
** 																**
*****************************************************************/
/*void* taskSchedulerFunction(void* )
{ //{1
	for(int i=0; i<nAntallTidsiterasjonerTESTING_SLETT; i++) // XXX Skal bli "uendelig" løkke etterkvart:
	//while(/ *En eller anna avsluttings-bool =* /true)
	{
			/ *FEILSJEKK XXX Skal bort før testing: * /
			if(tid::pTaskArbeidsKoe_List.empty()){ cout<<"\n\n\nFEIL. tid::pTaskArbeidsKoe_List er tom. Skal aldri skje. \nFeilmelding: [taskSchedulerFunction::c01]\n\n\n"; exit(-1);}

			// DEBUG: 	Skriv ut klassenavn på element:
			cout<<"\tKjører element i arbeidskø: " <<tid::pTaskArbeidsKoe_List.front() ->klasseNavn <<endl; 		
	

			// Setter igang utføring av neste jobb i lista:
			tid::pTaskArbeidsKoe_List.front() ->doTask(); 		//Dette er i orden, siden pTaskArbeidsKoe_List er av type list<tidInterface*> og alle arvinger av tidInterface har overlagra funksjonen doTask().

			// Tar vekk jobben fra pTaskArbeidsKoe_List:
			tid::pTaskArbeidsKoe_List.pop_front();
			
			//Evt annet som skal gjøres kvart tidssteg. Type sjekke etter andre events, legge til fleire synapser, etc.

			// For KANN: skal også sjekke om noko neuron er estimert til å fyre denne iterasjonen.
	}
} //}1
*/
