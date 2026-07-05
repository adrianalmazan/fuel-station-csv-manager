#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"



int load_data(const char *filepath, GasStation **stations, int *count) {
    FILE *file = fopen(filepath, "r");
    if (!file) return -1;

    char line[512];
    int capacity = 10;

    // Initial memory reservation
    *stations = calloc(capacity, sizeof(GasStation));
    if (*stations == NULL) {
        fclose(file);
        return -1;
    }

    fgets(line, sizeof(line), file); // Skip header

    while (fgets(line, sizeof(line), file)) {

        if (*count >= capacity) {
            capacity *= 2;

            GasStation *temp = realloc(*stations,
                                       capacity * sizeof(GasStation));

            if (temp == NULL) {
                printf("Memory allocation failed.\n");
                free(*stations);
                fclose(file);
                return -1;
            }

            *stations = temp;
        }

        GasStation *current = &(*stations)[*count];

        char price95_str[20] = "-1";
        char price98_str[20] = "-1";
        char diesel_str[20] = "-1";

        int parsed = sscanf(
            line,
            "%d;%[^;];%[^;];%d;%[^;];%c;%[^;];%[^;];%[^;];%[^\r\n]",
            &current->id,
            current->province,
            current->locality,
            &current->postal_code,
            current->address,
            &current->margin,
            current->brand,
            price95_str,
            price98_str,
            diesel_str
        );

        if (parsed >= 7) {

            for (int i = 0; price95_str[i]; i++)
                if (price95_str[i] == ',') price95_str[i] = '.';

            for (int i = 0; price98_str[i]; i++)
                if (price98_str[i] == ',') price98_str[i] = '.';

            for (int i = 0; diesel_str[i]; i++)
                if (diesel_str[i] == ',') diesel_str[i] = '.';

            current->price_95 = atof(price95_str);
            current->price_98 = atof(price98_str);
            current->price_diesel_a = atof(diesel_str);

            (*count)++;
        }
    }

    fclose(file);
    return 0;
}

void print_station(GasStation station) {
    printf("ID %d: %s\n", station.id, station.brand);
    printf("  Location: %s (%d %s, %s)\n", station.address, station.postal_code, station.locality, station.province);
    printf("  Prices  : Gas95: %.3f | Gas98: %.3f | Diesel A: %.3f\n", 
           station.price_95, station.price_98, station.price_diesel_a);
    printf("--------------------------------------------------\n");
}

void show_all(GasStation *stations, int count) {
    for (int i = 0; i < count; i++) {
        print_station(stations[i]);
    }
}

void show_by_id(GasStation *stations, int count) {
    int target_id;
    printf("Enter ID to search: ");
    scanf(" %d", &target_id);
    
    for (int i = 0; i < count; i++) {
        if (stations[i].id == target_id) {
            print_station(stations[i]);
            return;
        }
    }
    printf("ID %d not found.\n", target_id);
}


void modify_record(GasStation *stations, int count, const char *filepath) {
    int target_id;
    printf("Enter ID to modify: ");
    
    // We validate that the entered ID is a valid number.
    if (scanf(" %d", &target_id) != 1) {
        printf("Invalid input. Modification cancelled.\n");
        while(getchar() != '\n'); // Clear the input buffer
        return;
    }

    for (int i = 0; i < count; i++) {
        if (stations[i].id == target_id) {
            printf("Current record:\n");
            print_station(stations[i]);

            printf("Enter new Gasoline 95 price: ");
            if (scanf(" %f", &stations[i].price_95) != 1) {
                printf("Invalid input. Update cancelled.\n");
                while(getchar() != '\n'); 
                return;
            }

            printf("Enter new Gasoline 98 price: ");
            if (scanf(" %f", &stations[i].price_98) != 1) {
                printf("Invalid input. Update cancelled.\n");
                while(getchar() != '\n'); 
                return;
            }

            printf("Enter new Diesel price: ");
            if (scanf(" %f", &stations[i].price_diesel_a) != 1) {
                printf("Invalid input. Update cancelled.\n");
                while(getchar() != '\n'); 
                return;
            }

            printf("\nRecord successfully updated in memory.\n");
            
            if (save_data(filepath, stations, count) == 0) {
                printf("Changes successfully saved to %s.\n", filepath);
            }
            
            print_station(stations[i]);
            return;
        }
    }
    printf("ID %d not found.\n", target_id);
}

void calculate_max(GasStation *stations, int count) {
    float max_price = -1.0;
    int index = -1;

    for (int i = 0; i < count; i++) {
        if (stations[i].price_95 > max_price) {
            max_price = stations[i].price_95;
            index = i;
        }
    }

    if (index != -1) {
        printf("Station with the MAXIMUM Gasoline 95 price:\n");
        print_station(stations[index]);
    }
}

void calculate_min(GasStation *stations, int count) {
    float min_price = 999.0; // Initialize with a high value.
    int index = -1;

    for (int i = 0; i < count; i++) {
        // Ignore gas stations that do not sell 95-octane gasoline (e.g., price <= 0)
        if (stations[i].price_95 <= 0.0) continue; 
        
        if (stations[i].price_95 < min_price) {
            min_price = stations[i].price_95;
            index = i;
        }
    }

    if (index != -1) {
        printf("Station with the MINIMUM Gasoline 95 price:\n");
        print_station(stations[index]);
    }
}

// Comparison function required by qsort()
int compare_prices(const void *a, const void *b) {
    GasStation *station_a = (GasStation *)a;
    GasStation *station_b = (GasStation *)b;

    // If a gas station has no price (-1), we move it to the end.
    if (station_a->price_95 <= 0) return 1;
    if (station_b->price_95 <= 0) return -1;

    if (station_a->price_95 < station_b->price_95) return -1;
    if (station_a->price_95 > station_b->price_95) return 1;
    return 0; 
}

int save_data(const char *filepath, GasStation *stations, int count) {
    // We open the file in "w" (write) mode. This clears the previous content and writes from scratch.
    FILE *file = fopen(filepath, "w");
    if (!file) {
        printf("ERROR: Could not save data to %s\n", filepath);
        return -1;
    }

    // 1. Write the exact header of the original CSV
    fprintf(file, "ID;Provincia;Localidad;Codigo postal;Direccion;Margen;Rotulo;Precio gasolina 95 E5;Precio gasolina 98 E5;Precio gasoleo A\n");

    // 2. We iterate through the array and write each line.
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d;%s;%s;%d;%s;%c;%s;%.3f;%.3f;%.3f\n",
                stations[i].id, stations[i].province, stations[i].locality,
                stations[i].postal_code, stations[i].address, stations[i].margin,
                stations[i].brand, stations[i].price_95, stations[i].price_98,
                stations[i].price_diesel_a);
    }

    fclose(file);
    return 0;
}