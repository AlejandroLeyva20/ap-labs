#include <stdio.h>
#include <string.h>

int mystrlen(char *str);
char *mystradd(char *origin, char *addition);
int mystrfind(char *origin, char *substr);

int main(int argc, char *argv[]) {
    if(argc < 4){
        printf("Insufficient arguments.\n")
    } else if(argc > 4){
        printf("Too many arguments.\n")
    } else{
        if(strcmp(argc[1],"-add") == 0){
            mystrfind(argv[2],argv[3]);
        } else if(strcmp(argc[1],"-find") == 0){
            printf("Initial Lenght: %d\nNew String: %s\nNew length: %d\n", mystrlen(argv[2]), mystradd(argv[2],argv[3]), mystrlen(mystradd(argv[2],argv[3])));
        }
    }
    return 0;
}
