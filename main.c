#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "functions.h"

struct input line;

int main(int argc, char ** argv) {

  scanf("%d %d %d", &line.cities, &line.direct_flights, &line.queries);

  while( ! end(line)) {
    City   cities[line.cities];
    Flight direct_flights[line.direct_flights];
    Query  queries[line.queries];

    /* Populate variables with inputs */
    scan_cities(cities, line.cities);
    scan_direct_flights(direct_flights, line.direct_flights);
    scan_queries(queries, line.queries);


    /* Finds better way */
    find_better_way(cities, direct_flights, queries);


    /* Check next case */
    scanf("%d %d %d", &line.cities, &line.direct_flights, &line.queries);

  }

  return EXIT_SUCCESS;
}
