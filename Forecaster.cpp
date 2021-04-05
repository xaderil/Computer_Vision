#include "Forecaster.hpp"

int Forecaster::num_of_chart_data = 0;
vector<double>* Forecaster::XData = new vector<double>;
vector<double>* Forecaster::YData = new vector<double>;
vector<double>* Forecaster::ZData = new vector<double>;
size_t Forecaster::datasize = 0;

// Заполнение массива данных X. Сначала заполняем нули числами, потом сдвигаем влево 
void Forecaster::addData (double X, double Y, double Z){
    if (num_of_chart_data == (datasize)) {
        Forecaster::vectorRotateLeft(Forecaster::XData, X);
        Forecaster::vectorRotateLeft(Forecaster::YData, Y);
        Forecaster::vectorRotateLeft(Forecaster::ZData, Z);
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

/// Сдвиг данных влево
void Forecaster::vectorRotateLeft(vector<double>* data, double num)
{
    try {
    for(std::size_t i = 0; i < data->size()-1; i++) {
    (*data)[i] = (*data)[i+1];
    (*data)[data->size()-1] = num;
    };
    } catch (exception err) {
        cout << err.what() << endl;
    }
};