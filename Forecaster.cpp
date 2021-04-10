#include "Forecaster.hpp"

int Forecaster::num_of_chart_data = 0;
vector<double>* Forecaster::XData = new vector<double>;
vector<double>* Forecaster::YData = new vector<double>;
vector<double>* Forecaster::ZData = new vector<double>;
size_t Forecaster::datasize = 0;

// Заполнение массива данных X. Сначала заполняем нули числами, потом сдвигаем влево 
void Forecaster::addData (double X, double Y, double Z){
    if (num_of_chart_data == (datasize)) {
        Forecaster::XData->erase(XData->cbegin());
        Forecaster::YData->erase(YData->cbegin());
        Forecaster::ZData->erase(ZData->cbegin());
        Forecaster::XData->push_back(X);
        Forecaster::YData->push_back(Y);
        Forecaster::ZData->push_back(Z);
    }
    else {
        Forecaster::XData->push_back(X);
        Forecaster::YData->push_back(Y);
        Forecaster::ZData->push_back(Z);
        Forecaster::num_of_chart_data++;
    };
};


vector<double> Forecaster::getXData() {
    return *(Forecaster::XData);
};

vector<double> Forecaster::getYData() {
    return *(Forecaster::YData);
};

vector<double> Forecaster::getZData() {
    return *(Forecaster::ZData);
};
