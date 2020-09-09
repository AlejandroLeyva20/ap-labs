#include <stdio.h>
#include <stdlib.h>

int mystrlen(char *str){
    int aux = 0;
    while(str[aux] != '\0') {
      aux++; 
    } 
   return aux;
}

char *mystradd(char *origin, char *addition){
    int totalSize = mystrlen(origin)+mystrlen(addition);
    char *newStr = malloc(totalSize+1);
    int i=0,j=0;
    for(i=0; origin[i]!='\0'; i++) {
        newStr[i] = origin[i];      
    }
    for(i=i, j=0;addition[j]!='\0';i++, j++) {
        newStr[i] = addition[j];
    }
    return newStr;
}

int mystrfind(char *origin, char *substr){
    int s = mystrlen(substr), pos = 0;
    bool b = false, finished = false;
    for(int i=0, j=0; origin[i] != '\0';i++){
        if(origin[i]==substr[j]){
            j++;
            if(!b){
                pos = i;
                b = true;
            }
            if(j==s){
                finished = true;
                break;
            }
        }
    }
    if (finished==true){
        printf("['%s'] string was found at %d\n",substr, pos);
    }else{
        printf("Ths string was not found \n");
    }
    return 0;
}
