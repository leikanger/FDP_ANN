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
#include <iostream>
#include <string>
//#include <vector>
#include <list>
#include <map>
#include <vector>
//#include <set>


#include <stdlib.h> 	//- for rand()
#include <cmath> 		//- for pow() funksjoner
//include <algorithm> 	- for_each()
//include <fstream> 	- file streams



//Defines:
#define KOMMENTER_UT_pEstimatedTaskTime 1

#define DEFAULT_ANTALL_TIDSITERASJONER 1000
#define DEFAULT_PERIODE_MELLOM_RECALC_KAPPA 100
#define UTSKRIFT_AV_TID true

#define DEBUG_UTSKRIFTS_NIVAA 0

#define ALPHA (double)0.004
#define LEKKASJEFAKTOR_FOR_DEPOL (double)1-ALPHA

//#define MIN_LENGDE_PAA_pEstimatedTaskTime 20
//#define MAKS_LENGDE_PAA_pEstimatedTaskTime_JOBBER 100 // Dersom eg jobb er planlagt etter om [dette tallet] tidssteg, går den ikkje inn i pEstimatedTaskTime.

#define FYRINGSTERSKEL 1000

#define OCTAVE_SLEEP_ETTER_PLOTTA 3

#define DEBUG_EXIT(tekst) cerr<<"\n\nDEBUG_EXIT :\t" <<tekst <<"\nexit(99);\n\n"; exit(99);
//#define DEBUG(tekst) cerr<<"DEBUG :\t" <<tekst <<"\n";
#define DEBUG(tekst) ;

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
