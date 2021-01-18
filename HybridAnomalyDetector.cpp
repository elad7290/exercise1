
#include "HybridAnomalyDetector.h"

#define PRECISION 1.1
#define MIN_THRESHOLD 0.5

HybridAnomalyDetector::HybridAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

void HybridAnomalyDetector::learnNormal(const TimeSeries &ts) {
    vector<pair<pair<string, string>, float>> coraltivPair = coralFeatuers(ts);
    for (pair<pair<string, string>, float> pair : coraltivPair) {
        //find features
        string feature1 = pair.first.first;
        string feature2 = pair.first.second;
        //set struct
        correlatedFeatures correlatedFeatures;
        correlatedFeatures.feature1 = feature1;
        correlatedFeatures.feature2 = feature2;

        //find corrallation
        float corrallation = pair.second;
        correlatedFeatures.corrlation = corrallation;


        //find lin_reg
        vector<Point *> pointsArray = createsAPoins(ts.getValSamples(feature1), ts.getValSamples(feature2));
        if (isSimple(corrallation)) {
            //Line lin_reg = linear_reg(&pointsArray[0], pointsArray.size());
            Line lin_reg = linear_reg(&pointsArray[0], pointsArray.size());
            //find threshold
            float threshold = maxOffset(pointsArray, lin_reg);
            correlatedFeatures.lin_reg = lin_reg;
            correlatedFeatures.threshold = PRECISION * threshold;
        } else {
            Circle min_circle = findMinCircle(&pointsArray[0], pointsArray.size());
            correlatedFeatures.center_reg = min_circle.center;
            correlatedFeatures.threshold = PRECISION*min_circle.radius;
        }

        //insret to cf vector
        cf.push_back(correlatedFeatures);

        //free memory of all the points
        freeMemory(pointsArray);
    }
}

/**
 * **********************************************************
 * This function check for each new two-dimensional point   *
 * obtained from the flight we will check                   *
 * its distance from the regression line we learned for the *
 * properties that created it.                              *
 * **********************************************************
 * */
vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> anomalyDetector;
    int length = cf.size();
    for (int i = 0; i < length; ++i) {
        vector<float> sampleOne = ts.getValSamples(cf[i].feature1);
        vector<float> sampleTwo = ts.getValSamples(cf[i].feature2);
        vector<Point *> pointOfSampleOneAndTwo = createsAPoins(sampleOne, sampleTwo);
        int lengthOfVector = pointOfSampleOneAndTwo.size();
        for (int j = 0; j < lengthOfVector; ++j) {
            if(isSimple(cf[i].corrlation)){
                if (cf[i].threshold < dev(*pointOfSampleOneAndTwo[j], cf[i].lin_reg)) {
                    string s = cf[i].feature1 + "-" + cf[i].feature2;
                    long timeStep = j + 1;
                    anomalyDetector.push_back(AnomalyReport(s, timeStep));
                }
            } else{
                if(distance(cf[i].center_reg,*pointOfSampleOneAndTwo[j])>cf[i].threshold){
                    string s = cf[i].feature1 + "-" + cf[i].feature2;
                    long timeStep = j + 1;
                    anomalyDetector.push_back(AnomalyReport(s, timeStep));
                }
            }

        }
        freeMemory(pointOfSampleOneAndTwo);
    }
    return anomalyDetector;
}

/**
 * **************************************************************************
 * This function returns pairs of features and their correlative properties.*
 * **************************************************************************
 **/
vector<pair<pair<string, string>, float>> HybridAnomalyDetector::coralFeatuers(const TimeSeries &ts) {
    vector<string> names = ts.getFeatures();
    float max, temp;
    pair<string, string> corlPairsName;
    vector<pair<pair<string, string>, float>> c;
    for (int i = 0; i < names.size(); i++) {
        max = 0;
        for (int j = i + 1; j < names.size(); j++) {
            temp = pearson(&ts.getValSamples(names[i])[0], &ts.getValSamples(names[j])[0], ts.getLength(names[i]));
            if (max < temp) {
                max = temp;
                corlPairsName = make_pair(names[i], names[j]);
            }
        }
        if (max > MIN_THRESHOLD && max < m_threshold) {
            c.push_back(make_pair(corlPairsName, max));
        }
        if (max >= m_threshold) {
            c.push_back(make_pair(corlPairsName, max));

        }
    }
    return c;
}
