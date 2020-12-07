#include<bits/stdc++.h>
using namespace std;

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