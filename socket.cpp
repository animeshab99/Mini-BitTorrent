#include<bits/stdc++.h>
using namespace std;

class mysocket
{
public:
	string ip;
	int port;
	mysocket()
	{	ip = "";
		port = 0;
	}
	void setdata(string s)
	{
		stringstream str(s);
		vector<string> tkn;
		string tp;
		while (getline(str, tp, ':'))
			tkn.push_back(tp);

		ip = tkn[0];
		port = stoi(tkn[1]);
	}
};