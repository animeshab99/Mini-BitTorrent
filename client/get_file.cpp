#include "clientheader.h"
using namespace std;

string run_client_get(vector<string> tkn)
{
    string mtpath = tkn[1];
    char *tp = new char[mtpath.length()+1];
    strcpy(tp, mtpath.c_str());
    ifstream fp(tp,ifstream::binary);
    string ln;
    int ct=4;
    while ((ct >=0) && getline(fp, ln))
    {
        ct--;
    }
    fp.close();
     char *hs = new char[ln.length()+1];
    strcpy(hs, ln.c_str());
    string sht_hs = sha1(hs,strlen(hs),0);
    //cout<<sht_hs<<endl;
    string as = tkn[0]+"#"+sht_hs;
    //cout<<as<<endl;
    return as;
}