#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "structs.h"
#include <string.h>

#define PI 3.141592653589793
#define EARTH_RADIUS 6378

/* Utilizar memoization se der tempo */
/* Fazer um malloc nisso aqui */
Flight memoize[10000];

/**
 * Finds best route distance
 * @return int distance; -1 if there is no route
 */
int best_route_distance(Flight query, Flight direct_flights[], City cities[], struct input line) {
  int direct_flight = direct_flight_distance(query, direct_flights, line);

  if(direct_flight != -1) {
    return direct_flight;
  }



  /* Dijkstra */
  int i, n, j, MAX, ARESTAS;

  MAX     = line.cities;
  ARESTAS = line.direct_flights;

  int routes[ARESTAS][ARESTAS];
  char vis[MAX];
  int dis[MAX];
  memset(vis, 0, sizeof(vis));
  memset(dis, 0x7f, sizeof (dis));

  dis[0] = 0;

  Flight *df = direct_flights;

  /* Inicia matriz */
  for(i = 0; i < ARESTAS; i++){
    for(j = 0; j < ARESTAS; j++)
      routes[i][j] = 0;
  }

  /* Preenche matriz */
  for(i = 0; i < ARESTAS; i++) {
    routes[df[i].origin.id][df[i].destination.id] = df[i].distance;
  }

  while(1) {
    n = -1;

    for(i = 0; i < MAX; i++) {
      if(!vis[i] && (n < 0 || df[i].distance < df[n].distance))
        n = i;
    }

    if(n < 0)
      break;

    vis[n] = 1;

    for(i = 0; i < ARESTAS; i++) {
      if(routes[n][i] > 0 && dis[i] > dis[n] + routes[n][i]){
        dis[i] = dis[n] + routes[n][i];
      }
    }
  }

  int sum = 0;
  for(i = 0; i < MAX; i++) {
    /*printf("%i => %d\n", i, dis[i]);*/
    sum += dis[i];
  }

  if(sum > 0) {
    return sum;
  }

  return -1;
}

/**
 * Check if exist a direct flight
 * @return int the distance if flight exists; -1 otherwise
 */
int direct_flight_distance(Flight query, Flight direct_flights[], struct input line) {
  int i = 0;

  for(; i < line.direct_flights; i++) {
    if(strcmp(query.origin.name, direct_flights[i].origin.name) == 0 &&
       strcmp(query.destination.name, direct_flights[i].destination.name) == 0) {
      return distance_between_cities(query.origin, query.destination);
    }
  }
  return -1;
}

/**
 * Converts degrees to radians
 */
double deg2rad(double value) {
  return value * PI / 180;
}

/**
 * Returns distance between 2 cities
 */
int distance_between_cities(City origin, City destination) {
  double result, l1, l2, delta;

  l1    = deg2rad(origin.lat);
  l2    = deg2rad(destination.lat);
  delta = deg2rad(destination.lng - origin.lng);

  result = acos(sin(l2) * sin(l1) + cos(l2) * cos(l1) * cos(delta)) * EARTH_RADIUS;

  return rround(result);
}

/**
 * Scanf cities
 */
void scan_cities(City cities[], int num_cities) {
  City city;
  int i;

  for(i = 0; i < num_cities; i++) {
    scanf("%s %lf %lf", city.name, &city.lat, &city.lng);
    city.id = i;
    cities[i] = city;
  }
}

/**
 * Scanf direct flights
 */
void scan_direct_flights(Flight direct_flights[], City cities[], struct input line) {
  char origin[20], destination[20];
  int i, j;
  Flight flight;

  for(i = 0; i < line.direct_flights; i++) {
    scanf("%s %s", origin, destination);

    /* Find city by name */
    for(j = 0; j < line.cities; j++) {
      if(strcmp(cities[j].name, origin) == 0) {
        flight.origin = cities[j];
      } else if(strcmp(cities[j].name, destination) == 0) {
        flight.destination = cities[j];
      }
    }
    flight.distance = distance_between_cities(flight.origin, flight.destination);

    direct_flights[i] = flight;
  }
}

/**
 * Scanf queries
 */
void scan_queries(Flight queries[], City cities[], struct input line) {
  char origin[20], destination[20];
  Flight query;
  int i, j;

  for(i = 0; i < line.queries; i++) {
    scanf("%s %s", origin, destination);

    /* Find city by name */
    for(j = 0; j < line.cities; j++) {
      if(strcmp(cities[j].name, origin) == 0) {
        query.origin = cities[j];
      } else if(strcmp(cities[j].name, destination) == 0) {
        query.destination = cities[j];
      }
    }

    queries[i] = query;
  }
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
 * Checks if input is the end of the program
 */
bool end(struct input line) {
  return (line.cities + line.direct_flights + line.queries) == 0;
}
