#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

void mergeSort(void *arr[], int left, int right, int (*comp)(void *, void *));
int numcmp(const char *s1, const char *s2);

int main(int argc, char **argv)
{
    int numeric = 0, i = 0, len = 0, lRead;
    char *data[1000], *buffer = NULL;
    char *str;
    size_t size = 0;
    FILE *file;
    if(argc > 1 && (strcmp(argv[1], "-n") == 0)){
        numeric = 1;
        file = fopen(argv[2], "r");
    } else if(argc == 2 && (strcmp(argv[1], "-n") != 0)){
        file = fopen(argv[1], "r");
    } else {
        printf("missing parameters\n");
        return 0;
    }
    if(file == NULL){
        printf("Open file was not possible.\n");
        return 0;   
    }
    int lines = getline(&buffer, &size, file);
    int totLines = lines;
    while(lines >= 0){
        data[len] = buffer;
        len++;
        lines = getline(&buffer, &size, file);
    }
    fclose(file);
    mergeSort((void **) data, 0, totLines-1, (int (*)(void*, void*))(numeric ? numcmp : strcmp));
    if(argc > 1 && (strcmp(argv[1], "-n") == 0)){
        numeric = 1;
        file = fopen(argv[2], "w");
    } else if(argc == 2 && (strcmp(argv[1], "-n") != 0)){
        file = fopen(argv[1], "w");
    }
    for(int i = 0; i < len; i++){
        fputs(data[i], file);
    }
    fclose(file);
    return 0;
}

void merge(void *arr[1000], int left, int m, int right, int (*comp)(void *, void *)){
    int i, j, k; 
    int n1 = m - left + 1; 
    int n2 = right - m;

    char L[n1][1000], R[n2][1000];

    for (i = 0; i < n1; i++) 
        strcpy(L[i], arr[left+i]);
    for (j = 0; j < n2; j++) 
        strcpy(R[j], arr[m+1+j]);

    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2){ 
        if ((*comp)(L[i], R[j]) <= 0){ 
            strcpy(arr[k], L[i]);
            i++; 
        } 
        else { 
            strcpy(arr[k], R[j]);
            j++; 
        } 
        k++; 
    }
    while (i < n1){ 
        strcpy(arr[k], L[i]); 
        i++; 
        k++; 
    }
    while (j < n2){ 
        strcpy(arr[k], R[j]); 
        j++; 
        k++; 
    } 
}

void mergeSort(void *arr[1000], int left, int right, int (*comp)(void *, void *)){
    if(left < right){
        int m = left+(right-left)/2;
        mergeSort(arr, left, m, comp);
        mergeSort(arr, m+1, right, comp);
        merge(arr, left, m, right, comp);
    }
}

int numcmp(const char *s1, const char *s2){
    double v1, v2;
    v1 = atof(s1);
    v2 = atof(s2);
    if(v1 < v2)
        return -1;
    else if(v1 > v1)
        return 1;
    else return 0;
}
