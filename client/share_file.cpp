#include "clientheader.h"
string run_client_share(vector<string> tkn, string clnt_skt,string trck_sk1, string trck_sk2)
{
    string fpath = tkn[1];
    string mtpath = tkn[2];
    //cout<<"aa gya run client share"<<endl;
    char *fp, *tp;
    fp = new char[fpath.length() + 1];
    strcpy(fp, fpath.c_str());
    tp = new char[mtpath.length() + 1];
    strcpy(tp, mtpath.c_str());

    string f_hash = create_hash(fp,tp,trck_sk1,trck_sk2);
    if(f_hash=="-1")
    {
        return "-1";
    }

    char *lng_hash;
    lng_hash = new char[f_hash.length() + 1];
    strcpy(lng_hash, f_hash.c_str());
    
    string shrt_hash = sha1(lng_hash,f_hash.length(),0);
    string as= tkn[0]+"#"+shrt_hash+"#"+clnt_skt+"#"+fpath;
    //cout<<as<<endl;
    return as;
}