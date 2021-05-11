#pragma once

#include <vector>
#include <chrono>

#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TApplication.h"
#include "TAxis.h"
#include "TSystem.h"
#include "TDatime.h"
#include "TStyle.h"
#include "TStopwatch.h"

using namespace std;
class Monitoring {

public:        
 
    Monitoring(int argc, char* argv[], double forecast_distance);

    void nullifyAllData(int current_size, int size_forecasted);
    void setPointsToCurrentData(vector <double> XData, vector <double> YData, vector<double> ZData, vector <double> Time_Axis, chrono::duration<double, std::milli> current_time);
    void setPointsToForecastedData(vector <double> XData_Forecasted, vector <double> YData_Forecasted, vector <double> ZData_Forecasted, vector <double> Time_Axis_Forecast, chrono::duration<double, std::milli> current_time);

    TApplication* rootapp;
    TCanvas* c1;
    TMultiGraph* mg1;
    TMultiGraph* mg2;
    TMultiGraph* mg3;
    TGraph* f1;
    TGraph* f2;
    TGraph* f3;
    TGraph* f1_forecast;
    TGraph* f2_forecast;
    TGraph* f3_forecast;
};