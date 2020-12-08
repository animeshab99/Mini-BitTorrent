#include<bits/stdc++.h>
#include "socket.cpp"
#include <pthread.h>

using namespace std;

string seederfp;

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
	{	cout << "maa chuda";
		return 0;
	}
	else
	{	string line;
		while (getline(fpath, line))
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
	if (trackertable.count("hash"))
	{
		vector<trackerdata> v = mp[hash];
		int n = v.size();
		for (int i = 0; i < n - 1; i++)
			as += v[i].csocket + "#" + v[i].cfpath + "@";
		as += v[n - 1].csocket + "#" + v[n - 1].cfpath;
	}
	else
		ans = "maa chuda";
	return ans;
}

string run_remove(vector<string> tkn, char* fpath)
{
	string hash = tkn[1];
	string skt = tkn[2];
	bool fd_tkn = false, sz_1 = false;
	if (trackertable.count(hash))
	{
		vector<trackerdata> v = trackertable[hash];
		auto it;//=v.begin();
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
		return "maze karo";
	}
	else
		return "maa chudao";

}

string run_share(vector<string> tkn, char* fpath)
{
	string tkn_list = tkn[1] + " " + tkn[2] + " " + tkn[3];
	trackerdata td(tkn[1], tkn[2], tkn[3]);
	string as;
	if (!trackertable.count(td.shash))
	{
		trackertable[td.shash].push_back(td);
		int nul = write_seederlist(fpath, tkn_list);
		as = "maze maro";
	}
	else
	{
		vector<trackerdata> v = trackertable[td.shash];
		bool ch = false;
		for (int i = 0; i < v.size(); i++)
		{	if (v[i].csocket == td.csocket)
			{
				ch = true;
				as = "kahe chutiya bana re";
				break;
			}
		}
		if (!ch)
		{	as = "mil gai";
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
		auto i2;
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
		trackertable.erase(it);
		if (!sz_1)
		{	v.erase(i2);
			trackertable[it] = v;
		}
	}
	update_seederlist(fpath);
	return "bhag bsdk";
}

int main(int argc, char *argv[])
{	mysocket trackerskt1;
	{	mysocket trackerskt2;
		pthread_t mythread;
		if (argc != c)
		{
			cout << "kyu chutiya bana ra h";
			return 0;
		}
		else
		{
			read_seederlist(argv[3]);
			trackerskt1.setdata(string(argv[1]));
			trackerskt2.setdata(string(argv[2]));
			seederfp = string(argv[3]);
			int server_fp, new_socket, opt = 1;
			struct sockaddr_in adr;
			int adrlen = sizeof(adr);
			if (server_fp = socket(AF_INET, SOCK_STREAM, 0) == 0)
			{	cout << "error bolte";
				exit();
			}
			if (setsockopt(server_fp, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
			{	cout << "error bolte";
				exit();
			}
			adr.sin_family = AF_INET;
			adr.sin_addr.s_addr = inet_addr(trackerskt1.ip);
			adr.sin_port = htons(trackerskt1.port);

		}
		return 0;
	}
