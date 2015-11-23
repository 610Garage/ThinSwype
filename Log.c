#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "Config.h"
#include "Log.h"

static Config * conf;

void LogInit(Config * c){
    conf = c;
    char msg[50];
    time_t rawtime;
    struct tm * timeinfo;
    
    struct stat FileInfo;
    stat(LOG_FILE, &FileInfo);
    
    if(FileInfo.st_size > MAX_LOG_SIZE){
        remove(LOG_FILE_ARCHIVE);
        rename(LOG_FILE,LOG_FILE_ARCHIVE);
    }

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    snprintf(msg,50,"\n\n\nThinSwype started at %s\n",asctime(timeinfo));
    MyLog(msg,1);
}
void MyLog(const char * msg, char level){
    
    FILE * pFile;
    pFile = fopen (LOG_FILE,"a+");
    if (pFile!=NULL){
        if(level <= conf->LogLevel){
            fprintf(pFile,"%s\n", msg);
        }
        fclose (pFile);
        if(level <= 1){
            printf("%s\n", msg);
        }
    }else{
        printf("Cant open log file message:\n%s\n",msg);
    }
    
    
    
    
}