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
    TimeSeries(){};
	TimeSeries(const char *CSVfileName);
	TimeSeries(map<string, vector<float>> features);
	map<string, vector<float>> getMap() const;
	int getLength(string s) const;
	vector<float> getValSamples(string s) const;
	vector<string> getFeatures() const;
};

#endif /* TIMESERIES_H_ */
