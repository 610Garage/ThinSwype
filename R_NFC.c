#include <inttypes.h>

#include <nfc/nfc-types.h>

#include "R_NFC.h"
#include "Log.h"
#include "EDCrypt.h"
#include "Config.h"


char mdefMSG[MAX_MDEF_MSG_LENGTH];
static mifare_param mp;

const nfc_modulation nmModulations[] = {
  { .nmt = NMT_ISO14443A, .nbr = NBR_106 },
  { .nmt = NMT_ISO14443B, .nbr = NBR_106 },
};

int res;


/**
 * 
 * This takes the data directly off of the tag, rips out the first ndef msg, then cuts out the credential struct that the ohter porgram so kindly assmbled for us.
 * 
 * @param Payload 
 * pointer to the location that will hold the fresh new credentials
 * 
 * @param tag
 * pointer to the tags data
 * 
 * @return true/false
 * returns wehter it was sucesfull or a miserable failer
 * 
 */

bool NDDEF_DEGenerate(Credentials * Payload, mifareul_tag * tag, Config * cf){
    int DPlegnth;//Length of the decrypted message
    char buffer[100];//buffer to store tag payload
    char buffer2[MAX_MDEF_MSG_LENGTH];//buffer to store ndef payload
    char * TLV_start;//pointer to the start of the TLV header
    char NDEF_size;//size of just the NDEF message
    unsigned char DecryptedBuffer[140];//a buffer that stores the decrypted data
    
    //clean the buffer with acid, or empty space
    memset(DecryptedBuffer, 0x00, 140);
    memset(buffer2,0x00,MAX_MDEF_MSG_LENGTH);
    
    for(int i=1; i<8;i++){//itterate through all pages and laod them into a single buffer
        memcpy(&buffer[i*16-16],tag->amb[i].mbd.abtData,16);//coppy the tag info, 16 bytes at a time. DOn't know why 16 though.
    }
    
    
    TLV_start = memchr(buffer, 0x03,80);//Look for the first instance of the TLV header
    NDEF_size = TLV_start[1];//Get the ndef message size from the tlv header
    if(NDEF_size > MAX_MDEF_MSG_LENGTH){//Do we an ndef message that looks like our ndef message?
        //run for the hills, its bad!!!!
        return(false);
    }
    memcpy(buffer2,&TLV_start[2],NDEF_size);//copy just the ndef message to the second buffer. use the mesage size we got erlier.
    
    //lets be as sure as possible that were deiling with good data. This makes sure that the ndef header looks like the ndef header we expect.
    if(buffer2[0] != DefualtHeader[2] || buffer2[1] != DefualtHeader[3] || buffer2[3] != DefualtHeader[5]){
        //Well somthing went wrong... RUNN!!!!
        return(false);
    }
      
    DPlegnth = decrypt(&buffer2[4], (int)buffer2[2], cf->Key, cf->IV,DecryptedBuffer);//decrypt the payload
    
    memcpy(Payload,DecryptedBuffer, DPlegnth);//coppy the ndef payload to the credentials struct. Buffer[2] holds the message legnth.]
    return(true);//Its all good, let the caller know
}

/**
 * This is an adaptation from libndef examples. It reads the card and returns true/false on its sucsess
 * @param rt
 * @param tag
 * @return 
 */
bool N_read_card(ReaderTag rt,mifareul_tag * tag){
    
  uint32_t page;
  bool    bFailure = false;
  uint32_t uiBlocks = 30;
  
  memset(LogBuffer, 0x00, LOG_BUFFER_LEGNTH);
  sprintf(LogBuffer, "Reading %d pages |", uiBlocks + 1);
  MyLog(LogBuffer, 3);

  for (page = 4; page <= uiBlocks; page += 4) {
    // Try to read out the data block
    if (nfc_initiator_mifare_cmd(rt.device, MC_READ, page, &mp)) {
      memcpy(tag->amb[page / 4].mbd.abtData, mp.mpd.abtData, 16);
    } else {
      bFailure = true;
      MyLog("Tag read error", 2);
      break;
    }
    
  }
  

  return (!bFailure);
}


/**
 * Scans for tag. Scan rate/ duration is based on szModulations, uiPollNr, uiPeriod  Located in R_NFC.h
 * @param rt
 * 
 * @return 
 */
bool ScanForTag(ReaderTag *rt){
    //clean nt. thats is what will hold the data for the tag.. I think
    memset(&rt->nt, 0x00, sizeof(rt->nt));
    
    //Let someone know what were doing.
    sprintf(LogBuffer, "NFC device will poll during %ld ms (%u pollings of %lu ms for %" PRIdPTR " modulations)\n", (unsigned long) uiPollNr * szModulations * uiPeriod * 150, uiPollNr, (unsigned long) uiPeriod * 150, szModulations);
    MyLog(LogBuffer,3);
    
    //Start the scan
    if ((res = nfc_initiator_poll_target(rt->device, nmModulations, szModulations, uiPollNr, uiPeriod, &rt->nt))  < 0) {
        //somthing went wrong, ABORT, ABORT!!!!
        nfc_perror(rt->device, "nfc_initiator_poll_target");
        nfc_close(rt->device);
        nfc_exit(rt->context);
        MyLog("initiator poll failed, Exiting....",0);
        exit(EXIT_FAILURE);
    }

    if (res > 0) {
        //we have tagedge aka theres a tag present
        return(true);
    } else {
        //nada aka no tag found. Yes, I am sure that I spelled that wrong. WHat are you, the spelling police?
        MyLog("no tags found.", 3);
      return(false);
    }
}

/*
 * Starts the tag reader 
 * @param rt
 * @return 
 */
bool TagInit(ReaderTag *rt){
    
  

    // Try to open the NFC device
    rt->device = nfc_open(rt->context, NULL);
    if (rt->device == NULL) {
      //failure let the log know ant tell the caller
        ERR("Error opening NFC device");
        MyLog("Failed to open device",0);
        return(false);
    }

    //Start the reader
    if (nfc_initiator_init(rt->device) < 0) {
        //failure let the log know ant tell the caller
        nfc_perror(rt->device, "nfc_initiator_init");
        MyLog("Failed to init device", 0);
        return(false);
    }

    //We have a good reader
    return(true);
}

/**
 *
 *
 *
 * The end.
 *
 *
 */