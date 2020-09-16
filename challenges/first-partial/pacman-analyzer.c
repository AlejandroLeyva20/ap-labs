#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

typedef struct Package Package;

struct Package{
    char name[50];
    char installDate[50];
    char lastUpdate[50];
    int updates;
    char removalDate[50];
};

void analizeLog(char *logFile, char *report);
void extractCase(char *source, char *dest, int i);
int getPos(char *source, char *name, Package *package, int len, int i);
void getDate(char *source, char *dest, int i);

int main(int argc, char **argv) {

    if (argc != 5) {
	printf("Usage:./pacman-analyzer -input pacman.txt -report packages_report.txt\n");
	return 0;
    }

    analizeLog(argv[2], argv[4]);

    return 0;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    int file;
    file = open(logFile, O_RDONLY);
    if(file < 0){
        printf("Open file was not possible.\n");
        return;;
    }

    int installed, upgraded, removed, i, lRead, pos, len, posIni, b;
    installed = upgraded = removed = i = 0, len = 0;
    Package package[10000];
    char buffer[1], log[10000], subS[50], name[50], date[50], sInstalled[5], sUpgraded[5], sRemoved[5], sCurrInstalled[5], sUpdates[5];
    posIni = 0;
    b = 0;
    while(1){
        lRead = read(file, buffer, 1);
        if(lRead < 0){
            printf("An error occurred.\n");
            return;
        }

        if(lRead == 0) break;

        if(buffer[0] == '\n'){
            log[i] = '\0';

            extractCase(log, subS, 26 + posIni);
            if(!strcmp(subS, "installed")){
                installed++;
                pos = getPos(log, name, package, len, 36 + posIni);
                getDate(log, date, 1);
                if(pos < 0){
                    strcpy(package[len].name, name);
                    strcpy(package[len].installDate, date);
                    len++;
                } else {
                    strcpy(package[pos].installDate, date);
                    strcpy(package[pos].removalDate, "");
                }
            } else if(!strcmp(subS, "upgraded")){
                upgraded++;
                pos = getPos(log, name, package, len, 35 + posIni);
                getDate(log, date, 1);
                strcpy(package[pos].lastUpdate, date);
                package[pos].updates++;
            } else if(!strcmp(subS, "removed")){
                removed++;
                pos = getPos(log, name, package, len, 34 + posIni);
                getDate(log, date, 1);
                strcpy(package[pos].removalDate, date);
            }
            i = 0;
        } else {
            if(buffer[0] == ']' && i == 25 && !b) {
                b = 1;
                posIni = 8;
            }
            log[i] = buffer[0];
            i++;
        }
    }
    close(file);
    remove(report);
    file = open(report, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if(file < 0){
        printf("Open file was not possible.\n");
        return;;
    }
    sprintf(sInstalled, "%d", len);
    sprintf(sUpgraded, "%d", upgraded);
    sprintf(sRemoved, "%d", removed);
    sprintf(sCurrInstalled, "%d", len-removed);

    write(file, "Pacman Packages Report\n----------------------\n- Installed packages : ", strlen("Pacman Packages Report\n----------------------\n- Installed packages : "));
    write(file, sInstalled, strlen(sInstalled));
    write(file, "\n", strlen("\n"));
    write(file, "- Removed packages   : ", strlen("- Removed packages   : "));
    write(file, sRemoved, strlen(sRemoved));
    write(file, "\n", strlen("\n"));
    write(file, "- Upgraded packages  : ", strlen("- Upgraded packages  : "));
    write(file, sUpgraded, strlen(sUpgraded));
    write(file, "\n", strlen("\n"));
    write(file, "- Current installed  : ", strlen("- Current installed  : "));
    write(file, sCurrInstalled, strlen(sCurrInstalled));
    write(file, "\n\nList of packages\n----------------\n", strlen("\n\nList of packages\n----------------\n"));

    for(int i = 0; i < len; i++){
        write(file, "- Package Name        : ", strlen("- Package Name        : "));
        write(file, package[i].name, strlen(package[i].name));
        write(file, "\n", strlen("\n"));
        write(file, "  - Install date      : ", strlen("  - Install date      : "));
        write(file, package[i].installDate, strlen(package[i].installDate));
        write(file, "\n", strlen("\n"));
        write(file, "  - Last update date  : ", strlen("  - Last update date  : "));
        if(strcmp(package[i].lastUpdate,"")) write(file, package[i].lastUpdate, strlen(package[i].lastUpdate));
        else write(file, "-", strlen("-"));
        write(file, "\n", strlen("\n"));
        sprintf(sUpdates, "%d", package[i].updates);
        write(file, "  - How many updates  : ", strlen("  - How many updates  : "));
        write(file, sUpdates, strlen(sUpdates));
        write(file, "\n", strlen("\n"));
        write(file, "  - Removal date      : ", strlen("  - Removal date      : "));
        if(strcmp(package[i].removalDate,"")) write(file, package[i].removalDate, strlen(package[i].removalDate));
        else write(file, "-", strlen("-"));
        write(file, "\n", strlen("\n"));
    }
    close(file);
    printf("Report is generated at: [%s]\n", report);
}

void extractCase(char *source, char *dest, int i){
    int j;
    for(j = 0; source[i] != ' '; i++, j++){
        dest[j] = source[i];
    }
    dest[j] = '\0';
}

int getPos(char *source, char *name, Package *package, int len, int i){
    int j;
    for(j = 0; source[i] != ' '; i++, j++){
        name[j] = source[i];
    }
    name[j] = '\0';
    for(int i = 0; i < len; i++){
        if(!strcmp(package[i].name, name)){
            return i;
        }
    }
    return -1;
}

void getDate(char *source, char *dest, int i){
    int j;
    for(j = 0; j < 16; i++, j++){
        if(source[i] != ' ' && i == 11) dest[j] = ' ';
        else dest[j] = source[i];
    }
    dest[j] = '\0';
}
