#include <stdbool.h>
#include "structs.h"

int rround(double n);

int distance(City origin, City destination);

double deg2rad(double value);

bool end(struct input line);

void scan_cities(City cities[], int num_cities);

void scan_direct_flights(Flight direct_flights[], int num_flights);

void scan_queries(Query queries[], int num_queries);
