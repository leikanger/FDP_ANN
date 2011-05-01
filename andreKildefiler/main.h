
#include <iostream>
#include <string>
//#include <vector>
#include <list>
//#include <map>
#include <vector>
//#include <set>


#include <stdlib.h> 	//- for rand()
#include <cmath> 		//- for pow() funksjoner
//include <algorithm> 	- for_each()
//include <fstream> 	- file streams



//Defines:
#define DEFAULT_ANTALL_TIDSITERASJONER 1000

#define ALPHA (double)0.996
#define LEKKASJEFAKTOR_FOR_DEPOL ALPHA

#define MIN_LENGDE_PAA_pEstimatedTaskTime 20
#define FAKTOR_FOR_AA_FAA_RETT_PERIODE 1000 

#define FYRINGSTERSKEL 1000

#define DEBUG_EXIT(tekst) cerr<<"\n\nDEBUG_EXIT :\t" <<tekst <<"\nexit(99);\n\n"; exit(99);
#define DEBUG(tekst) cerr<<"DEBUG :\t" <<tekst <<"\n";

#ifndef MAIN_H_
#define MAIN_H_

#include "../neuroElements/auron.h"
#include "time.h"

#ifndef SYNAPSE_H_
#include "../neuroElements/synapse.h"
#endif
/*
#ifndef DENDRITE_H_
#include "../neuroElements/dendrite.h"
#endif*/

using std::cerr;
using std::cout;
using std::endl;





// deklarasjoner:







/*
	simulert, diskret TID
*/
//unsigned long ulTidsiterasjoner; // ligg i main.h
// flytta inn som static time_class::ulTidsiterasjoner;







#endif
// vim:fdm=marker:fmr=//{,//}
