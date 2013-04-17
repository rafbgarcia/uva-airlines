#include <stdbool.h>
#include "structs.h"


/**
 * Round the number to nearest integer
 * @example 8.5 => 9
 */
int rround(double n);

/**
 * Converts degrees to radians
 */
double deg2rad(double value);

/**
 * Returns distance between 2 cities
 */
int distance_between_cities(City origin, City destination);

/**
 * Check if exist a direct flight
 * @return int the distance if flight exists; -1 otherwise
 */
int direct_flight_distance(Flight query, Flight direct_flights[], struct input line);

/**
 * Checks if input is the end of the program
 */
bool end(struct input line);

/**
 * Scanf cities
 */
void scan_cities(City cities[], int num_cities);

/**
 * Scanf direct flights
 */
void scan_direct_flights(Flight direct_flights[], City cities[], struct input line);
/*void scan_direct_flights(Flight direct_flights[], int num_flights);*/

/**
 * Scanf queries
 */
void scan_queries(Flight queries[], City cities[], struct input line);
