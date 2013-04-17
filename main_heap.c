#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.141592653589793
#define EARTH_RADIUS 6378

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
#define MAXNODES 100
#define INFINITY 99999
#define MEMBER 1
#define NONMEMBER 0

typedef struct graph GRAPH;
typedef struct head HEAD;
typedef struct node NODE;
typedef struct pqueue PQUEUE;
typedef struct nodepqueue NODEPQUEUE;

/* cabeças das listas de adjacências. Para cada vértice do grafo é criado uma struct head; */
/* as heads são armazenadas em um vetor */
struct head {
  int perm; /* armazena 1 se vértice já foi incluído em PERM ou 0 em caso contrário */
  int path; /* nó precedente no caminho de custo mínimo*/
  int posQueue; /* posicao do vértice na fila de prioridades*/
  int minDist; /* distância mínima (atual se perm=0 ou final se perm=1) a partir do vértice de origem*/

  NODE *adj; /* ponteiro para lista de nós adjacentes*/
};

/* nós das listas de adjacências */
struct node {
  int arc; /* índice do vértice adjacente */
  int dist; /* peso da aresta */
  NODE *next; /* próximo nó adjacente da lista; */
};

/* nó da fila de prioridades. */
struct nodepqueue {
  int arc; /* vértice */
  int dist; /* distância */
};

/* fila de prioridades ascendente, implementada através de um heap ascendente */
struct pqueue {
  NODEPQUEUE elem[MAXNODES + 1];
  int last; /* índice do último elemento da fila (ou qtde. de elementos na fila) */
};

/* grafo (listas de adjacências) */
struct graph {
  HEAD heads[MAXNODES]; /* vetor de cabeças */
};

/* inicializa o vetor de cabeças que representa o grafo */
/* retorna ponteiro para uma struct graph */
GRAPH* init_graph () {
  GRAPH *gTemp = (GRAPH *) malloc(sizeof(GRAPH));
  int i = 0;
  for (; i < MAXNODES; i++) {
    gTemp->heads[i].adj = NULL;
  }
  return gTemp;
}

void heapfy_up (GRAPH * g, PQUEUE *pq, int i) {
  int father, childLeft, childRight, smallest;

  father = i / 2;

  if (father > 0) {
    childLeft = father * 2;
    childRight = father * 2 + 1;

    if (childLeft <= pq->last && pq->elem[childLeft].dist < pq->elem[father].dist)
      smallest = childLeft;
    else
      smallest = father;

    if (childRight <= pq->last && pq->elem[childRight].dist < pq->elem[smallest].dist)
      smallest = childRight;

    if (smallest != father) {
      change(g, pq, father, smallest);
      heapfy_up (g, pq, father);
    }
  }
}

void heapfy_down (GRAPH * g, PQUEUE *pq, int i) {
  int childLeft, childRight, smallest;

  childLeft = i * 2;
  childRight = i * 2 + 1;

  if (childLeft <= pq->last && pq->elem[childLeft].dist < pq->elem[i].dist) {
      smallest = childLeft;
  }
  else {
      smallest = i;
  }
  if (childRight <= pq->last && pq->elem[childRight].dist < pq->elem[smallest].dist)
      smallest = childRight;

  if (smallest != i) {
      change(g, pq, i, smallest);
      heapfy_down (g, pq, smallest);
  }
}

void insert_pqueue(GRAPH *g, PQUEUE *pq, int inode) {
  /* insere o elemento no final da fila de prioridades */
  pq->last = pq->last + 1;

  /* armazena a distância do vértice que está sendo incluído na fila */
  pq->elem[pq->last].arc = inode;
  pq->elem[pq->last].dist = g->heads[inode].minDist;

  /* atualiza posQueue o que me permite saber onde um vértice está na fila */
  g->heads[inode].posQueue = pq->last;

  /* refaz o heap ascendente */
  heapfy_up (g, pq, pq->last);
}

void update_pqueue(GRAPH *g, PQUEUE *pq, int inode) {
  /* refaz o heap ascendente a partir da posicao de inode */
  heapfy_up (g, pq, g->heads[inode].posQueue);
}

NODEPQUEUE remove_pqueue(GRAPH *g, PQUEUE *pq) {
    NODEPQUEUE aux;
    if (pq->last > 0) {
        /* armazena o primeiro elemento da fila de prioridades */
        aux = pq->elem[1];
        change(g, pq, 1, pq->last);

        pq->last = pq->last - 1;

        heapfy_down (g, pq, 1);

        return aux;
    }
    else {
        aux.arc = -1;
        return aux;
    }
}

int dijkstra(GRAPH *g, int s, int t) {
  int newdist;
  int current; NODEPQUEUE currentn;
  NODE *aux;

  /* cria fila de prioridades */
  PQUEUE *pq = (PQUEUE *) malloc(sizeof(PQUEUE));

  /* (re)inicializa dados usados pela fila de prioridade */
  init_dijkstra (g, pq);

  /* faz distância de s = 0 e o inclui na fila de prioridades */
  g->heads[s].minDist = 0;
  insert_pqueue (g, pq, s);

  while (1) {
    /* remove da fila de prioridades o elemento com menor distância/peso (O (log n)) */
    currentn = remove_pqueue (g, pq);
    current = currentn.arc;

    /* quando fila de prioriedades está vazia ela retorna -1 e o algoritmo termina */
    if (current == -1) {
      break;
    }

    /* faz-se current pertencer a PERM */
    g->heads[current].perm = 1;

    /* atualiza, se necessário, as distâncias de todos os vértices adjacentes a current */
    /* que ainda não foram incluídos em PERM */
    for (aux = g->heads[current].adj; aux; aux = aux->next)
    {
      if (g->heads[aux->arc].perm == 0) {
        newdist = g->heads[current].minDist + aux->dist;

        /* atualiza distância se a atual distância do vértice é maior que a distância */
        /* passando por current */
        if (g->heads[aux->arc].minDist > newdist) {
          if (g->heads[aux->arc].minDist == INFINITY) {
            g->heads[aux->arc].minDist = newdist;
            g->heads[aux->arc].path = current;

            /* O (log n) */
            insert_pqueue (g, pq, aux->arc);
          }
          else {
            g->heads[aux->arc].minDist = newdist;
            g->heads[aux->arc].path = current;

            /* O (log n) */
            update_pqueue (g, pq, aux->arc);
          }
        }
      }
    }
  }

  /* impressao do resultado ****************/

  if (g->heads[t].path == -1) {
    return -1;


    printf ("\n\nCAMINHO NAO EXISTE\n\n");
  }
  else {
    return g->heads[t].minDist;


    printf("\n\nRESULTADO: ");
    int caminho = t;
    printf("%d <- ", t);

    while (caminho != s)
    {
        printf("%d ", g->heads[caminho].path);
        caminho = g->heads[caminho].path;
        if (caminho != s)
            printf (" <- ");
    }

    printf("\n\ncusto: %d\n\n", g->heads[t].minDist);
  }
}


/**************
*** END DIJKSTRA
****************/

int best_route_distance(Flight query, Flight direct_flights[], struct input line) {
  GRAPH *graph;
  graph = init_graph();

  int i,j, num_df;
  num_df = line.direct_flights;
  /*
  for(i = 0; i < num_df; i++) {
    for(j = 0; j < num_df; j++) {
      joinwt(graph,
        direct_flights[i].origin.id,
        direct_flights[i].destination.id,
        direct_flights[i].distance
      );
    }
  }*/

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

    /* Find better route */
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
