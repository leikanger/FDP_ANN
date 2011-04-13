
#include <iostream>
#include <string>
//#include <vector>
#include <list>
#include <map>
#include <set>

#include <stdlib.h> 	//- for rand()
#include <cmath> 		//- for pow() funksjoner
//include <algorithm> 	- for_each()
//include <fstream> 	- file streams



//Defines:
#define DEFAULT_ANTALL_TIDSITERASJONER 100

#define ALPHA 0.96
#define LEKKASJEFAKTOR_FOR_DEPOL ALPHA


#define FYRINGSTERSKEL 1000



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

//using std::list;
using std::cout;
using std::endl;
//using std::vector;





// deklarasjoner:







/*
	simulert, diskret TID
*/
//unsigned long ulTidsiterasjoner; // ligg i main.h
// flytta inn som static time_class::ulTidsiterasjoner;







#endif
