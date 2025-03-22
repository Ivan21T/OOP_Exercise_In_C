#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct ParkingSpot {
    int id;
    bool occupied;
    char type[50];
    double price;
    char registrationPlate[9];
    struct ParkingInterval* parkingIntervals[20];
};

struct ParkingInterval {
    struct ParkingSpot* parkingSpot;
    char registrationPlate[9];
    int hoursParked;
    double revenue;
};
void FreeMemory(struct ParkingSpot* parkingSpots[], int* indexArray) {
    for (int i = 0; i < *indexArray; i++) {
        if (parkingSpots[i] != NULL) {
            for (int j = 0; j < 20; j++) {
                if (parkingSpots[i]->parkingIntervals[j] != NULL) {
                    free(parkingSpots[i]->parkingIntervals[j]);
                    parkingSpots[i]->parkingIntervals[j] = NULL;
                }
            }
            free(parkingSpots[i]);
            parkingSpots[i] = NULL;
        }
    }
    *indexArray = 0;
}
void InitialiseParkingInterval(struct ParkingInterval* parkingInterval, struct ParkingSpot* spot, char registrationPlate[9], int hoursParked)
{
    if (hoursParked <= 0)
    {
        fprintf(stdout, "Hours parked can�t be zero or negative! \n");
        return;
    }
    parkingInterval->parkingSpot = spot;
    strcpy(parkingInterval->registrationPlate, registrationPlate);
    parkingInterval->hoursParked = hoursParked;
    parkingInterval->revenue = hoursParked * (spot->price);
}

void InitialiseParkingSpot(struct ParkingSpot* spot, int id, bool occupied, char type[50], double price)
{
    if (price < 0 && strcmp(type, "subscription") != 0) 
    {
        fprintf(stdout, "Price cannot be negative unless it's a subscription parking spot \n");
        return;
    }
    else
    {
        spot->price = price;
    }
    spot->id = id;
    strcpy(spot->type, type);
    spot->occupied = occupied;
    for (int i = 0; i < 20; i++) {
        spot->parkingIntervals[i] = NULL;  
    }
    if (strcmp(spot->type, "subscription") == 0)
    {
        char registrationPlate[9];
        printf("Give registration plate to the system: ");
        scanf("%s", registrationPlate);
        fprintf(stdout, "\n");
        strcpy(spot->registrationPlate, registrationPlate);
    }
}

void InformationAboutParkingSpot(struct ParkingSpot* spot)
{
    if (strcmp(spot->type, "subscription") == 0)
    {
        fprintf(stdout, "Id: %d, Occupied: %s, Type:subscription, Price: %.2f, Registration plate: %s \n",
            spot->id, spot->occupied ? "true" : "false", spot->price, spot->registrationPlate);
    }
    else
    {
        fprintf(stdout, "Id: %d, Occupied: %s, Type: %s, Price: %.2f \n",
            spot->id, spot->occupied ? "true" : "false", spot->type, spot->price);
    }
}

void InformationAboutParkingInterval(struct ParkingInterval* parkingInterval)
{
    fprintf(stdout, "Parking spot: %d, RegistrationPlate: %s, HoursParked: %d hours, Revenue: %.2f BGN \n",
        parkingInterval->parkingSpot->id, parkingInterval->registrationPlate, parkingInterval->hoursParked, parkingInterval->revenue);
}

void CreateParkingSpot(int id, bool occupied, char type[50], double price, int* indexArray, struct ParkingSpot* parkingSpots[])
{
    if (*indexArray >= 30) {
        fprintf(stdout, "Array of parking spots is  full.\n");
        return;
    }
    struct ParkingSpot* parkingSpot = malloc(sizeof(struct ParkingSpot));
    InitialiseParkingSpot(parkingSpot, id, occupied, type, price);
    parkingSpots[*indexArray] = parkingSpot;
    (*indexArray)++;
    fprintf(stdout, "Successful created parking spot! \n");
}

void ParkVehicle(int parkingSpotId, char registrationPlate[9], int hoursParked, int* indexArray, struct ParkingSpot* parkingSpots[])
{
    if (hoursParked <= 0) {
        fprintf(stdout, "Hours parked cannot be zero or negative.\n");
        return;
    }

    for (int i = 0; i < *indexArray; i++) {
        if (parkingSpots[i]->id == parkingSpotId) {
            if (parkingSpots[i]->occupied) {
                fprintf(stdout, "Vehicle %s can't park at %d.\n", registrationPlate, parkingSpotId);
                return;
            }

            if (strcmp(parkingSpots[i]->type, "subscription") == 0) {
                if (strcmp(parkingSpots[i]->registrationPlate, registrationPlate) != 0) {
                    fprintf(stdout, "Vehicle %s can't park at %d.\n", registrationPlate, parkingSpotId);
                    return;
                }
            }

            for (int j = 0; j < 20; j++) {
                if (parkingSpots[i]->parkingIntervals[j] == NULL) {
                    struct ParkingInterval* newInterval = malloc(sizeof(struct ParkingInterval));
                    if (!newInterval) {
                        fprintf(stdout, "Memory allocation failed! \n");
                        return;
                    }

                    InitialiseParkingInterval(newInterval, parkingSpots[i], registrationPlate, hoursParked);
                    parkingSpots[i]->parkingIntervals[j] = newInterval;
                    parkingSpots[i]->occupied = true;

                    fprintf(stdout, "Vehicle %s parked at %d for %d hours.\n", registrationPlate, parkingSpotId, hoursParked);
                    return;
                }
            }
        }
    }
    fprintf(stdout, "Parking spot %d not found!\n", parkingSpotId);
}

void GetParkingSpotById(int parkingSpotId, struct ParkingSpot* parkingSpots[], int* indexArray)
{
    for (int i = 0; i < *indexArray; i++)
    {
        if (parkingSpotId == parkingSpots[i]->id)
        {
            InformationAboutParkingSpot(parkingSpots[i]);
            return;
        }
    }
    fprintf(stdout, "Parking spot %d not found!\n", parkingSpotId);
}

void FreeParkingSpot(int parkingSpotId, struct ParkingSpot* parkingSpots[], int* indexArray)
{
    for (int i = 0; i < *indexArray; i++)
    {
        if (parkingSpotId == parkingSpots[i]->id)
        {
            if (parkingSpots[i]->occupied == true)
            {
                parkingSpots[i]->occupied = false;
                fprintf(stdout, "Parking spot %d is now free!\n", parkingSpots[i]->id);
                return;
            }
            else {
                fprintf(stdout, "Parking spot %d is not occupied!\n", parkingSpots[i]->id);
                return;
            }
        }
    }
    fprintf(stdout,"Parking spot %d not found!",parkingSpotId);
}

void CalculateTotal(struct ParkingSpot* parkingSpots[],int *indexArray)
{
    double totalSum = 0;
    for (int i = 0; i < *indexArray; i++) {
            for (int j = 0; j < 20; j++) {
                if (parkingSpots[i]->parkingIntervals[j] != NULL) {
                    totalSum += parkingSpots[i]->parkingIntervals[j]->revenue;
                }
            }
        }
    fprintf(stdout, "Total revenue from the parking is %.2f BGN.\n", totalSum);
}

int main() {
    int indexInArray = 0; //ParkingSpot index
    struct ParkingSpot* parkingSpots[30] = { NULL };

    CreateParkingSpot(1, false, "car", 2.5, &indexInArray, parkingSpots);
    CreateParkingSpot(2, false, "subscription", 0.0, &indexInArray, parkingSpots);

    ParkVehicle(1, "BG5678CD", 3, &indexInArray, parkingSpots);
    ParkVehicle(2, "BG1234AB", 5, &indexInArray, parkingSpots);

    GetParkingSpotById(1, parkingSpots, &indexInArray);
    GetParkingSpotById(2, parkingSpots, &indexInArray);

    ParkVehicle(2, "BG9876EF", 4, &indexInArray, parkingSpots);

    FreeParkingSpot(1, parkingSpots, &indexInArray);
    FreeParkingSpot(2, parkingSpots, &indexInArray);

    FreeParkingSpot(1, parkingSpots, &indexInArray);

    CalculateTotal(parkingSpots,&indexInArray);
    FreeMemory(parkingSpots, &indexInArray);
    return 0;
}

