#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/stat.h>

#include "Config.h"


char consoleInput[CONSOLE_INPUT_BUFFER_LEGNTH];

char FileLine[300];

/**
 * Get a single line from the counsole and place it into the buffer
 * @param buffer
 * A pointer to the buffer that the line will be stored
 * @return 
 * returns the number of elements in the buffer
 */
int GetCLine(char * buffer){
    int c = 0;//holds a character form the counsole stream
    int count = 0;//number of chars in buffer
    
    memset(buffer, 0x00, CONSOLE_INPUT_BUFFER_LEGNTH);//clean the buffer
    
    while((c=fgetc(stdin)) != '\n' && count < CONSOLE_INPUT_BUFFER_LEGNTH){//get the char from console, see if the line is over, make sure were not overruning buffer
        buffer[count++] = (char)c;//throw the char into the buffer and incrament the char counter/buffer position
    }
    buffer[count++] = '\n';//the while loop discards the new line char, so lets pop it back in
    return(count);//tell the caller the number of chars in buffer
}

/**
 * Get a line from the file
 * @param buffer
 * The buffer that will store the line
 * @param pFile
 * the pointer to the open file
 * @return 
 * the number of chars in the buffer
 */
int GetFLine(char* buffer, FILE * pFile){
   
   int count = 0;//buffer char count
   int c;//hold char from file to buffer
   
    while((c=fgetc(pFile)) != '\n' && count < 300 && c!= EOF){//get a char from file, see if we hit end of line, make sure were not overrunning the buffer, and see if we hit the end of the file
        buffer[count++] = c;//add char to buffer
    }
    if(count >= 299){//if we maxed out, we didn't get anything useful
        return(0);//report that we got nuthin
    }
   if(buffer[0] == '#'){//is this a comment?
       return(0);//then we got nothin
   }
   if(c == EOF && count <= 1){//if we hit end of file with nothin in the buffer
       return(EOF);//we got less then nothin
   }
   return(count);//everything is good, report the number of chars we got
       
}

/**
 * Figures out what the line contains and adds the value to the config strct
 * @param line
 * buffer that holds the line data
 * @param count
 * amount of chars in the line buffer
 * @param cf
 * a pointer to the config struct
 * 
 * To do
 * Add fail safes
 */
void ParseFileLine(char * line, int count, Config * cf, FILE * pFile){
    if(!memcmp(line,"Log Level=", sizeof("Log Level=")-1)){//checks to see if the line begines with "Log Level="
        cf->LogLevel = atoi(&line[sizeof("Log Level=")-1]);//it does, so copy everything past "Log Level=" to the config strut
        return;//thats all
    }
    
    //just like the first one, but for the other varibles
    if(!memcmp(line,"Remote start CMD=",sizeof("Remote start CMD=")-1)){
        memset(cf->RemoteStartCMD, 0x00, Max_CMD_LEGNTH);
        memcpy(cf->RemoteStartCMD, &line[sizeof("Remote start CMD=")-1], count - (sizeof("Remote start CMD=")-1));
        return;
    }
    
    if(!memcmp(line,"Remote stop CMD=", sizeof("Remote stop CMD=")-1)){
        memset(cf->RemoteStopCMD, 0x00, Max_CMD_LEGNTH);
        memcpy(cf->RemoteStopCMD, &line[sizeof("Remote stop CMD=")-1], count- (sizeof("Remote stop CMD=")-1));
        return;
    }
    
    
    if(!memcmp(line,"enc=", sizeof("enc=")-1)){
        //Its the encryption data, this needs to be handled diferently
        fseek(pFile,-(count- (sizeof("enc=")-1)),SEEK_CUR);//find the begining of the data
        fread(&cf->key, sizeof(char), sizeof(cf->key),pFile );//then read the file to the KEY block
        return;
    }
    
}
/**
 * create a new config file
 * @param conf
 * a pointer to the config file strut
 */
void UpdateConfigFile(Config * conf){
    FILE * pFile;//contains the file stuff
    char buffer[300];//the buffer that contains 
        
    pFile = fopen(CONFIG_FILE, "wb+");//open the file with write and creation permitions
    
    if(pFile == NULL){//check to see if the file exists
        printf("Couldn't open file");//let the user know that we couldn't go any further
        exit(EXIT_FAILURE);//stop the program
    }
    
    memset(buffer,0x00,200);//clear the buffer
    snprintf(buffer, 300, "Log Level=%i\n",conf->LogLevel);//Set up the line that will be writen to the file
    fputs(buffer, pFile);//write the line to the file
    
    //same for the rest
    memset(buffer,0x00,200);
    snprintf(buffer, 300, "Remote start CMD=%s\n",conf->RemoteStartCMD);
    fputs(buffer, pFile);
    
    memset(buffer,0x00,200);
    snprintf(buffer, 300, "Remote stop CMD=%s\n",conf->RemoteStopCMD);
    fputs(buffer, pFile);
    
    memset(buffer,0x00,200);
    snprintf(buffer, 300, "enc=");
    memcpy(&buffer[4],&conf->key, sizeof(conf->key));
    buffer[4+sizeof(conf->key)+1] = '\n';
    fwrite(buffer,sizeof(char),4+sizeof(conf->key)+1, pFile);
    
   
    fclose(pFile);//close the file
}

/**
 * Get the new configuration varibles from the user
 * @param conf
 * pointer to the config struct
 */
void NewConfig(Config * conf){
    int count;
    FILE * pFile;
    
    struct stat FolderBuffer;
    
     if(stat(CONFIG_FOLDER,&FolderBuffer) == -1){//if the folder don't exist
        if(mkdir(CONFIG_FOLDER, 0777) < 0){//then create it
            perror("mkdir");//it failed, tell the user
            exit(EXIT_FAILURE);//end end the program
        }
    }
    
    pFile = fopen(CONFIG_FILE, "w+");//open the file with write creation permisions
    
    if(pFile == NULL){//check to see if actully opened the file
        printf("Couldn't open file, Probably don't have su privaleges");//tell the user that we could
        exit(EXIT_FAILURE);//nothing more to do
    }
    fclose(pFile);//close the file, we simply opened it to test
    
    printf("Config file missing or curupt. Please answer quistions to generate new config.\n\n");
    printf("Please enter log level, 1-3. 1 = criticle, 3 = debug\n or press enter for defualt. [%i]\n",DEFUALT_LOG_LEVEL);
    
    while(1){//loop until we get the correct data
        count = GetCLine(consoleInput);//grab a line
        if(consoleInput[0] == '\n'){//if the input is empty, use defualt valubles
            conf->LogLevel = DEFUALT_LOG_LEVEL;//set the defualt value
            break;//we go the data, lets leave
        }
        conf->LogLevel = atoi(consoleInput);//get the int from the user
        
        if(conf->LogLevel > 3 || conf->LogLevel < 1){//make suere the int is within range
            printf("out of range. Pick a number between 1 and 3.\n");//tell the user there an idiot
        }else{
            break;//were good
        }
    }
    
    printf("Enter the cmd that will start the remote connection.\nRemeber, it must follow printf anotation\n");
   
    while(1){//loop until we get the correct data
        count = GetCLine(consoleInput);//grab a line
        if(count > Max_CMD_LEGNTH){//make sure that the cmd is not too long
            printf("CMD too long, must be less than %i\n", Max_CMD_LEGNTH);//its too long, tell the user that there an idiot
        }else if(consoleInput[0] == '\n'){//else check to see if we got an empty line
            memcpy(conf->RemoteStartCMD, Defualt_RemoteStartCMD, sizeof(Defualt_RemoteStartCMD));//set defualt
            break;//we got data, tie to leave the while loop
        }else{
            memcpy(conf->RemoteStartCMD, consoleInput, count);//copy user input to the config strut
            break;//we got data, time to leave
        }
    }
    
    printf("Enter the cmd that will stop the remote connection.\nRemeber, it must follow printf anotation\n");
    
    while(1){//same as above ^^^^
        count = GetCLine(consoleInput);
        if(count > Max_CMD_LEGNTH){
            printf("CMD too long, must be less than %i\n", Max_CMD_LEGNTH);
        }else if(consoleInput[0] == '\n'){
            memcpy(conf->RemoteStopCMD, DEFUALT_REMOTE_STOP_CMD, sizeof(DEFUALT_REMOTE_STOP_CMD));
            break;
        }else{
            memcpy(conf->RemoteStopCMD, consoleInput, count);

            break;
        }
    }
    
    printf("Enter password\n");
    
    while(1){//same as above ^^^^
        count = GetCLine(consoleInput);
        if(count > MAX_PASSWORD_LEGNTH){
            printf("Password too long, needs to be shorter than %i\n",MAX_PASSWORD_LEGNTH);
        }else if(consoleInput[0] == '\n'){
            printf("No defualt available. DUH!\n");//theres no defualt password
        }else{
            consoleInput[count-1] = 0x00;//get rid of the new line character
            keyGen(&conf->key, consoleInput);//generate the keys from the password
            break;
        }
    }
    
    
    printf("done\n");
    UpdateConfigFile(conf);//we got the new data, update the config file.
}

/**
 * read config file and set the config strut, if the file dosn't exist, creat it
 * @param conf
 * pointer to the config stru
 * @return 
 * weahter we were sucseful or not
 */
bool readConfig(Config * conf){
   FILE * pFile;
   int count = 0;
    
   pFile = fopen (CONFIG_FILE , "rb");//open the config file with read only permisions
   if (pFile == NULL){
       perror("fopen");//it proably dosn't exist, but tell the user the problem anyway
       NewConfig(conf);//lets creat a new config file
   }else{//file is good, lets get reading
       
       while((count = GetFLine(FileLine, pFile)) != EOF){//make sure were not at the end of the file
           if(count > 0){//make usre that we have data
               ParseFileLine(FileLine, count, conf, pFile);//then parse said data into the conf struct
           }
       }
        
        fclose (pFile);//clean up
   }
}