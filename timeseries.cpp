#include "timeseries.h"

using namespace std;

TimeSeries::TimeSeries(const char *CSVfileName)
{
	ifstream file;
	string line;
	file.open(CSVfileName);
	file >> line;
	vector<string> keys(0);
	//first we see how mach features we have.
	while (!line.empty())
	{
		string token = line.substr(0, line.find(","));
		line.replace(line.find(token), token.length() + 1, "");
		keys.push_back(token);
	}
	//Initialization of a number of vectors corresponding to the number of keys
	for (int i = 0; i < keys.size(); i++)
	{
		m_features.insert(make_pair(keys[i], vector<float>(0)));
	}
	//Assigning values to their respective keys
	//and conversion from string to float.
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

TimeSeries::TimeSeries(map<string, vector<float>> features) {
    m_features=features;
}
map<string, vector<float>> TimeSeries::getMap() const
{
	return m_features;
}

int TimeSeries::getLength(string s) const
{
	if (m_features.count(s) == 1)
	{
		return m_features.at(s).size();
	}
	return 0;
}
vector<float> TimeSeries::getValSamples(string s) const
{
	if (m_features.count(s) == 1)
	{
		return m_features.at(s);
	}
	exit(1);
}
vector<string> TimeSeries::getFeatures() const{
	vector<string> s (0);
	for(auto element : m_features){
		s.push_back(element.first);
	}
	return s;
}