#include<bits/stc++.h>
#include "torrent_creater.cpp"
using namespace std;

string run_client_get(vector<string> tkn)
{
    string mtpath = tkn[1];
    char *tp = new char[mtpath.length()+1];
    strcpy(tp, mtpath.c_str());
    ifstream fp(tp,ifstream::binary);
    string ln;
    int ct=4;
    while ((count >=0) && getline(fp, ln))
    {
        count--;
    }
    fp.close();
     char *hs = new char[ln.length()+1];
    strcpy(hs, ln.c_str());
    string sht_hs = sha1(hs,ln.length(),0);
    string as = tkn[0]+"#"+sht_hs;
    return as;
}