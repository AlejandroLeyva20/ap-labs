#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <syslog.h>
#include "logger.h"

int lstdout = 1;

void text(int a, int fg, int bg){
    char c[20];

	sprintf(c,"%c[%d;%d;%dm", 0x1B, a, fg + 30, bg + 40);
	printf("%s", c);
}

int initLogger(char *logType) {
    printf("Initializing Logger on: %s\n", logType);
    if(!strlen(logType) || !strcmp(logType, "stdout")) {
        lstdout = 1;
    } else if(!strcmp(logType, "syslog")) {
        lstdout = 0;
    }

    return lstdout;
}

int infof(const char *format, ...) {
    text(1, 2, 0);
	va_list a;
	va_start(a, format);
    if(lstdout) {
	    vprintf(format, a);
    } else {
        openlog("SYSLOG", 0, 0);
        vsyslog(LOG_INFO, format, a);
        closelog();
    }
	va_end(a);
	text(0, 7, 0);
	return 0;
}

int warnf(const char *format, ...) {
    text(1, 3, 0);
	va_list a;
	va_start(a, format);
	if(lstdout) {
	    vprintf(format, a);
    } else {
        openlog("SYSLOG", 0, 0);
        vsyslog(LOG_WARNING, format, a);
        closelog();
    }
	va_end(a);
	text(0, 7, 0);
    return 0;
}

int errorf(const char *format, ...) {
    text(1, 1, 0);
	va_list a;
	va_start(a, format);
	if(lstdout) {
	    vprintf(format, a);
    } else {
        openlog("SYSLOG", 0, 0);
        vsyslog(LOG_ERR, format, a);
        closelog();
    }
	va_end(a);
	text(0, 7, 0);
    return 0;
}
int panicf(const char *format, ...) {
    text(1, 1, 0);
	va_list a;
	va_start(a, format);

	if(lstdout) {
	    vprintf(format, a);
    } else {
        openlog("SYSLOG", 0, 0);
        vsyslog(LOG_EMERG, format, a);
        closelog();
    }
	va_end(a);
	text(0, 7, 0);
	raise (SIGABRT);
    return 0;
}

