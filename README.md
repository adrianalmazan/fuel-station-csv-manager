# Fuel Station CSV Manager

A terminal-based C application that loads, manages, and analyzes fuel station pricing data stored in a CSV file.

## Features

- Load fuel station data from a CSV file.
- Interactive command-line menu.
- Search fuel stations by ID.
- Update fuel prices.
- Find the highest Gasoline 95 price.
- Find the lowest Gasoline 95 price.
- Sort records by Gasoline 95 price using the C standard library `qsort`.
- Dynamic memory management using `calloc`, `realloc`, and `free`.

## Technologies

- C (ISO C)
- Dynamic memory allocation
- File handling
- CSV parsing
- String manipulation
- Standard Library (`qsort`)

## Project Structure

```text
fuel-station-csv-manager/
│
├── src/
│   ├── main.c
│   ├── data.c
│   └── data.h
│
├── data/
│   └── fuel_prices_albacete.csv
│
├── README.md
├── .gitignore
```

## Compilation

Compile the project using GCC:

gcc src/main.c src/data.c -o fuel_manager


## Running

Run the executable:

./fuel_manager


Or specify a custom CSV file:

./fuel_manager ./data/another_file.csv


## Example

```
--- Albacete Gas Stations Database ---
Successfully loaded 180 records.

Select an action:

1. Show all records
2. Find record by ID
3. Modify record by ID
4. Calculate maximum price
5. Calculate minimum price
6. Sort records by price
7. Exit
```

## Author

Adrián