/* 
 * File:   main.c
 * Author: 610bob
 *
 * Created on November 12, 2015, 8:41 PM
 */
#include <signal.h>
#include <unistd.h>

#include <nfc/nfc.h>

#include "R_NFC.h"
#include "UserInfo.h"
#include "Log.h"
#include "Config.h"

using namespace std;

static ReaderTag RT;//contains libnfcs context, device, and tag info
mifareul_tag mtDump;//varible that stores the data on the tag

Credentials crd;//holds the user name, password, and vm adress
static Config conf; //holds the configuraton read from the config file, needs to be static. Mabey somthing else uses a varible called conf

static bool run = true;//keeps the main loop alive until we want IT DEAD!!!!!


void StartRDesktop(Credentials *crd, Config *cf){//Well, its the whole reason for the app, lets get to it.
    char cmd[200];// temp holding buffer for the comand to be exicuted
    memset(cmd, 0x00, sizeof(cmd));//I like clean slaights
    //the comand with the loging credentials
    snprintf(cmd,200,cf->RemoteStartCMD,crd->user,crd->password, crd->VM);
    MyLog(cmd,3);//Log, just incase stuff goes goofy
    popen(cmd, "r");//Exicute the comand and be amazed AT THE POWER OF THE TAG!!!
    //while(1);
}

void StopRDesktop(Config *cf){//
    char cmd[200];// temp holding buffer for the comand to be exicuted
    memset(cmd, 0x00, sizeof(cmd));//I like clean slaights
    //The death comands
    snprintf(cmd,200,cf->RemoteStopCMD);
    MyLog(cmd,3);//Log, just incase stuff goes goofy
    popen(cmd, "r");//Exicute the comand to exicute the remote desktop
}

static void stop_polling(int sig){//sombody wants out, lets close out nicely
    
  (void) sig;
  run = false;//Kill,kiLL, KILL THE MAIN LOOP!!!!!!!!
  //Lets stop any comands currently sent to the reader, if we don't it won't start back up nicly
  if (RT.device != NULL)//Make sure were not talking to a zombie
    nfc_abort_command(RT.device);//CLEAR!!!!
  else {//Well, no comand to stop, lets close out
    nfc_exit(RT.context);//release stuff, I assume
    StopRDesktop(&conf);//Lets leave a clean enviroment. Stop the Remote desktop!
    exit(EXIT_SUCCESS);//and were done
  }
}

/*
 * 
 */
int main(int argc, char** argv) {//Realy, realy, you need a coment for this? Go back to c 101 :)
    
    memset(&mtDump, 0x00, sizeof(mtDump));//clear out the variible
    
    signal(SIGINT, stop_polling);//register the exit function
    
    readConfig(&conf);//get the configuration
    
    
    nfc_init(&RT.context);//init libnfc
    
    if (RT.context == NULL) {//well, that went wrong
       MyLog("Unable to init libnfc (malloc)", 0);//say it
       exit(EXIT_FAILURE);//then quit it. nothing more to do here
    }
    
    TagInit(&RT);//init the tag reader
    
    if(RT.device == NULL){//no tag reader, no party. Say it and quit it.
        MyLog("Tag reader == null", 0);
        exit(EXIT_FAILURE);
    }
    
    while(run){//loop until someone says stop
        
            if(ScanForTag(&RT)){//start polling for a tag
                if(N_read_card(RT,&mtDump)){//found a tag, good. Whats on it though. Is it readable?
                    if(NDDEF_DEGenerate(&crd, &mtDump,&conf)){//Great, its readable. Lets get that ndef info off. If it is ndef info that is.
                        if(Credentials_Check(&crd)){//YEa, its ndef info, and its good. But are the credentials complete and secure?
                            MyLog("Card credentials are good", 3);//yup, good credentials, tell the world, well the log anyway.
                            
                            StopRDesktop(&conf);//kill any running remote desktops
                            sleep(1);//give it a second..... literly
                            StartRDesktop(&crd, &conf);//start the remote desktop with the new credentials
                            MyLog("VM loded, all done", 3);//tell the log, if anyone cares
                        }
                    }
                }
                while (0 == nfc_initiator_target_is_present(RT.device, NULL)) {}// no need to reread a tag. Whight for it to go beybey.
            }
        
    }
    
    //Well, were done here. Lets clean up.
    if (RT.device != NULL){//Are we talking to a zombie?
        nfc_abort_command(RT.device);//stop any pending comands to the tag reader
    }
    nfc_exit(RT.context);//release stuff, I assume
    StopRDesktop(&conf);//Lets leave a clean enviroment. Stop the Remote desktop!
    exit(EXIT_SUCCESS);//exit, its all good
    
}