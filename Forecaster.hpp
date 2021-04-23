using namespace std;

#include <vector>
#include <iostream>

class Forecaster {

public:
    Forecaster();

    static int num_of_chart_data;
    static size_t datasize;
    static int forecast_distance; // In milliseconds 
    
    static vector<double> getTime_Data();
    static vector<double> getTime_Data_Forecast();

    static void resizeTimeForecast();

    static void addData(double X, double Y, double Z, double time);
    static vector<double> getXData();

    static void addYData(double Y);
    static vector<double> getYData();

    static void addZData(double Z);
    static vector<double> getZData();

    static void makeForecast(double time, double first_real);

    static vector<double> getXData_Forecasted();
    static vector<double> getYData_Forecasted();
    static vector<double> getZData_Forecasted();    

private:

    static vector<double>* XData;
    static vector<double>* YData;
    static vector<double>* ZData;

    static vector<double>* XData_Forecasted;
    static vector<double>* YData_Forecasted;
    static vector<double>* ZData_Forecasted;

    static vector<double>* Time_Data;
    static vector<double>* Time_Data_Forecast;

    
};