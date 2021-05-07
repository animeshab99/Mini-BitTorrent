#include "socket.cpp"
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <pthread.h>


using namespace std;

char *seederfp;

class trackerdata
{
public:
	string shash, csocket, cfpath;
	trackerdata()
	{	shash = "";
		csocket = "";
		cfpath = "";
	}
	trackerdata(string hash, string ip_port, string path)
	{	shash = hash;
		csocket = ip_port;
		cfpath = path;
	}
};
map<string, vector<trackerdata>> trackertable;
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

int read_seederlist (char *fpath)
{	ifstream fp( fpath , ifstream:: binary);
	if (!fp)
	{	cout << "Seeder list not open"<<endl;
		return 0;
	}
	else
	{	string line;
		while (getline(fp, line))
		{	string d = line;
			vector<string> v = split_str(line, ' ');
			trackerdata t1(v[0], v[1], v[2]);
			trackertable[t1.shash].push_back(t1);
		}
		fp.close();
		return 1;
	}
}
int write_seederlist(char *fpath, string d)
{	ofstream fp(fpath , std::ios_base::app | std::ios_base::out);
	fp << d << endl;
	fp.close();
	return 1;
}

void update_seederlist(char *fpath)
{	ofstream fp(fpath , std::ios_base::out);
	for (auto it : trackertable)
	{
		vector<trackerdata> v = it.second;
		for (int i = 0; i < v.size(); i++)
		{
			string as = v[i].shash + " " + v[i].csocket + " " + v[i].cfpath;
			fp << as << endl;
		}
	}
	fp.close();
}
string run_get(vector<string> tkn)
{
	string as = "";
	string hash = tkn[1];
	if (trackertable.count(hash))
	{
		vector<trackerdata> v = trackertable[hash];
		int n = v.size();
		for (int i = 0; i < n - 1; i++)
			as += v[i].csocket + "#" + v[i].cfpath + "@";
		as += v[n - 1].csocket + "#" + v[n - 1].cfpath;
	}
	else
		as = "NO SEEDER AVAILABLE FOR GIVEN FILE";

	return as;
}

string run_remove(vector<string> tkn, char* fpath)
{
	string hash = tkn[1];
	string skt = tkn[2];
	bool fd_tkn = false, sz_1 = false;
	if (trackertable.count(hash))
	{
		vector<trackerdata> v = trackertable[hash];
		auto it=v.begin();
		for (it = v.begin() ; it != v.end(); it++)
		{	if ((*it).csocket == skt)
			{	fd_tkn = true;
				if (v.size() == 1)
					sz_1 = true;
				break;
			}
		}
		trackertable.erase(hash);
		if (!sz_1)
		{	v.erase(it);
			trackertable[hash] = v;
		}
	}
	if (fd_tkn)
	{	update_seederlist(fpath);
		return "FILE REMOVED";
	}
	else
		return "NO FILE FIND TO REMOVE";

}

string run_share(vector<string> tkn, char* fpath)
{
	string tkn_list = tkn[1] + " " + tkn[2] + " " + tkn[3];
	//cout<<tkn_list<<endl;
	trackerdata td(tkn[1], tkn[2], tkn[3]);
	string as;
	if (!trackertable.count(td.shash))
	{
		trackertable[td.shash].push_back(td);
		int nul = write_seederlist(fpath, tkn_list);
		as = "FILE SHARED";
	}
	else
	{
		vector<trackerdata> v = trackertable[td.shash];
		bool ch = false;
		for (int i = 0; i < v.size(); i++)
		{	if (v[i].csocket == td.csocket)
			{
				ch = true;
				as = "FILE ALREADY SHARED";
				break;
			}
		}
		if (!ch)
		{	as = "FILE SHARED";
			trackertable[td.shash].push_back(td);
			int nul = write_seederlist(fpath, tkn_list);
		}
	}
	return as;
}

string run_close(vector<string> tkn, char *fpath)
{
	string client_skt = tkn[1];
	for (auto it : trackertable)
	{
		vector<trackerdata> v = it.second;
		bool sz_1 = false, skt_mila = false;
		auto i2=v.begin();
		for ( i2 = v.begin(); i2 != v.end(); i2++)
		{
			if ((*i2).csocket == client_skt)
			{
				skt_mila = true;
				if (v.size() == 1)
					sz_1 = true;
				break;
			}
		}
		if (!skt_mila)
			continue;
		trackertable.erase(it.first);
		if (!sz_1)
		{	v.erase(i2);
			trackertable[it.first] = v;
		}
	}
	update_seederlist(fpath);
	return "GOOD BYE";
}

void * server_service(void *(socket_ptr))
{
	int socket = *(int *)socket_ptr;
	
	while (1)
	{	bool band_kar = false;
		char buffer[1024]={0};
		int data = read(socket, buffer, 1024);
		if (!data)
		{
			close(socket);
			return socket_ptr;
		}
		string s = string(buffer);
		//cout<<"recieved string is "<<s<<endl;
		string output = "";
		vector<string> tkn = split_str(s, '#');

		if (tkn[0] == "share")
		{
			output = run_share(tkn, seederfp);
		}
		else if (tkn[0] == "remove")
		{
			output = run_remove(tkn, seederfp);
		}
		else if (tkn[0] == "close")
		{
			output = run_close(tkn, seederfp);
			band_kar = true;
		}
		else if (tkn[0] == "get")
		{
			output = run_get(tkn);
		}
		else
		{
			output = "bhag bsdk";
		}
		//output += '\0';
		char *o=new char[output.length()+1];
		strcpy(o,output.c_str());
		send(socket, o, strlen(o), 0);
		if (band_kar)
		{
			close(socket);
			break;
		}
	}
	return socket_ptr;
}
int main(int argc, char *argv[])
{	mysocket trackerskt1;
	{	mysocket trackerskt2;
		pthread_t mythread;
		if (argc != 4)
		{
			cout << "TRACKER INFO IS WRONG";
			return 0;
		}
		else
		{
			read_seederlist(argv[3]);
			trackerskt1.setdata(string(argv[1]));
			trackerskt2.setdata(string(argv[2]));
			seederfp = argv[3];
			int server_fp, new_socket, opt = 1;
			struct sockaddr_in adr;
			int adrlen = sizeof(adr);
			if ((server_fp = socket(AF_INET, SOCK_STREAM, 0)) == 0)
			{	cout << "SOCKET NOT CREATED";
				exit(0);
			}
			if (setsockopt(server_fp, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
			{	cout << "SOCKET NOT OPEN";
				exit(0);
			}
			adr.sin_family = AF_INET;
			adr.sin_addr.s_addr = inet_addr(trackerskt1.ip);
			adr.sin_port = htons(trackerskt1.port);

			if (bind(server_fp, (struct sockaddr *) &adr, adrlen) < 0)
			{
				cout << "BIND NOT WORK";
				exit(0);
			}

			if (listen(server_fp, 10) < 0)
			{
				cout << "LISTEN FAILED";
				exit(0);
			}
			cout<<"Tracker is active "<<endl;
			while (1)
			{	//cout << "server open";
				if ((new_socket = accept(server_fp, (struct sockaddr *) &adr,(socklen_t*) &adrlen)) < 0)
				{	cout << "CLIENT NOT ACCDEPTED";
					exit(0);
				}
				//cout << "server open";
				if (pthread_create(&mythread, NULL, server_service, (void *)&new_socket) < 0)
				{	cout << "THREAD NOT CREATED";
					exit(0);
				}
			}

		}
		return 0;
	}
}
