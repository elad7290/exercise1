
#include "SimpleAnomalyDetector.h"
#include "anomaly_detection_util.h"
#define THRESHOLD 0.9

//declaration of functions
float maxOffset(vector<Point *> p, Line line);
vector<Point *> createsAPoins(vector<float> x, vector<float> y);
vector<pair<pair<string, string>, float>> coralFeatuers(const TimeSeries &ts);


SimpleAnomalyDetector::SimpleAnomalyDetector()
{
	cf = vector<correlatedFeatures>(0);
}

SimpleAnomalyDetector::~SimpleAnomalyDetector()
{
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts)
{
	vector<pair<pair<string, string>, float>> coraltivPair = coralFeatuers(ts);
	for (pair<pair<string, string>, float> pair : coraltivPair)
	{
		//find features
		string feature1 = pair.first.first;
		string feature2 = pair.first.second;
		//find corrallation
		float corrallation = pair.second;
		//find lin_reg
		vector<Point *> pointsArray = createsAPoins(ts.getValSamples(feature1), ts.getValSamples(feature2));
		//Line lin_reg = linear_reg(&pointsArray[0], pointsArray.size());
		Line lin_reg = linear_reg(&pointsArray[0],pointsArray.size());
		//find threshold
		float threshold = maxOffset(pointsArray, lin_reg);

		//set struct
		correlatedFeatures correlatedFeatures;
		correlatedFeatures.feature1 = feature1;
		correlatedFeatures.feature2 = feature2;
		correlatedFeatures.corrlation = corrallation;
		correlatedFeatures.lin_reg = lin_reg;
		correlatedFeatures.threshold = threshold;

		//insret to cf vector
		cf.push_back(correlatedFeatures);

		//free memory
		for (Point *p : pointsArray)
		{
			delete (p);
		}
	}
}
/*
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts)
{
	//
}
*/
//מחזירה לי זוגות של פיצרים והמאפיינים  הקורלטיבים שלהן
vector<pair<pair<string, string>, float>> coralFeatuers(const TimeSeries &ts)
{
	vector<string> names = ts.getFeatures();
	float max, temp;
	pair<string, string> corlPairsName;
	vector<pair<pair<string, string>, float>> c;
	for (int i = 0; i < names.size(); i++)
	{
		max = 0;
		for (int j = i + 1; j < names.size(); j++)
		{
			temp = pearson(&ts.getValSamples(names[i])[0], &ts.getValSamples(names[j])[0], ts.getLength(names[i]));
			if (max < temp)
			{
				max = temp;
				corlPairsName = make_pair(names[i], names[j]);
			}
		}
		if (max >= THRESHOLD)
		{
			c.push_back(make_pair(corlPairsName, max));
		}
	}
	return c;
}

//יוצרת לי מערך של נקודות מ 2 וקטורים שונים
vector<Point *> createsAPoins(vector<float> x, vector<float> y)
{
	vector<Point *> points(0);
	for (int i = 0; i < x.size(); i++)
	{
		points.push_back(new Point(x[i], y[i]));
	}
	return points;
}

//פונקציה זאת מחזירה לי היסט מקסימאלי
float maxOffset(vector<Point *> p, Line line)
{
	float max = 0;
	for (Point *point : p)
	{
		float temp = dev(*point, line);
		if (max < temp)
		{
			max = temp;
		}
	}
	return max;
}