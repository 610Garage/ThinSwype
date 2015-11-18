/* 
 * File:   main.c
 * Author: 640bob
 *
 * Created on November 12, 2015, 8:41 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <signal.h>

#include <string.h>
#include <ctype.h>

#include <nfc/nfc.h>

#include "utils/nfc-utils.h"
#include "utils/mifare.h"

#include "R_NFC.h"
#include "UserInfo.h"
#include <unistd.h>

static ReaderTag RT;
static mifareul_tag mtDump;

bool run = true;

static void stop_polling(int sig)
{
  (void) sig;
  run = false;
  if (RT.device != NULL)
    nfc_abort_command(RT.device);
  else {
    nfc_exit(RT.context);
    exit(EXIT_FAILURE);
  }
}

/*
 * 
 */
int main(int argc, char** argv) {
    
    memset(&mtDump, 0x00, sizeof(mtDump));
    Credentials crd;
    char cmd[200];
    
    signal(SIGINT, stop_polling);
    
    
    nfc_init(&RT.context);
    
    if (RT.context == NULL) {
          ERR("Unable to init libnfc (malloc)");
          exit(EXIT_FAILURE);
        }
        TagInit(&RT);
      
    while(run){
        
            if(ScanForTag(&RT)){
                if(N_read_card(RT,&mtDump)){
                    if(NDDEF_DEGenerate(&crd, &mtDump)){
                        if(Credentials_Check(&crd)){
                            printf("\nGood\n");
                            memset(cmd, 0x00, sizeof(cmd));
                            //snprintf(cmd,200,"startx /usr/bin/nice -n 18 /usr/bin/rdesktop -a 24 -x 6 -P -r sound:local -u %s -p %s -f %s",crd.user,crd.password, crd.VM);
                            //printf("%s",cmd);
                            //system(cmd);
                            printf("Waiting for card removing...");
                            sleep(1);
                            while (0 == nfc_initiator_target_is_present(RT.device, NULL)) {}
                            nfc_perror(RT.device, "nfc_initiator_target_is_present");
                            printf("done.\n");
                        }
                    }
                }
            }
        
        //nfc_idle(RT.device);
        //nfc_exit(RT.context);
        sleep(1);
    }
    if (RT.device != NULL)
        nfc_abort_command(RT.device);
    else {
      nfc_exit(RT.context);
      exit(EXIT_SUCCESS);
    }
}

