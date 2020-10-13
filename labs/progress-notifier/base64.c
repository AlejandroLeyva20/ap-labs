#include <stdio.h>
#include "logger.h"
#include <stdlib.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void encode(char *fileName);
void decode(char *fileName);
char* base64_encode(char* plain);
char* base64_decode(char* cipher);
void progressSig(int sig);
void quitSig(int sig);

int fileSize, bytes;
char base46_map[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                     'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                     'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                     'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

int main(int argc, char **argv){
    if(argc != 3){
        errorf("Unsupported number of parameters.");
    }

    if(signal(SIGINT, progressSig) == SIG_ERR) {
        errorf("Signal error.\n");
        return -1;
    }

    if(signal(SIGUSR1, quitSig) == SIG_ERR) {
        errorf("Signal error.\n");
        return -1;
    }   

    if(!strcmp(argv[2],"--encode")){
        encode(argv[2]);
    } else if(!strcmp(argv[2],"--decode")){
        decode(argv[2]);
    } else {
        errorf("Unsupported parameter.");
    }
    return 0;
}

void encode(char *fileName){

    int src = open(fileName, O_RDONLY);
    if(src < 0) {
        errorf("Source file.\n");
        return;
    }
    int dst = open("encoded.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(dst < 0) {
        errorf("Destination file.\n");
        return;
    }

    fileSize = lseek(src, 0, SEEK_END);
    if(fileSize == -1) {
        errorf("Error with file length.\n");
        return;
    }
    if(lseek(src, -fileSize, SEEK_END) == -1) {
        errorf("Error returning to the beginning of the file.\n");
        return;
    }

    char data[129];
    char *encoded;
    int bytesRd = -1;

    while((bytesRd = read(src, data, 128)) > 0) {
        data[bytesRd] = '\0';
        encoded = base64_encode(data);

        if(write(dst, encoded, strlen(encoded)) < 0) {
            errorf("Error writing.\n");
            return;
        }

        bytes += bytesRd;
    }

    if(bytesRd < 0) {
        errorf("Error reading file.\n");
        return;
    }

    if(close(src) < 0) {
        errorf("Unable to close source file.\n");
        return;
    }

    if(close(dst) < 0) {
        errorf("Unable to close destination file.\n");
        return;
    }
}

void decode(char *fileName){

    int src = open(fileName, O_RDONLY);
    if(src < 0) {
        errorf("Source file.\n");
        return;
    }
    int dst = open("decoded.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(dst < 0) {
        errorf("Destination file.\n");
        return;
    }

    fileSize = lseek(src, 0, SEEK_END);
    if(fileSize == -1) {
        errorf("Error getting file length.\n");
        return;
    }

    if(lseek(src, -fileSize, SEEK_END) == -1) {
        errorf("Error returning to the beginning of the file.\n");
        return;
    }

    char data[129];
    char *decoded;
    int bytesRd = -1;

    while((bytesRd = read(src, data, 128)) > 0) {
        data[bytesRd] = '\0';
        decoded = base64_decode(data);

        if(write(dst, decoded, strlen(decoded)) < 0) {
            errorf("Error writing.\n");
            return;
        }

        bytes += bytesRd;
    }
    
    if(bytesRd < 0) {
        errorf("Error reading.\n");
        return;
    }

    if(close(src) < 0) {
        errorf("Unable to close source file.\n");
        return;
    }

    if(close(dst) < 0) {
        errorf("Unable to close destination file.\n");
        return;
    }
}

char* base64_encode(char* plain) {

    char counts = 0;
    char buffer[3];
    char* cipher = malloc(strlen(plain) * 4 / 3 + 4);
    int i = 0, c = 0;

    for(i = 0; plain[i] != '\0'; i++) {
        buffer[counts++] = plain[i];
        if(counts == 3) {
            cipher[c++] = base46_map[buffer[0] >> 2];
            cipher[c++] = base46_map[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
            cipher[c++] = base46_map[((buffer[1] & 0x0f) << 2) + (buffer[2] >> 6)];
            cipher[c++] = base46_map[buffer[2] & 0x3f];
            counts = 0;
        }
    }

    if(counts > 0) {
        cipher[c++] = base46_map[buffer[0] >> 2];
        if(counts == 1) {
            cipher[c++] = base46_map[(buffer[0] & 0x03) << 4];
            cipher[c++] = '=';
        } else {                      // if counts == 2
            cipher[c++] = base46_map[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
            cipher[c++] = base46_map[(buffer[1] & 0x0f) << 2];
        }
        cipher[c++] = '=';
    }

    cipher[c] = '\0';   /* string padding character */
    return cipher;
}

char* base64_decode(char* cipher) {

    char counts = 0;
    char buffer[4];
    char* plain = malloc(strlen(cipher) * 3 / 4);
    int i = 0, p = 0;

    for(i = 0; cipher[i] != '\0'; i++) {
        char k;
        for(k = 0 ; k < 64 && base46_map[k] != cipher[i]; k++);
        buffer[counts++] = k;
        if(counts == 4) {
            plain[p++] = (buffer[0] << 2) + (buffer[1] >> 4);
            if(buffer[2] != 64)
                plain[p++] = (buffer[1] << 4) + (buffer[2] >> 2);
            if(buffer[3] != 64)
                plain[p++] = (buffer[2] << 6) + buffer[3];
            counts = 0;
        }
    }

    plain[p] = '\0';    /* string padding character */
    return plain;
}

void progressSig(int sig)
{
    infof("Current progress: %.2f\%\n", (bytes * 100.0) / fileSize);
    signal(sig, progressSig);
}

void quitSig(int sig)
{
    infof("Exiting program.\n");
    exit(1);
}