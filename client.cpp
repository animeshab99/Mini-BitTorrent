#include<bits/stdc++.h>
#include "socket.cpp";
using namespace std;
struct datatype
{   
    char * dpath, *reply, *trnt_path;
    int sk;
}
string cskt,tskt1,tskt2;
map<string,string> download_status;

int main(int argc,char *argv[])
{
    mysocket clntsock;
    mysocket trackersk1;
    mysocket trackersk2;
    if(argc !=5)
    {
        cout<<"bhup bsdk";
        return 0;
    }
    cskt = string(argv[1]);
    tskt1 = string(argv[2]);
    tskt2 = string(argv[3]);

    clnsock.setdata(cskt);
    trackersk1.setdata(tskt1);
    trackersk2.setdata(tskt2);

    pthread_id c_seeder;
    if(pthread_create(&c_seeder,NULL,seederserverservice, (void *) cskt ))
    {
        cout<<"error";
    }

    int sock =0;

}