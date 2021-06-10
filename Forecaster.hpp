using namespace std;

#include <vector>
#include <iostream>

class Forecaster {

public:
    Forecaster();

    static int              num_of_chart_data;
    static size_t           datasize;
    static int              forecast_distance; // Milliseconds 
    static int              forecast_counter;
    
    static vector<double>   getTime_Data();
    static vector<double>   getTime_Data_Forecast();

    static void             addData(double X, double Y, double Z, double U, double V, double time);
    static void             nullifyData();

    static vector<double>   getXData();
    static vector<double>   getYData();
    static vector<double>   getZData();
    static vector<double>   getUData();
    static vector<double>   getVData();
    static vector<double>   getXData_Forecasted();
    static vector<double>   getYData_Forecasted();
    static vector<double>   getZData_Forecasted();    

    static void             makeForecast(double time, double first_real);
    static bool             compareTimeAxis(); // Check a differece between first points of time
    
    static double           calculateCoordinates();
    

private:

    static vector<double>*  XData;
    static vector<double>*  YData;
    static vector<double>*  ZData;
    static vector<double>*  UData;
    static vector<double>*  VData;

    static vector<double>*  XData_Forecasted;
    static vector<double>*  YData_Forecasted;
    static vector<double>*  ZData_Forecasted;

    static vector<double>*  Time_Data;
    static vector<double>*  Time_Data_Forecast;

    
};