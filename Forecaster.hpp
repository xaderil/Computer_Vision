using namespace std;

#include <vector>
#include <iostream>

class Forecaster {

public:
    Forecaster();

    static int num_of_chart_data;
    static size_t datasize;

    static void vectorRotateLeft(vector<double>* data, double num); // В дальнейшем избавиться 
    
    static void addData(double X, double Y, double Z);
    static vector<double> getXData();

    static void addYData(double Y);
    static vector<double> getYData();

    static void addZData(double Z);
    static vector<double> getZData();




private:

    static vector<double>* XData;
    static vector<double>* YData;
    static vector<double>* ZData;
    
};