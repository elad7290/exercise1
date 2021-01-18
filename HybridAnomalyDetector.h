

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    void learnNormal(const TimeSeries &ts)override;
    vector<AnomalyReport> detect(const TimeSeries &ts)override;
    vector<pair<pair<string, string>, float>> coralFeatuers(const TimeSeries &ts)override;


};


#endif /* HYBRIDANOMALYDETECTOR_H_ */
