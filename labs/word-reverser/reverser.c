#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char word[99999], reversed[99999999];
    int i = 0, j = 0;
    while((word[i] = getchar())!= EOF){
        if(word[i] == " "){
            for(int k = i-1; k >= 0; k--, j++){
                reversed[j] = word[i];
            }
            reversed[j] = " ";
            j++;
            i = 0;
        } else if(word[i] == "\n"){
            for(int k = i-1; k >= 0; k--, j++){
                reversed[j] = word[i];
            }
            reversed[j] = "\n";
            printf("%s", reversed);
        } else{
            i++;
        } 
    }
    return 0;
}
