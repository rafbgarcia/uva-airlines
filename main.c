#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.141592653589793
#define EARTH_RADIUS 6378
#define MAXNODES 100
#define INFINITY 99999
#define MEMBER 1
#define NONMEMBER 0

/***************************
 * Structs
 ***************************/
typedef struct City {
  int id;
  char name[20];
  double lat;
  double lng;
} City;

typedef struct Flight {
  City origin;
  City destination;
  int distance;
} Flight;

struct input {
  int cities;
  int direct_flights;
  int queries;
};



/***************************
 * Functions
 ***************************/
void print_no_route_msg() {
  printf("no route exists\n");
}

void print_case_msg(int count) {
  printf("Case #%d\n", count);
}

void print_distance_msg(int distance) {
  printf("%d km\n", distance);
}

double deg2rad(double value) {
  return value * PI / 180;
}

int distance_between_cities(City origin, City destination) {
  double result, l1, l2, delta;

  l1    = deg2rad(origin.lat);
  l2    = deg2rad(destination.lat);
  delta = deg2rad(destination.lng - origin.lng);

  result = acos(sin(l2) * sin(l1) + cos(l2) * cos(l1) * cos(delta)) * EARTH_RADIUS;

  return rround(result);
}

void scan_cities(City cities[], int num_cities) {
  City city;
  int i;

  for(i = 0; i < num_cities; i++) {
    scanf("%s %lf %lf", city.name, &city.lat, &city.lng);
    city.id = i;
    cities[i] = city;
  }
}

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

int rround(double n) {
  int n1 = n;
  double n2 = n1 + 0.5;
  return (n >= n2) ? (n1 + 1) : n1;
}

int end(struct input line) {
  return (line.cities + line.direct_flights + line.queries) == 0 ? 1 : 0;
}


/********************
*** DIJKSTRA
****************/

typedef struct graph GRAPH;
struct arc {
  int adj;    /* se adj=1 então ADJACENTE; se adj=0 então NÃO ADJACENTE */
  int weight; /* peso da aresta */
};
struct graph {
  struct arc arcs[MAXNODES][MAXNODES]; /* matriz de adjacências */
};

/* inicializa matriz de adjacência que representa o grafo */
/* retorna ponteiro para esta matriz (tipo GRAPH) */
GRAPH* init_graph () {
  GRAPH *gTemp = (GRAPH *) malloc(sizeof(GRAPH));
  int i,j;
  for (i = 0; i < MAXNODES; i++) {
    for (j = 0; j < MAXNODES; j ++) {
      gTemp->arcs[i][j].adj = 0;
      gTemp->arcs[i][j].weight = INFINITY;
    }
  }
  return gTemp;
}

/* cria uma aresta que "liga" (incide) em dois nós e atribui o respectivo peso; */
/* recebe o grafo, dois nós (node1 e node2) e o peso (wt) da aresta */
void joinwt(GRAPH *g, int node1, int node2, int wt) {
  g->arcs[node1][node2].adj = 1;
  g->arcs[node1][node2].weight = wt;
}

int dijkstra(GRAPH *g, int s, int t) {
  int dist[MAXNODES], perm[MAXNODES], path[MAXNODES];
  int current, i, k, dc;
  int smalldist, newdist;

  /* Inicializa todos os índices de 'perm' como 0 e de 'dist' como INFINITY */
  for (i = 0; i < MAXNODES; i++) {
      perm[i] = NONMEMBER;
      dist[i] = INFINITY;
  }

  /* Inclui 's' em perm (perm[s]=1) e configura(armazena) sua distancia como sendo zero */
  perm[s] = MEMBER;
  dist[s] = 0;

  /* define 's' como origem (fonte) da busca */
  current = s;
  k = current;

  while (current != t) {
    smalldist = INFINITY;
    dc = dist[current];

    for (i = 0; i < MAXNODES; i++) {
      /* se o elemento NÃO está em perm */
      if (perm[i] == NONMEMBER) {
        /*calcula distância a partir do vértice corrente ao vértice adjacente i*/
        newdist = dc + g->arcs[current][i].weight;

        /*se a distância partindo do vértice corrente for menor, atualiza o vetor*/
        /*de distâncias e de precedência*/
        if (newdist < dist[i]) {
          dist[i] = newdist;
          path[i] = current;
        }

        /*determina o vértice (entre todos os não pertencentes a perm) com menor distância*/
        if (dist[i] < smalldist) {
          smalldist = dist[i];
          k = i;
        }
      }
    }

    /* embora estamos assumindo grafos ponderados e conexos, este if garante que
     em caso de não existência de um caminho o programa não entre em loop infinito */
    if (current == k) {
      return -1;
    }

    current = k;
    perm[current] = MEMBER;
  }
  return dist[t];
}


/**************
*** END DIJKSTRA
****************/

int best_route_distance(Flight query, Flight direct_flights[], struct input line) {
  GRAPH *graph;
  graph = init_graph();

  int i, j, num_df;
  num_df = line.direct_flights;

  for(i = 0; i < num_df; i++) {
    for(j = 0; j < num_df; j++) {
      joinwt(graph,
        direct_flights[i].origin.id,
        direct_flights[i].destination.id,
        direct_flights[i].distance
      );
    }
  }

  return dijkstra(graph, query.origin.id, query.destination.id);
}



/***************************
 * Main
 ***************************/
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

    for(i = 0; i < line.queries; i++) {
      distance = best_route_distance(queries[i], direct_flights, line);

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
