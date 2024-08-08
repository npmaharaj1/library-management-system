#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // this will be needed for date handling later
#include <ncurses.h> // For terminal control for UI
#include "otherFunctions.h"

void getFutureDate (Date* futureDate, int daysIntoFuture) {
    // Get the current time
    time_t t = time(NULL);
    struct tm *currentTime = localtime(&t);
    
    //Add specified number of days to the current date
    currentTime->tm_mday += daysIntoFuture;

    // Normalise the time structure (month/year overflow)
    mktime(currentTime);

    // Populate the futureDate struct with the new date
    futureDate->day = currentTime->tm_mday;
    futureDate->month = currentTime->tm_mon + 1; // tm_mon is 0 indexed
    futureDate->year = currentTime->tm_year + 1900; // tm_year is years since 1900
}