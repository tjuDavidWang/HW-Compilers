#pragma once
#include<exception>
#include<iostream>
#include<string>
using namespace std;
class PL0Exception :public exception
{
public:
	PL0Exception(string msg) :mMsg(msg) {}
	PL0Exception(string msg,int line,int col) :mMsg(msg),line(line),col(col) {}
	virtual ~PL0Exception()noexcept {}
	virtual const char* what() const noexcept override
	{
		return mMsg.c_str();
	}
	string msg() {
		string linestr = " at line " + to_string(line);
		string colstr = " at col " + to_string(col);
		string output = mMsg + linestr + colstr;
		return output;
	}
private:
	string mMsg;
	int line=0;
	int col=0;
};