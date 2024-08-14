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

int calculateOverDueHours(Book* book) {
    struct tm currentDateTime;
    time_t now = time(NULL);
    currentDateTime = *localtime(&now);

    struct tm dueDateTime = {
        .tm_year = book->dueDate.year - 1900,
        .tm_mon = book->dueDate.month - 1,
        .tm_mday = book->dueDate.day,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };
    int hoursOverdue = calculateHoursDifference(&dueDateTime, &currentDateTime);
    return hoursOverdue > 0 ? hoursOverdue : 0;
}

// Calculates the difference between two times in hours
int calculateHoursDifference(struct tm *start, struct tm *end) {
    time_t startTime = mktime(start);
    time_t endTime = mktime(end);
    double difference = difftime(endTime, startTime);
    return (int)(difference / 3600); // Convert seconds to hours
}

int calculateFine(Book* book) {
    int hoursOverDue = calculateOverDueHours(book);
    int Fine = hoursOverDue * 6;
    if(Fine > MAXFINE) {
        Fine = MAXFINE;
    }
    return Fine;
}