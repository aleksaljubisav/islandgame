#include <curl/curl.h>
#include <iostream>
#include "backend.h"
#include "gVariablesConstants.h"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* out)
{
	out->append((char*)contents, size * nmemb);
	return size * nmemb;
}

void fetchMapData(const string& url, string& output)
{
	CURL* curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		}
		curl_easy_cleanup(curl);
	}
}

void parseMapData(const string& data, vector<vector<int>>& grid)
{
	size_t pos = 0, rowStart = 0;
	while (rowStart < data.size() && grid.size() < MATRIX_SIZE)
	{
		size_t rowEnd = data.find("\n", rowStart);
		if (rowEnd == std::string::npos)
		{
			rowEnd = data.size();
		}
		std::string row = data.substr(rowStart, rowEnd - rowStart);

		vector<int> rowValues;
		size_t valueStart = 0, valueEnd;
		while ((valueEnd = row.find(" ", valueStart)) != string::npos)
		{
			rowValues.push_back(stoi(row.substr(valueStart, valueEnd - valueStart)));
			valueStart = valueEnd + 1;
		}
		rowValues.push_back(stoi(row.substr(valueStart)));

		grid.push_back(rowValues);

		rowStart = rowEnd + 1;
	}
}