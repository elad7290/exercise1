

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* m_dio;
	vector<Command *>m_command_array;
	TimeSeries train,test;
	bool exit= false;
	HybridAnomalyDetector hybrid;
    vector<AnomalyReport> anomalies;
	// you can add data members
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
