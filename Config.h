/* 
 * File:   Config.h
 * Author: 610bob
 *
 * Created on November 17, 2015, 2:15 PM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#define DEFUALT_LOG_LEVEL 1
#define Defualt_RemoteStartCMD "startx /usr/bin/nice -n 18 /usr/bin/rdesktop -a 24 -x 6 -P -r sound:local -u \"%s\" -p \"%s\" -f \"%s\" 2>&1"
#define DEFUALT_REMOTE_STOP_CMD "pkill -15 rdesktop"

#define CONFIG_FOLDER "/usr/ThinSwype"
#define CONFIG_FILE "/usr/ThinSwype/Config"
#define CONSOLE_INPUT_BUFFER_LEGNTH 300
    
#define Max_CMD_LEGNTH 250 //must be less than CONSOLE_INPUT_BUFFER_LEGNTH
    
#include <inttypes.h>
#include <stdbool.h>
    
    typedef struct {
        int LogLevel;
        char RemoteStartCMD[Max_CMD_LEGNTH];
        char RemoteStopCMD[Max_CMD_LEGNTH];
    } Config;
    
void NewConfig();
bool readConfig(Config * conf);


#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

