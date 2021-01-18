#include "SimpleAnomalyDetector.h"
#include "anomaly_detection_util.h"

//#define THRESHOLD 0.9
#define PRECISION 1.1

//declaration of functions
float maxOffset(vector<Point *> p, Line line);

vector<Point *> createsAPoins(vector<float> x, vector<float> y);

vector<pair<pair<string, string>, float>> coralFeatuers(const TimeSeries &ts);

void freeMemory(vector<Point *> vector);


SimpleAnomalyDetector::SimpleAnomalyDetector() {
    cf = vector<correlatedFeatures>(0);
    m_threshold=0.9;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    vector<pair<pair<string, string>, float>> coraltivPair = coralFeatuers(ts);
    for (pair<pair<string, string>, float> pair : coraltivPair) {
        //find features
        string feature1 = pair.first.first;
        string feature2 = pair.first.second;
        //find corrallation
        float corrallation = pair.second;
        //find lin_reg
        vector<Point *> pointsArray = createsAPoins(ts.getValSamples(feature1), ts.getValSamples(feature2));
        //Line lin_reg = linear_reg(&pointsArray[0], pointsArray.size());
        Line lin_reg = linear_reg(&pointsArray[0], pointsArray.size());
        //find threshold
        float threshold = maxOffset(pointsArray, lin_reg);

        //set struct
        correlatedFeatures correlatedFeatures;
        correlatedFeatures.feature1 = feature1;
        correlatedFeatures.feature2 = feature2;
        correlatedFeatures.corrlation = corrallation;
        correlatedFeatures.lin_reg = lin_reg;
        correlatedFeatures.threshold = PRECISION * threshold;

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
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> anomalyDetector;
    int length = cf.size();
    for (int i = 0; i < length; ++i) {
        vector<float> sampleOne = ts.getValSamples(cf[i].feature1);
        vector<float> sampleTwo = ts.getValSamples(cf[i].feature2);
        vector<Point *> pointOfSampleOneAndTwo = createsAPoins(sampleOne, sampleTwo);
        int lengthOfVector = pointOfSampleOneAndTwo.size();
        for (int j = 0; j < lengthOfVector; ++j) {
            if (cf[i].threshold < dev(*pointOfSampleOneAndTwo[j], cf[i].lin_reg)) {
                string s = cf[i].feature1 + "-" + cf[i].feature2;
                long timeStep = j + 1;
                anomalyDetector.push_back(AnomalyReport(s, timeStep));
            }
        }
        freeMemory(pointOfSampleOneAndTwo);
    }
    return anomalyDetector;
}

/**
 * ****************************
 * Release dynamic allocation.*
 * ****************************
 * */
void SimpleAnomalyDetector::freeMemory(vector<Point *> vector) {
    for (Point *p : vector) {
        delete (p);
    }
}

/**
 * **************************************************************************
 * This function returns pairs of features and their correlative properties.*
 * **************************************************************************
 **/
vector<pair<pair<string, string>, float>> SimpleAnomalyDetector::coralFeatuers(const TimeSeries &ts) {
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
        if (max >=m_threshold) {
            c.push_back(make_pair(corlPairsName, max));
        }
    }
    return c;
}

/**
 * *****************************************************
 * Creates an array of points from 2 different vectors.*
 * *****************************************************
 **/
vector<Point *> SimpleAnomalyDetector::createsAPoins(vector<float> x, vector<float> y) {
    vector<Point *> points;
    for (int i = 0; i < x.size(); i++) {
        points.push_back(new Point(x[i], y[i]));
    }
    return points;
}

/**
 * ****************************************
 * This function returns a maximum offset.*
 * ****************************************
 **/
float SimpleAnomalyDetector::maxOffset(vector<Point *> p, Line line) {
    float max = 0;
    for (Point *point : p) {
        float temp = dev(*point, line);
        if (max < temp) {
            max = temp;
        }
    }
    return max;
}
/**
 * ***************************************************************************
 * This function returns true if we need to run the simple anomaly detection.*
 * ***************************************************************************
 **/
bool SimpleAnomalyDetector::isSimple(float corrlation){
    if(abs(corrlation)>=m_threshold){
        return true;
    }
    return false;
}