#include <stdio.h>
#include <stdbool.h>
#include "structs.h"


void find_better_way(City cities[], Flight direct_flights[], Query queries[]) {
  // Percorrer as consultas
  // Pegar a cidade destino
  // Checar se algum voo direto tem destino para este destino
  // Se nao tiver, a rota nao existe

}

bool end(struct input line) {
  return (line.cities + line.direct_flights + line.queries) == 0;
}

void scan_cities(City cities[], int num_cities) {
  City city;
  int i;

  for(i = 0; i < num_cities; i++) {
    scanf("%s %lf %lf", city.name, &city.lat, &city.lng);
    cities[i] = city;
  }
}

void scan_direct_flights(Flight direct_flights[], int num_flights) {
  Flight flight;
  int i;

  for(i = 0; i < num_flights; i++) {
    scanf("%s %s", flight.origin, flight.destination);
    direct_flights[i] = flight;
  }
}

void scan_queries(Query queries[], int num_queries) {
  Query query;
  int i;

  for(i = 0; i < num_queries; i++) {
    scanf("%s %s", query.origin, query.destination);
    queries[i] = query;
  }
}
