

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures {
    string feature1, feature2;  // names of the correlated features
    float corrlation;
    Point center_reg;
    Line lin_reg;
    float threshold;
};


class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {
protected:
    vector<correlatedFeatures> cf;
    float m_threshold;
public:
    SimpleAnomalyDetector();

    virtual ~SimpleAnomalyDetector();

    virtual void learnNormal(const TimeSeries &ts);

    virtual vector<AnomalyReport> detect(const TimeSeries &ts);

    vector<correlatedFeatures> getNormalModel() {
        return cf;
    }
    virtual vector<pair<pair<string, string>, float>> coralFeatuers(const TimeSeries &ts);
    bool isSimple(float corrlation);
    float maxOffset(vector<Point *> p, Line line);
    void freeMemory(vector<Point *> vector);
    vector<Point *> createsAPoins(vector<float> x, vector<float> y);
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
