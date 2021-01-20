#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual ~DefaultIO() {}

    // you may add additional methods here
};

class StandardIO : public DefaultIO {
public:
    StandardIO() {

    }

    string read() override {
        string s;
        cin >> s;
        return s;
    }

    void write(string text) override {
        cout << text;
    }

    void write(float f) override {
        cout << f;
    }

    void read(float *f) override {
        cin >> *f;
    }

    ~StandardIO() {}
};
// you may add here helper classes


// you may edit this class
class Command {
protected:
    DefaultIO *dio;
    string m_description;
public:
    Command(DefaultIO *dio) : dio(dio) {}

    virtual string show() = 0;

    virtual void execute() = 0;

    virtual ~Command() {}
    float precision(float val, int n) {
        val = val * pow(10, n);
        val = floor(val);
        val = val / pow(10, n);
        return val;
    }
};


// implement here your command classes

class UploadCommand : public Command {
private:
    TimeSeries *m_train;
    TimeSeries *m_test;
public:
    UploadCommand(DefaultIO *dio, TimeSeries *tr, TimeSeries *ts) : Command(dio) {
        m_description = "upload a time series csv file\n";
        m_train = tr;
        m_test = ts;
    }

    string show() {
        return m_description;
    }

    void execute() {
        string string1 = "Please upload your local train CSV file.\n";
        dio->write(string1);
        *m_train = TimeSeries(readFromIO(dio));
        string1 = "Upload complete.\n";
        dio->write(string1);
        string1 = "Please upload your local test CSV file.\n";
        dio->write(string1);
        *m_test = TimeSeries(readFromIO(dio));
        string1 = "Upload complete.\n";
        dio->write(string1);
    }

    map<string, vector<float>> readFromIO(DefaultIO *dio) {
        map<string, vector<float>> features;
        // find keys
        string str = dio->read();
        while (!str.empty()) {
            // parameter of feature
            string token = str.substr(0, str.find(","));
            str.replace(str.find(token), token.length() + 1, "");
            features[token] = vector<float>(0);
        }
        // find values
        str = dio->read();
        while (strcmp(&str[0], "done") != 0) {
            for (auto pair : features) {
                string token = str.substr(0, str.find(","));
                str.replace(str.find(token), token.length() + 1, "");
                features[pair.first].push_back(stof(token));
            }
            str = dio->read();
        }
        return features;
    }
};

class algorithm : public Command {
private:
    HybridAnomalyDetector *m_hybrid;
public:
    algorithm(DefaultIO *dio, HybridAnomalyDetector *hybrid) : Command(dio) {
        m_description = "algorithm settings\n";
        m_hybrid = hybrid;
    }

    string show() {
        return m_description;
    }

    void execute() {
        bool flag = false;
        float updateThreshold;
        dio->write("The current correlation threshold is ");
        dio->write(precision(m_hybrid->getThreshold(), 1));
        dio->write("\n");
        string s;
        while (!flag) {
            dio->write("Type a new threshold\n");
            dio->read(&updateThreshold);
            if ((updateThreshold >= 0) && (updateThreshold <= 1)) {
                m_hybrid->setThreshold(updateThreshold);
                flag = true;
            } else {
                s = "please choose a value between 0 and 1.\n";
                dio->write(s);
            }
        }

    }

};

class anomalyDetectionCommand : public Command {
private:
    TimeSeries *m_train;
    TimeSeries *m_test;
    HybridAnomalyDetector *m_hybrid;
    vector<AnomalyReport> *m_anomalies;
public:
    anomalyDetectionCommand(DefaultIO *dio, TimeSeries *tr, TimeSeries *ts, HybridAnomalyDetector *hybridAnomalyDetector,
                            vector<AnomalyReport> *anomalies) : Command(dio) {
        m_description = "detect anomalies\n";
        m_train = tr;
        m_test = ts;
        m_hybrid = hybridAnomalyDetector;
        m_anomalies = anomalies;
    }

    string show() {
        return m_description;
    }

    void execute() {
        m_hybrid->learnNormal(*m_train);
        *m_anomalies = m_hybrid->detect(*m_test);
        dio->write("anomaly detection complete.\n");
    }
};

class display : public Command {
private:
    vector<AnomalyReport> *m_anomalies;
public:
    display(DefaultIO *dio, vector<AnomalyReport> *anomalies) : Command(dio) {
        m_description = "display results\n";
        m_anomalies = anomalies;

    }

    string show() {
        return m_description;
    }

    void execute() {
        for (AnomalyReport a: *m_anomalies) {
            dio->write(to_string(a.timeStep) + "\t" + a.description + "\n");
        }
        dio->write("Done.\n");
    }
};

class analyzeResults : public Command {
private:
    vector<AnomalyReport> *m_anomalies;
    TimeSeries *m_timeSeries;
public:
    analyzeResults(DefaultIO *dio, vector<AnomalyReport> *anomalies, TimeSeries *timeSeries) : Command(dio) {
        m_description = "upload anomalies and analyze results\n";
        m_anomalies = anomalies;
        m_timeSeries = timeSeries;
    }

    string show() {
        return m_description;
    }

    vector<pair<long, long>> compressRangeHelper(vector<long> v) {
        vector<pair<long, long>> result;
        int last = 0;
        result.push_back(make_pair(v[last], v[last]));
        int size = v.size();
        for (int i = 1; i < size; ++i) {
            if (result[last].second + 1 == v[i]) {
                result[last].second = v[i];
            } else {
                last++;
                result.push_back(make_pair(v[i], v[i]));
            }
        }
        return result;
    }

    vector<pair<long, long>> compressRange(vector<AnomalyReport> *anomalies) {
        vector<pair<long, long>> compressed;
        map<string, vector<long>> my_map;
        //this loop map description to timeStep.
        for (AnomalyReport anomaly:*anomalies) {
            my_map[anomaly.description].push_back(anomaly.timeStep);
        }
        vector<pair<long, long>> helper;
        for (auto a:my_map) {
            helper = compressRangeHelper(a.second);
            compressed.insert(compressed.end(), helper.begin(), helper.end());
        }
        return compressed;
    }

    float number_of_time_steps(vector<pair<long, long>> v) {
        int size = v.size();
        float sum = 0;
        for (int i = 0; i < size; i++) {
            sum += (v[i].second - v[i].first) + 1;
        }
        return sum;
    }

    void execute() {
        vector<pair<long, long>> my_anomalies_compress_range;
        vector<pair<long, long>> input_anomalies_compress_range;
        dio->write("Please upload your local anomalies file.\n");
        string input = dio->read();
        //this loop gives me at the end ,the min and the max range from the user
        while (strcmp(&input[0], "done") != 0) {
            string token1 = input.substr(0, input.find(","));
            input.replace(input.find(token1), token1.length() + 1, "");
            string token2 = input.substr(0, input.find(","));
            input_anomalies_compress_range.push_back(make_pair(stof(token1), stof(token2)));
            input=dio->read();

        }
        dio->write("Upload complete.\n");
        my_anomalies_compress_range = compressRange(m_anomalies);
        vector<pair<long, long>> cut = find_cut(input_anomalies_compress_range, my_anomalies_compress_range);
        //
        float TP = cut.size();
        float P = input_anomalies_compress_range.size();
        // write
        dio->write("True Positive Rate: ");
        dio->write(precision(TP / P, 3));
        dio->write("\n");

        float n = m_timeSeries->getLength(m_timeSeries->getFeatures()[0]);
        float N = n - number_of_time_steps(input_anomalies_compress_range);
        float FP = my_anomalies_compress_range.size() - cut.size();
        // write
        dio->write("False Positive Rate: ");
        dio->write(precision(FP / N, 3));
        dio->write("\n");
    }

    vector<pair<long, long>> find_cut(vector<pair<long, long>> given_anomalies_serial, vector<pair<long, long>> my_anomalies_serial) {
        int given_size = given_anomalies_serial.size();
        int my_size = my_anomalies_serial.size();
        vector<pair<long, long>> cuts;
        for (int i = 0; i < given_size; ++i) {
            for (int j = 0; j < my_size; ++j) {
                // if there is a cut
                if ((my_anomalies_serial[j].first >= given_anomalies_serial[i].first &&
                     my_anomalies_serial[j].first <= given_anomalies_serial[i].second) ||
                    (my_anomalies_serial[j].second >= given_anomalies_serial[i].first &&
                     my_anomalies_serial[j].second <= given_anomalies_serial[i].second)) {
                    cuts.push_back(my_anomalies_serial[j]);
                    continue;
                }
                if ((given_anomalies_serial[i].first >= my_anomalies_serial[j].first &&
                     given_anomalies_serial[i].first <= my_anomalies_serial[j].second) ||
                    given_anomalies_serial[i].second >= my_anomalies_serial[j].first &&
                    given_anomalies_serial[i].second <= my_anomalies_serial[j].second) {
                    cuts.push_back(my_anomalies_serial[j]);
                    continue;
                }
            }
        }
        return cuts;
    }
};

class exitCommand : public Command {
    bool *to_exit;
public:
    exitCommand(DefaultIO *dio, bool *b) : Command(dio) {
        m_description = "exit\n";
        to_exit = b;
    }

    string show() {
        return m_description;
    }

    void execute() {
        *to_exit = true;
    }
};

#endif /* COMMANDS_H_ */
