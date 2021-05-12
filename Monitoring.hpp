#pragma once
#include <thread>
#include <vector>
#include <chrono>

#include <TCanvas.h>
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TApplication.h"
#include "TPolyLine3D.h"
#include "TGraph2D.h"
#include "TAxis.h"
#include "TSystem.h"
#include "TDatime.h"
#include "TStyle.h"
#include "TStopwatch.h"

using namespace std;
class Monitoring {

public:        
    
    Monitoring(int argc, char* argv[], double forecast_distance, int plot_type);

    void             nullifyAllData(int current_size, int size_forecasted);
    void             setPointsToCurrentData(vector <double> XData, vector <double> YData, vector<double> ZData, vector <double> Time_Axis, chrono::duration<double, std::milli> current_time);
    void             setPointsToForecastedData(vector <double> XData_Forecasted, vector <double> YData_Forecasted, vector <double> ZData_Forecasted, vector <double> Time_Axis_Forecast, chrono::duration<double, std::milli> current_time);
    static void      updateGraphs(Monitoring & Monitoring);
    static int       plot_type;

    TApplication*    rootapp;

    /// 2d graphs
    TCanvas*         c1;
    TMultiGraph*     mg1;
    TMultiGraph*     mg2;
    TMultiGraph*     mg3;
    TGraph*          f1;
    TGraph*          f2;
    TGraph*          f3;
    TGraph*          f1_forecast;
    TGraph*          f2_forecast;
    TGraph*          f3_forecast;

    /// 3d graphs
    TCanvas*         c;
    TGraph2D*        g;
    TPolyLine3D*     pl;

};