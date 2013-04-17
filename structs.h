#ifndef STRUCTS_H_
#define STRUCTS_H_

typedef struct City {
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

#endif
