/*#include<bits/stdc++.h>
#include "socket.cpp"
#include<bits/stdc++.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <pthread.h>
#include <dirent.h>*/
#include "clientheader.h"
#include "socket.cpp"
using namespace std;
void *serverservice(void * sock_dec)
{
    int skt = *(int *) sock_dec;
    char buffer[1024]={0};
    read(skt,buffer,1024);
    string fl_pth = string(buffer);

    char *fp = new char[fl_pth.length()+1];
    strcpy(fp,fl_pth.c_str());

    ifstream fl(fp,ifstream::binary);

    if(!fl)
    {
        cout<<"file error";
    }

    struct stat f_st;
    stat(fp, &f_st);
    long long t_sz = f_st.st_size;
    long long c_sz= 512*1024;

    int tot_chnks = t_sz/c_sz;
    int ls_chnk = t_sz%c_sz;
    if(ls_chnk!=0)
        tot_chnks++;
    else    
        ls_chnk = c_sz;

   for(int i=0;i<tot_chnks;i++)
   {
        int cr_sz=c_sz;
        if(i==tot_chnks-1)
            cr_sz= ls_chnk;

        char * data = new char[cr_sz];
        fl.read(data,cr_sz);
        send(skt,data,cr_sz,0);
   }
   close(skt);
   fl.close();
   return sock_dec;     
}
 void *seederserverservice( void * sock_cln)
 {
     string sd_skt = *(string *)sock_cln;
     pthread_t threadid;
     mysocket clnt_seeder_skt;
     clnt_seeder_skt.setdata(sd_skt);
     
     int serverfd,opt=1,new_skt;
     struct sockaddr_in adrs;
        int adr_len = sizeof(adrs);

     if((serverfd=socket(AF_INET,SOCK_STREAM,0))==0)
     {
        cout<<"errorsd1";
        exit(0);
     }

    if(setsockopt(serverfd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &(opt), sizeof(opt)))
    {
        cout<<"errorsd2";
        exit(0);
    }

    adrs.sin_family = AF_INET;
    adrs.sin_addr.s_addr = inet_addr(clnt_seeder_skt.ip);
    adrs.sin_port = htons(clnt_seeder_skt.port);
     
    if(bind(serverfd,(struct sockaddr *) &adrs ,sizeof(adrs))<0)
    {
        cout<<"errorsd3";
        exit(0);
    }

    if(listen(serverfd,10)<0)
    {
        cout<<"errorsd4";
        exit(0);
    }

    while(1)
    {
        if((new_skt = accept(serverfd,(struct sockaddr *)&adrs,(socklen_t *)&adr_len))<0)
        {
            cout<<"errorsd5";
            exit(0);
        }
        if(pthread_create(&threadid,NULL,serverservice,(void *)&new_skt)<0)
        {
            cout<<"errorsd6";
            exit(0);
        }
        if(new_skt<0)
        {
            cout<<"errorsd7";
        }
    }
   return sock_cln; 
 }
