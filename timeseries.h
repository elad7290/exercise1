#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include "iostream"
#include "fstream"
#include "cstring"
#include "map"
#include "vector"

using namespace std;

class TimeSeries
{
private:
	map<string, vector<float>> m_features;

public:
	TimeSeries(const char *CSVfileName)
	{
		ifstream file;
		string line;
		file.open(CSVfileName);
		//int indicator=0;
		//first we see how mach features we have.
		file >> line;
		vector<string> keys(0);
		while (!line.empty())
		{
			string token = line.substr(0, line.find(","));
			line.replace(line.find(token), token.length() + 1, "");
			keys.push_back(token);
		}

		for (int i = 0; i < keys.size(); i++)
		{
			m_features.insert(make_pair(keys[i], vector<float>(0)));
		}

		while (file >> line)
		{
			for (int i = 0; i < keys.size(); i++)
			{
				string token = line.substr(0, line.find(","));
				m_features[keys[i]].push_back(stof(token));
				line.replace(line.find(token), token.length() + 1, "");
			}
		}
		file.close();
	}
};

#endif /* TIMESERIES_H_ */
