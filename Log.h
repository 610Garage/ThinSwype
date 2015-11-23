/* 
 * File:   Log.h
 * Author: 610bob
 *
 * Created on November 17, 2015, 12:26 AM
 */

#ifndef LOG_H
#define	LOG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "Config.h"
    
#define LOG_BUFFER_LEGNTH 50
#define LOG_FILE "/usr/ThinSwype/Log"
#define LOG_FILE_ARCHIVE "/usr/ThinSwype/Log.archive"
#define MAX_LOG_SIZE 60000

    char LogBuffer[LOG_BUFFER_LEGNTH];

void LogInit(Config * c);
void MyLog(const char * msg, char level);

#ifdef	__cplusplus
}
#endif

#endif	/* LOG_H */

