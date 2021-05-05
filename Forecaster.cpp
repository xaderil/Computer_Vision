#include "Forecaster.hpp"

int Forecaster::num_of_chart_data = 0;
size_t Forecaster::datasize = 0;
int Forecaster::forecast_distance = 0;

vector<double>* Forecaster::XData = new vector<double>;
vector<double>* Forecaster::YData = new vector<double>;
vector<double>* Forecaster::ZData = new vector<double>;

vector<double>* Forecaster::XData_Forecasted = new vector<double>;
vector<double>* Forecaster::YData_Forecasted = new vector<double>;
vector<double>* Forecaster::ZData_Forecasted = new vector<double>;

vector<double>* Forecaster::Time_Data = new vector<double>;
vector<double>* Forecaster::Time_Data_Forecast = new vector<double>;

// Заполнение массива данных X. Сначала заполняем нули числами, потом сдвигаем влево 
void Forecaster::addData (double X, double Y, double Z, double time){
    if (num_of_chart_data == (datasize)) {
        Forecaster::XData->erase(XData->cbegin());
        Forecaster::YData->erase(YData->cbegin());
        Forecaster::ZData->erase(ZData->cbegin());
        Forecaster::XData->push_back(X);
        Forecaster::YData->push_back(Y);
        Forecaster::ZData->push_back(Z);

        Forecaster::Time_Data->erase(Time_Data->cbegin());
        Forecaster::Time_Data->push_back(time);
    }
    else {
        Forecaster::XData->push_back(X);
        Forecaster::YData->push_back(Y);
        Forecaster::ZData->push_back(Z);

        Forecaster::Time_Data->push_back(time);
        Forecaster::num_of_chart_data++;
    };
};

void Forecaster::setToZeroData() {
    Forecaster::num_of_chart_data = 0;
    Forecaster::XData->resize(0);
    Forecaster::YData->resize(0);
    Forecaster::ZData->resize(0);
    Forecaster::Time_Data->resize(0);

}

vector<double> Forecaster::getXData() {
    return *(Forecaster::XData);
};

vector<double> Forecaster::getYData() {
    return *(Forecaster::YData);
};

vector<double> Forecaster::getZData() {
    return *(Forecaster::ZData);
};

vector<double> Forecaster::getXData_Forecasted() {
    return *(Forecaster::XData_Forecasted);
};

vector<double> Forecaster::getYData_Forecasted() {
    return *(Forecaster::YData_Forecasted);
};

vector<double> Forecaster::getZData_Forecasted() {
    return *(Forecaster::ZData_Forecasted);
};

vector<double> Forecaster::getTime_Data() {
    return *(Forecaster::Time_Data);
};

vector<double> Forecaster::getTime_Data_Forecast() {
    return *(Forecaster::Time_Data_Forecast);
};

void Forecaster::makeForecast(double time, double time_first_real, bool time_differece) {

    // if  (XData_Forecasted->size() == datasize) {

    //     XData_Forecasted->erase(XData_Forecasted->cbegin());
    //     YData_Forecasted->erase(YData_Forecasted->cbegin());
    //     ZData_Forecasted->erase(ZData_Forecasted->cbegin());

    //     XData_Forecasted->push_back(XData[0][0]);
    //     YData_Forecasted->push_back(XData[0][0]);
    //     ZData_Forecasted->push_back(XData[0][0]);

    // } else {
    //     XData_Forecasted->push_back(XData[0][0]);
    //     YData_Forecasted->push_back(YData[0][0]);
    //     ZData_Forecasted->push_back(ZData[0][0]);
    //     // Time_Data_Forecast->push_back(time + forecast_distance);
    // };
    
    //     Time_Data_Forecast->push_back(time + forecast_distance);
    //     Time_Data_Forecast->erase(Time_Data_Forecast->cbegin());
}

bool Forecaster::compareTimeAxis() {

    // if ((Forecaster::Time_Data_Forecast[0][0] - Forecaster::Time_Data[0][0]) > 50) {
        
    //     cout << "true" << endl;
    //     double hui = Forecaster::Time_Data_Forecast[0][0] - Forecaster::Time_Data[0][0];
    //     cout << hui << endl;
    //     return true;

    // } else {
    //     cout << "false" << endl;
    //     double hui = Forecaster::Time_Data_Forecast[0][0] - Forecaster::Time_Data[0][0];
    //     cout << hui << endl;
    //     return false;
    // };

};


void Forecaster::resizeTimeForecast() {

    Forecaster::Time_Data_Forecast->resize(datasize);

}