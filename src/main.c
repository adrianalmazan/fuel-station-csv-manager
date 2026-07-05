#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"




int main(int argc, char *argv[]) {
    GasStation *stations = NULL;
    int count = 0;
    int option;

    // Allow passing the file path via the terminal, or use your own default
    const char *filepath = (argc > 1) ? argv[1] : "./data/fuel_prices_albacete.csv";

    if (load_data(filepath, &stations, &count) != 0) {
        printf("ERROR: Could not load data from %s\n", filepath);
        return -1;
    }

    printf("--- Albacete Gas Stations Database ---\n");
    printf("Successfully loaded %d records.\n", count);

    // Interactive menu main loop
    do {
        printf("\nSelect an action:\n"
               "1. Show all records.\n"
               "2. Find record by ID.\n"
               "3. Modify record by ID.\n"
               "4. Calculate maximum price (Gasoline 95).\n"
               "5. Calculate minimum price (Gasoline 95).\n"
               "6. Sort records by price (Gasoline 95).\n"
               "7. Exit.\n"
               "Option: ");
        
        if (scanf(" %d", &option) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while(getchar() != '\n'); // Clear the input buffer
            continue;
        }

        printf("\n");
        switch (option) {
            case 1:
                show_all(stations, count);
                break;
            case 2:
                show_by_id(stations, count);
                break;
            case 3:
                modify_record(stations, count, filepath);
                break;
            case 4:
                calculate_max(stations, count);
                break;
            case 5:
                calculate_min(stations, count);
                break;
            case 6:
                // Uso de qsort (Quick Sort)
                qsort(stations, count, sizeof(GasStation), compare_prices);
                printf("Records sorted by Gasoline 95 price.\n");
                show_all(stations, count);
                break;
            case 7:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid option. Try again.\n");
        }
    } while (option != 7);

    // Free dynamic memory before exiting.
    free(stations);
    return 0;
}

