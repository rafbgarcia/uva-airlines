#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "functions.h"

void print_no_route_msg() {
  printf("no route exists\n");
}
void print_case_msg(int count) {
  printf("Case #%d\n", count);
}
void print_distance_msg(int distance) {
  printf("%d km\n", distance);
}

int main(int argc, char ** argv) {
  struct input line;
  int i, case_count, distance;

  scanf("%d %d %d", &line.cities, &line.direct_flights, &line.queries);

  case_count = 1;
  while( ! end(line)) {
    if(case_count > 1) {
      printf("\n");
    }

    City   cities[line.cities];
    Flight direct_flights[line.direct_flights];
    Flight queries[line.queries];

    scan_cities(cities, line.cities);
    scan_direct_flights(direct_flights, cities, line);
    scan_queries(queries, cities, line);

    print_case_msg(case_count++);

    /* Find better route */
    for(i = 0; i < line.queries; i++) {
      distance = best_route_distance(queries[i], direct_flights, cities, line);

      if(distance != -1) {
        print_distance_msg(distance);
      } else {
        print_no_route_msg();
      }
    }

    /* Check next case */
    scanf("%d %d %d", &line.cities, &line.direct_flights, &line.queries);
  }

  return EXIT_SUCCESS;
}
