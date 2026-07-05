#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>

// Fuel station data structure
typedef struct {
    int id;
    char province[50];
    char locality[50];
    int postal_code;
    char address[100];
    char margin;
    char brand[50];
    float price_95;
    float price_98;
    float price_diesel_a;
} GasStation;


int load_data(const char *filepath, GasStation **stations, int *count);
void print_station(GasStation station);
void show_all(GasStation *stations, int count);
void show_by_id(GasStation *stations, int count);
void modify_record(GasStation *stations, int count, const char *filepath) ;
void calculate_max(GasStation *stations, int count);
void calculate_min(GasStation *stations, int count);
int compare_prices(const void *a, const void *b);
int save_data(const char *filepath, GasStation *stations, int count);

#endif