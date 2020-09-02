#include <stdio.h>
#include <stdlib.h>

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday){
    int leapYear, d, i, days[2][13] = {{0,31,28,31,30,31,30,31,31,30,31,30,31},{0,31,29,31,30,31,30,31,31,30,31,30,31}};
    if(year%4 == 0 && year%100 != 0 || year%400 == 0) {
        leapYear = 1;
    } else leapYear = 0;
    for(i = 1; i <= 12 || d<yearday; i++){
        d += days[leapYear][i];
    }
    *pmonth = --i;
    *pday = days[leapYear][i] - (d - yearday);
}

int main() {
    int year = atoi(argv[1]);
    int day = atoi(argv[2]);
    int pmonth, pday;
    char months[13] = {"","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    if(day < 1 || day > 366 || (!(year%4 == 0 && year%100 != 0 || year%400 == 0) && day > 365)){
        printf("Invalid input");
        return 0;
    }
    month_day(year, day, &pmonth, &pday);
    printf("%s %d, %d", months[pmonth], pday, year);
    return 0;
}

