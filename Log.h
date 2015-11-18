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

#define LOG_BUFFER_LEGNTH 50

    char LogBuffer[LOG_BUFFER_LEGNTH];

void MyLog(const char * msg, char level);

#ifdef	__cplusplus
}
#endif

#endif	/* LOG_H */

