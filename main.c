#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structs.h"
#include "functions.h"

struct input line;


void test(City cities[], Flight direct_flights[], Query queries[]) {
  printf("%s, %lf, %lf\n", cities[0].name, cities[0].lat, cities[0].lng);
  printf("%s, %lf, %lf\n", cities[1].name, cities[1].lat, cities[1].lng);
  printf("%s, %s\n", direct_flights[0].origin, direct_flights[0].destination);
  printf("%s, %s\n", queries[0].origin, queries[0].destination);
}

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



    /* Check next case */
    scanf("%d %d %d", &line.cities, &line.direct_flights, &line.queries);
  }

  return EXIT_SUCCESS;
}
