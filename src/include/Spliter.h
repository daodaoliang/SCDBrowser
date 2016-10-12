#ifndef Spliter_h
#define Spliter_h

#include <string>
#include <vector>

using namespace std;

typedef string::size_type (string::*find_t)(const string& delim, 
	string::size_type offset) const;

 vector<string> Split(const string& s,
	const string& match,
	bool removeEmpty=false,
	bool fullMatch=false);
#endif