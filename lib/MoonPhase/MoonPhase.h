/* Moon phase calculation for OpenWeather library*/
// Adapted by Bodmer from code here:
// http://www.voidware.com/moon_phase.htm

#include <stdio.h>
#include <math.h>

#define PI  3.1415926535897932384626433832795
#define RAD (PI/180.0)
#define SMALL_FLOAT (1e-12)

double Julian(int year, int month, double day);

double sun_position(double j);

double moon_position(double j, double ls);

uint8_t moon_phase(int year, int month, int day, double hour, int* ip);
