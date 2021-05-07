//#include<bits/stdc++.h>
#include "clientheader.h"
using namespace std;

class mysocket
{
public:
	char *ip;
	int port;
	mysocket()
	{	//ip;
		port = 0;
	}
	void setdata(string s)
	{
		stringstream str(s);
		vector<string> tkn;
		string tp;
		while (getline(str, tp, ':'))
			tkn.push_back(tp);
		ip=new char[tkn[0].length()+1];
		strcpy(ip,tkn[0].c_str());
		port = stoi(tkn[1]);
	}
};