#ifndef STRUCTS_H_
#define STRUCTS_H_

typedef struct City {
  char name[20];
  double lat;
  double lng;
} City;

typedef struct Flight {
  char origin[20];
  char destination[20];
} Flight;

typedef struct Query {
  char origin[20];
  char destination[20];
} Query;

struct input {
  int cities;
  int direct_flights;
  int queries;
};

#endif
