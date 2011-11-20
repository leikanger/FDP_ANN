
#include "../neuroElements/auron.h"
#include "../neuroElements/synapse.h"

#define PI 3.14159

inline double dynamiskSensorFunk()
{
	return (2.1+sin((double)time_class::getTid() * (double)ALPHA))*(double)FYRINGSTERSKEL;
}
inline double statiskSensorFunk()
{
	return 1.100000000000*(double)FYRINGSTERSKEL;
	//return 1.5000000*FYRINGSTERSKEL;
}

// GAMMELT:
inline double sensorFunk1()
{
	#define SVINGNINGS_AMP 2.133

	return ( 1.1 * FYRINGSTERSKEL*( (1 + sin( 2 * 3.14*(float)time_class::getTid()/1000 ))) );
}
inline double sensorFunk1a()
{
	#define SVINGNINGS_AMP 2.133

	return ( 1.1 * FYRINGSTERSKEL*( (2 + sin( 2 * 3.14*(float)time_class::getTid()/1000 ))) );
}

inline double sensorFunk2()
{
	// Brukt i plotta: return (  FYRINGSTERSKEL * 0.50 * (1 - sin( 3* 3.14*(float)time_class::getTid()/1000 )) );
	if( time_class::getTid() < 2000)
		return (  FYRINGSTERSKEL * 0.50 * (1 - sin( 10* 3.14*(float)time_class::getTid()/1000 )) );
	else // tid er mellom 3000 og 3500
	{
		return  (FYRINGSTERSKEL * 2 * 0.5 * (1 - cos( 1*3.14*(float)time_class::getTid()/1000 )) );
	}
}

inline double sensorFunk3a()
{
	if(time_class::getTid() < 1000)
		return 0;
	else
		return 2*FYRINGSTERSKEL;
}
// TEST FOR sf3() etterpå (for å sjå om vi fortsatt har delay for overføring av kappa)
inline double sensorFunk3()
{
	if(time_class::getTid() < 2000)
	 	return  (FYRINGSTERSKEL *  (1 - cos( 1*3.14*(float)time_class::getTid()/1000 )) );
	else if( time_class::getTid() < 3000)
		return 0;
	else
		return FYRINGSTERSKEL*2;
}
inline double sensorFunk4()
{
	//return (( 1+((float)time_class::getTid() / 300) ) * FYRINGSTERSKEL);
	return ( FYRINGSTERSKEL*( 2 * (1 + sin( 3.14*(float)time_class::getTid()/4000 ))) );
}

// TEST 1 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
inline double sensorFunkEksempelFunk()
{
	if( time_class::getTid() <2000)
	 	return  (2*FYRINGSTERSKEL *  (1 - cos( 1*3.14*(float)time_class::getTid()/1000 )) );
	else 
	 	return  (FYRINGSTERSKEL *  (1 - cos( 2*3.14*(float)time_class::getTid()/1000 )) );
}
inline double sensorFunk_TEST1_s2()
{
	if(time_class::getTid() > 2500 && time_class::getTid() < 7000)
		return 1.8*FYRINGSTERSKEL;
	else
		return 0.5*FYRINGSTERSKEL; //Dette gir samme resultat som 0. Tester det..
}
inline double sensorFunk_TEST1_s3()
{
	if( 4800 < time_class::getTid() < 5100 )
		return 1.5*FYRINGSTERSKEL;
	else
		return 0.1*FYRINGSTERSKEL;
}
inline double sensorFunk_TEST1_s4()
{
	return 1.3*FYRINGSTERSKEL;
}
inline double sensorFunk_TEST1_s5()
{
	return 2*(FYRINGSTERSKEL + FYRINGSTERSKEL * sin(2*PI*((float)time_class::getTid()/2000)) );
}
// SLUTT test * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

