/* 
 * File:   UserInfo.h
 * Author: 610bob
 *
 * Created on November 12, 2015, 1:23 AM
 */

#ifndef USERINFO_H
#define	USERINFO_H

#define Max_User_Legnth 12
#define Max_Password_Legnth 30
#define Max_VM_Legnth 24

typedef struct{
    char user[Max_User_Legnth];
    char ULegnth;
    char password[Max_Password_Legnth];
    char PLegnth;
    char VM[Max_VM_Legnth];
    char VLegnth;
} Credentials;

bool Credentials_Check(Credentials *crd);
void SetUserInfo(char * u, char * p, char * vm, Credentials *crd);

#endif	/* USERINFO_H */

