#include "clientheader.h"
#include "socket.cpp"
using namespace std;
class data
{   public:
    char * dpath, *reply, *trnt_path;
    int sk;
};
string cskt,tskt1,tskt2,torrent_pt;
map<string,string> download_status;

void readalltorrents(int sk)
{
    DIR *dir;
    struct dirent *diread;
    dir=opendir(".");
    if(dir)
    {
        while((diread = readdir(dir))!=NULL)
        {
            string fl = string(diread->d_name);
            string ex = ".mtorrent";
             string fp;
            if(fl.find(ex)!= string::npos)
            {
                ifstream fp ( fl, ifstream::binary );
                int ct=4;
                string fpath,ln;
                while(ct>=0&&getline(fp,ln))
                {   if(ct==2)
                        fpath=ln;
                    ct--;
                }
               
               char *lhash = new char[ln.length()+1];
               strcpy(lhash,ln.c_str());
               string shash = sha1(lhash,ln.length(),0);
               fp.close();
               string as = "share#"+shash+"#"+cskt;


            }
        }
    }
}
vector<string> split_str(string cmd, char c)
{	vector<string> v;
	string tp = "";
	for (int i = 0; i < cmd.length(); i++)
	{	if (i == cmd.length() - 1)
		{	tp += cmd[i];
			v.push_back(tp);
		}
		else if (cmd[i] == c)
		{	v.push_back(tp);
			tp = "";
		}
		else
			tp += cmd[i];
	}
	return v;
}
int tranfer(string data,string destination_path)
{
    stringstream check1(data);
    string mdl;
    vector<pair<string,string>> seeder_list;
    while(getline(check1,mdl,'@'))
    {   
        vector<string> d = split_str(mdl,'#');

        seeder_list.push_back({d[0],d[1]});

    }
    mysocket seeder_socket;
    seeder_socket.setdata(seeder_list[0].first);
    string fpath = seeder_list[0].second;
    int sock = 0;
    struct sockaddr_in seeder_adrs;
    if((sock=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        cout<<"error1";
        return -1;
    }
    memset(&seeder_adrs,'0',sizeof(seeder_adrs));
    seeder_adrs.sin_family = AF_INET;
    seeder_adrs.sin_port = htons(seeder_socket.port);

    if(inet_pton(AF_INET,seeder_socket.ip, &seeder_adrs.sin_addr)<=0)
    {
        cout<<"error2";
        return -1;
    }
    if(connect(sock,(struct sockaddr *)&seeder_adrs,sizeof(seeder_adrs))<0)
    {
        cout<<"error3";
        return -1;
    }
    char *dp = new char[destination_path.length()+1];
    strcpy(dp,destination_path.c_str());
    ofstream fl(dp,ofstream::binary);

    char *fp = new char[fpath.length()+1];
    strcpy(fp,fpath.c_str());
    
    send(sock,fp,strlen(fp),0);
    int n;
    download_status[destination_path]="D";
    do
    {
        char *buf = new char[512*1024];
        n=read(sock,buf,512*1024);
        fl.write(buf,n);
    }
    while(n>0);
    fl.close();
    return 1;
}
void * run_get(void *obj)
{
    data d = *(data *) obj;
    string tracker_data = string (d.reply);
    string d_path = string(d.dpath);
    int skt = d.sk;
    string mt_path = string(d.trnt_path);
    if((tranfer(tracker_data,d_path)==1))
    {
        download_status[d_path]="S";
        vector<string> c;
        c.push_back("share");
        c.push_back(d_path);
        c.push_back(torrent_pt);
        string data = run_client_share(c,cskt,tskt1,tskt2);
       if(data!="-1")
        {
            char *sd = new char[ data.length()+1];
            strcpy(sd,data.c_str());
            send(skt,sd,strlen(sd),0);
            char buf[1024];
            read(skt,buf,1024);
        }
   }
    else
    {
        cout<<"errorrunget";
    }
    return obj;
}


int main(int argc,char *argv[])
{	
    mysocket clntsock;
   mysocket trackersk1;
   mysocket trackersk2;
    if(argc !=4)
    {
        cout<<"Invalid Input";
        return 0;
    }
    cskt = string(argv[1]);
    tskt1 = string(argv[2]);
    tskt2 = string(argv[3]);

    clntsock.setdata(cskt);
    trackersk1.setdata(tskt1);
    trackersk2.setdata(tskt2);

    pthread_t c_seeder;
    //cout<<"socket data aa gya bc"<<endl;
   // return 0;
    if(pthread_create(&c_seeder,NULL,seederserverservice, (void *) &cskt ))
    {
        cout<<"errorpt1";
    }

    int sock =0;
    struct sockaddr_in saddrs;
    if((sock=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        cout<<"errorpt2";
        return -1;
    }

    memset(&saddrs,'0',sizeof(saddrs));
    saddrs.sin_family = AF_INET;
    saddrs.sin_port = htons(trackersk1.port);

    if(inet_pton(AF_INET,trackersk1.ip, &saddrs.sin_addr)<=0)
    {
        cout<<"ERROR";
        return -1;
    }

    if(connect(sock,(struct sockaddr*)&saddrs,sizeof(saddrs))<0)
    {
        return -1;
    }

    readalltorrents(sock);
    cout<<"connected to server"<<endl;
    while(1)
    {
        string cmd;
        char *mtpath;
        char *destpath;

        getline(cin>>ws,cmd);
        bool get=false,cl=false;
        vector<string> cmds = split_str(cmd,' ');
        string fun = cmds[0];
        string final_instruction;
        //cout<<"hello";
        //return 0;
        if(fun=="share")
        {      
            if(cmds.size()!=3)
            {
                cout<<"errorshare";
            }
           // cout<<"idhar aa gya"<<endl;
            final_instruction = run_client_share(cmds,cskt,tskt1,tskt2);
            if(final_instruction=="-1")
                continue;

        }
        else if(fun=="get")
        {
            if(cmds.size()!=3)
            {
                cout<<"errorgeett";
            }
            final_instruction = run_client_get(cmds);
            destpath = new char[cmds[2].length()+1];
            strcpy(destpath,cmds[2].c_str());
            torrent_pt=cmds[1];
             mtpath = new char[cmds[1].length()+1];
            strcpy(mtpath,cmds[1].c_str());
            if(final_instruction=="-1")
                continue;
            else
                get = true;    
        }
        else if(fun=="remove")
        {
            if(cmd.size()!=2)
            {
                cout<<"error";
            }
            final_instruction = run_client_remove(cmds,cskt);
            mtpath = new char[cmds[1].length()+1];
            strcpy(mtpath,cmds[1].c_str());
            if(final_instruction=="-1")
                continue;
        }
        else if(fun=="show_downloads")
        {
            if(download_status.size()==0)
                cout<<"NO DOWNLOADS"<<endl;
            else
            {
                for(auto i:download_status)
                    cout<<i.second<<" "<<i.first<<endl;
            }
            continue;    
        }
        else if(fun=="close")
        {
            final_instruction = "close"+cskt;
            cl = true;
        }
        else
        {
            cout<<"error"<<endl;
            continue;
        }
        char *req_to_tracker = new char[final_instruction.length()+1];
        strcpy(req_to_tracker,final_instruction.c_str());
        cout<<"final instruction is "<<final_instruction<<endl;
        send(sock,req_to_tracker,strlen(req_to_tracker),0);
        //cout<<"daTA SENT"<<endl;
        char tracker_reply[1024]={0};
        read(sock,tracker_reply,1024);
        //cout<<"data recieved"<<endl;
        string tr = string(tracker_reply);
        //cout<<tr<<endl;
        if(get)
        {
        	//cout<<tr<<endl;
            data inp;
            inp.sk = sock;
            inp.dpath = new char[cmds[2].length()+1];
            strcpy(inp.dpath,cmds[2].c_str());
            inp.reply = new char[tr.length()+1];
            strcpy(inp.reply,tr.c_str());
            inp.trnt_path = new char[cmds[1].length()+1];
            strcpy(inp.trnt_path,cmds[1].c_str());

            pthread_t threadid;
            if(pthread_create(&threadid,NULL, run_get,(void *)&inp)<0)
            {
                cout<<"ERROR";
            }
            get=false;
        }
        if(tracker_reply=="FILE REMOVED")
        {
            if(remove(mtpath)!=0)
                cout<<"torrent not delete"<<endl;
        }   
        if(cl)
        {   cout<<"Thankyou"<<endl;
            close(sock);
            break;
        }
    }
    return 0;
}