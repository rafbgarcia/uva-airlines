#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "structs.h"

#define pi 3.141592653589793
#define earth_radius 6378


void find_better_way(City cities[], Flight direct_flights[], Query queries[]) {
  /*
  Percorrer as consultas
  Pegar a cidade destino
  Checar se algum voo direto tem destino para este destino
  Se nao tiver, a rota nao existe
  */
}

/**
 * Round the number to nearest integer
 * @example 8.5 => 9
 */
int rround(double n) {
  int n1 = n;
  double n2 = n1 + 0.5;
  return (n >= n2) ? (n1 + 1) : n1;
}

/**
 * Converts degrees to radians
 */
double deg2rad(double value) {
  return value * pi / 180;
}

/**
 * Returns distance between 2 cities
 */
int distance_between_cities(City origin, City destination) {
  double result, l1, l2, delta;

  l1    = deg2rad(origin.lat);
  l2    = deg2rad(destination.lat);
  delta = deg2rad(destination.lng - origin.lng);

  result = acos(sin(l2) * sin(l1) + cos(l2) * cos(l1) * cos(delta)) * earth_radius;

  return rround(result);
}

/**
 * Checks if input is the end of the program
 */
bool end(struct input line) {
  return (line.cities + line.direct_flights + line.queries) == 0;
}

/**
 * Scanf cities
 */
void scan_cities(City cities[], int num_cities) {
  City city;
  int i;

  for(i = 0; i < num_cities; i++) {
    scanf("%s %lf %lf", city.name, &city.lat, &city.lng);
    cities[i] = city;
  }
}

/**
 * Scanf direct flights
 */
void scan_direct_flights(Flight direct_flights[], int num_flights) {
  Flight flight;
  int i;

  for(i = 0; i < num_flights; i++) {
    scanf("%s %s", flight.origin, flight.destination);
    direct_flights[i] = flight;
  }
}

/**
 * Scanf queries
 */
void scan_queries(Query queries[], int num_queries) {
  Query query;
  int i;

  for(i = 0; i < num_queries; i++) {
    scanf("%s %s", query.origin, query.destination);
    queries[i] = query;
  }
}
