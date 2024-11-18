#ifndef _BACKEND_H
#define _BACKEND_H

#include <vector>
#include <string>
using namespace std;

extern size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* out);

extern void fetchMapData(const string& url, string& output);

extern void parseMapData(const string& data, vector<vector<int>>& grid);

#endif