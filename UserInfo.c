#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "UserInfo.h"
#include "Log.h"


/**
 * Checks to make sure we don't have currupted data. It also makes sure the data dosent have any " marks. This is to prevent shell injection
 * @param crd
 * @return 
 * Whether the credentials are good or not
 */
bool Credentials_Check(Credentials *crd){
    //making sure that ervything is as long as we expect. Also inspecting for " marks
    if(crd->PLegnth == strlen(crd->password) && crd->ULegnth == strlen(crd->user) && crd->VLegnth && strlen(crd->VM) && memchr(crd, '"',sizeof(*crd)) == NULL){
        MyLog("Credentials check is good",3);// tell the log
        return(true);// tell the caller that the credentials are good for use
    }
    MyLog("Credentials check failed",3);//tell the log
    return(false);//Credentails bad. tell caller.
}

/**
 * Checks the credentals legnth and then set the credentials into the crd struct
 * Used for righting the tag
 * @param u
 * user name
 * @param p
 * password
 * @param vm
 * vm adress
 * @param crd
 * pointer to the credentials struct
 */
void SetUserInfo(char * u, char * p, char * vm, Credentials *crd){
    //make sure that the crd struct is clean
    memset(crd, 0x00, sizeof(*crd));
    
    int UL = strlen(u);//how long is the user name
    if(UL > Max_User_Legnth){//is the user name length short enough?
        printf("\nUser is too long\n");// tell the user that there an idoit :)
        return;
    }
    int PL = strlen(p);//how long is the passowrd?
    if(PL > Max_Password_Legnth){//is it too long?
        printf("\nPassword is too long\n");//tell the user that there stupider than I thought :)
        return;
    }
    int VL = strlen(vm);//how long is the vm adress
    if(VL > Max_VM_Legnth){//is it too long?
        printf("\nVM is too long\n");//Tell the user to go get an mri, they may be missing a brain :)
        return;
    }
    //copy the data over to the srtuct
    memcpy(&(crd->user[0]),u,UL);//user name
    crd->ULegnth = UL;//user name legnth
    memcpy(&crd->password,p,PL);//password
    crd->PLegnth = PL;//password legnth
    memcpy(&crd->VM,vm,VL);//vm adress
    crd->ULegnth = VL;//vm adress legnth
}
