/************************************************************************
**      RollNo:2018201033          Name : Darshan Kansagara			   **
************************************************************************/

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
#include <dirent.h>
//#include "socket.cpp"
using namespace std;

extern const char *logpath;
// extern int globalcurchunksize;

//mtorrent generator's functions
string sha1(char *chnk,int sz, int shrt);
string file_hash(char *fpath);
string create_hash(char *fpath , char *tpath,string sk1,string sk2);

//Client Commands
string run_client_share(vector<string> tkn, string clnt_skt,string trck_sk1, string trck_sk2);
string run_client_get(vector<string> tkn);
string run_client_remove(vector<string> tkn,string cln_skt);

 void *seederserverservice( void * sock_cln);
//class mysocket;
//void writelog(string str);
