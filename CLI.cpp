#include "CLI.h"
#include "timeseries.h"

#define SIZE 7

CLI::CLI(DefaultIO *dio) {
    m_dio = dio;
    m_command_array = vector<Command *>(SIZE);
    m_command_array[0] = nullptr;
    m_command_array[1] = new UploadCommand(m_dio, &train, &test);
    m_command_array[2] = new algorithm(m_dio, &hybrid);
    m_command_array[3] = new anomalyDetectionCommand(m_dio, &train, &test, &hybrid, &anomalies);
    m_command_array[4] = new display(m_dio, &anomalies);
    m_command_array[5] = new analyzeResults(m_dio, &anomalies,&test);
    m_command_array[6] = new exitCommand(m_dio, &exit);
}

void CLI::start() {
    while (!exit) {
        string s = "Welcome to the Anomaly Detection Server.\nPlease choose an option:\n";
        int size = m_command_array.size();
        for (int i = 1; i < size; i++) {
            s += to_string(i) + "." + m_command_array[i]->show();
        }
        m_dio->write(s);
        string sl = m_dio->read();
        int choice = stoi(sl);
        m_command_array[choice]->execute();
    }
}


CLI::~CLI() {
    int size=m_command_array.size();
    for (int i=1;i<size;i++) {
        free(m_command_array[i]);
    }
}

