#include "clientheader.h"
using namespace std;

string run_client_remove(vector<string> tkn,string cln_skt)
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
    string sht_hs = sha1(hs,ln.length(),0);
    string as = tkn[0]+"#"+sht_hs+"#"+cln_skt;
    return as;
}