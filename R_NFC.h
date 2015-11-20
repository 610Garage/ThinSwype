/* 
 * File:   R_NFC.h
 * Author: 610bob
 *
 * Created on November 12, 2015, 9:11 PM
 */

    
#ifndef R_NFC_H
#define	R_NFC_H

    #ifdef __cplusplus
    extern "C" {
    #endif

#include <nfc/nfc.h>
#include "utils/nfc-utils.h"
#include "utils/mifare.h"

#include "UserInfo.h"
#include "Config.h"

        #define MAX_MDEF_MSG_LENGTH 1024
        #define PAYLOAD_LEGNTH 4
        #define DEFUALT_HEADER_LEGNTH 6
        #define TLV_L 1

        static const char DefualtHeader[] = { 0x03, 0, 0xD1,0x01, 0, 'T'};


        typedef struct {
          nfc_device *device;
          nfc_target nt;
          nfc_context *context;
        } ReaderTag;

        #define uiPollNr 20
        #define uiPeriod 2
        #define szModulations 2


        bool NDDEF_DEGenerate(Credentials * Payload, mifareul_tag * tag, Config * cf);
        bool TagInit(ReaderTag *rt);
        bool ScanForTag(ReaderTag *rt);
        bool N_read_card(ReaderTag rt,mifareul_tag * tag);

    #ifdef __cplusplus
    } // closing brace for extern "C"
    #endif

#endif	/* R_NFC_H */

