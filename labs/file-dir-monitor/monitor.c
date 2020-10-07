#define _XOPEN_SOURCE 500
#include "logger.h"
#include <stdio.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ftw.h>
#include <unistd.h>

#define BUF_SIZE (10 * (sizeof(struct inotify_event) + 100 + 1))

char buf[BUF_SIZE] __attribute__ ((aligned(8)));
int inotifyIni, inotifyWatch;
struct inotify_event *event;
ssize_t l;
char *ptr;

static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
            if(ftwbuf->level <= 2){
                inotifyWatch = inotify_add_watch(inotifyIni, fpath, IN_CREATE | IN_DELETE | IN_MOVED_FROM);
                if (inotifyWatch == -1)
                    errorf("inotify_add_watch\n");
            }
            return 0;           /* To tell nftw() to continue */
       }

int main(int argc, char **argv){
    int flags = 0;
    inotifyIni = inotify_init();
    if(inotifyIni == -1) {
        errorf("inotify_init error\n");
    }
    if(argc == 2){
        if(nftw(argv[1], display_info, 20, flags) == -1) errorf("nftw error\n");
        infof("Starting File/Directory Monitor on %s\n", argv[1]);
        infof("-----------------------------------------------------\n");
    } else {
        errorf("Parameters are not valid\n");
    }
    
    while(1){
        l = read(inotifyIni, buf, BUF_SIZE);
        if(l == -1 || l == 0){
            errorf("Read error\n");
        }
        for (ptr = buf; ptr < buf + l; ptr += sizeof(struct inotify_event) + event->len) {
            event = (struct inotify_event *) ptr;
            if (event->mask & IN_CREATE) {
                    if (event->mask & IN_ISDIR) {
                        infof("- [Directory - Create] - %s\n", event->name);
                    }
                    else
                        infof("- [File - Create] - %s\n", event->name);
                }
                
                if (event->mask & IN_MOVED_FROM) {
                    if (event->mask & IN_ISDIR)
                        infof("- [Directory - Modified] - %s\n", event->name);
                    else
                        infof("- [File - Modified] - %s\n", event->name);
                }
                
                if (event->mask & IN_DELETE) {
                    if (event->mask & IN_ISDIR)
                        infof("- [Directory - Delete] - %s\n", event->name);
                    else
                        infof("- [File - Delete] - %s\n", event->name);
                }            
        }  
    }
    (void) close(inotifyIni);
    return 0;
}
