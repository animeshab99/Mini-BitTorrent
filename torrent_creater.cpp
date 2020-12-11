#include<bits/stdc++.h>
using namespace std;
#define c_size 512*1024
string file_hash(char *fpath)
{
    string f_hash = "";
    ifstream fp(fpath,ifstream :: binary);
    if(!fp)
    {
        cout<<"are oo bsdiwale chacha"<<endl;
        return "-1";
    }
    struct stat s;
    stat(fpath,s);
    long long int t_size = s.st_size;
    long long int cs = c_size;
    int no_chunks = t_size/cs;
    int lcs = t_size%cs; 
    if(lcs!=0)
        no_chunks++;
    else
        lcs = cs;    
    for(int i=0;i<no_chunks;i++)
    {
        int cr_sz;
        if(i==no_chunks-1)
            cr_sz = lcs;
        else
            cr_sz = cs;
        char *buf = new char[cr_sz];
        fp.read(buf,cr_sz);
        string tp_hs = sha1(buf,cr_sz,1);
        f_hash+=tp_hs;
    }
    return f_hash;

}

string create_hash(char *fpath , char *tpath,string sk1,string sk2)
{
    struct stat s;
    if(stat(&fpath,s)==-1)
    {
        cout<<"kyu maz le ra"<<endl;
        return "-1";
    }
    ofstream fp;
    fp.open(string(fpath));
    fp<<sk1<<endl;
    fp<<sk2<<endl;
    fp<<string(fpath)<<endl;
    fp<<s.st_size<<endl;
    string hash = file_hash(fpath);
    fp<<hash<<endl;
    fp.close();
    return hash;
}