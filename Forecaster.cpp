#include "Forecaster.hpp"

int Forecaster::num_of_chart_data = 0;
size_t Forecaster::datasize = 0;
int Forecaster::forecast_distance = 0;
int Forecaster::forecast_counter = 0;

vector<double>* Forecaster::XData = new vector<double>;
vector<double>* Forecaster::YData = new vector<double>;
vector<double>* Forecaster::ZData = new vector<double>;
vector<double>* Forecaster::UData = new vector<double>;
vector<double>* Forecaster::VData = new vector<double>;

vector<double>* Forecaster::XData_Forecasted = new vector<double>;
vector<double>* Forecaster::YData_Forecasted = new vector<double>;
vector<double>* Forecaster::ZData_Forecasted = new vector<double>;

vector<double>* Forecaster::Time_Data = new vector<double>;
vector<double>* Forecaster::Time_Data_Forecast = new vector<double>;

// Заполнение массива данных X. Сначала заполняем нули числами, потом сдвигаем влево 
void Forecaster::addData (double X, double Y, double Z, double U, double V, double time){
    if (num_of_chart_data == (datasize)) {
        Forecaster::XData->erase(XData->cbegin());
        Forecaster::YData->erase(YData->cbegin());
        Forecaster::ZData->erase(ZData->cbegin());
        Forecaster::UData->erase(UData->cbegin());
        Forecaster::VData->erase(VData->cbegin());
        Forecaster::XData->push_back(X);
        Forecaster::YData->push_back(Y);
        Forecaster::ZData->push_back(Z);
        Forecaster::UData->push_back(U);
        Forecaster::VData->push_back(V);

        Forecaster::Time_Data->erase(Time_Data->cbegin());
        Forecaster::Time_Data->push_back(time);
    }
    else {
        Forecaster::XData->push_back(X);
        Forecaster::YData->push_back(Y);
        Forecaster::ZData->push_back(Z);
        Forecaster::UData->push_back(U);
        Forecaster::VData->push_back(V);

        Forecaster::Time_Data->push_back(time);
        Forecaster::num_of_chart_data++;
    };
};

void Forecaster::nullifyData() {
    Forecaster::num_of_chart_data = 0;
    Forecaster::XData->resize(0);
    Forecaster::YData->resize(0);
    Forecaster::ZData->resize(0);
    Forecaster::UData->resize(0);
    Forecaster::VData->resize(0);
    Forecaster::Time_Data->resize(0);

    Forecaster::XData_Forecasted->resize(0);
    Forecaster::YData_Forecasted->resize(0);
    Forecaster::ZData_Forecasted->resize(0);
    Forecaster::Time_Data_Forecast->resize(0);

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

vector<double> Forecaster::getUData() {
    return *(Forecaster::UData);
};

vector<double> Forecaster::getVData() {
    return *(Forecaster::VData);
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

void Forecaster::makeForecast(double time, double time_first_real) {

    
    if (num_of_chart_data == datasize) {

        forecast_counter++;

        if (forecast_counter == 1) {

            for (int i = 0; i < datasize; i++) {

                XData_Forecasted->push_back(XData[0][i]);
                YData_Forecasted->push_back(YData[0][i]);
                ZData_Forecasted->push_back(ZData[0][i]);
                Time_Data_Forecast->push_back(Time_Data[0][i] + forecast_distance);

            };

        } else {

            bool similar_time_axis = compareTimeAxis();

            if (!similar_time_axis) {

                XData_Forecasted->push_back(XData[0][datasize-1]);
                YData_Forecasted->push_back(YData[0][datasize-1]);
                ZData_Forecasted->push_back(ZData[0][datasize-1]);
                Time_Data_Forecast->push_back(Time_Data[0][datasize-1] + forecast_distance);

            } else {

                XData_Forecasted->erase(XData_Forecasted->cbegin());
                YData_Forecasted->erase(YData_Forecasted->cbegin());
                ZData_Forecasted->erase(ZData_Forecasted->cbegin());
                Time_Data_Forecast->erase(Time_Data_Forecast->cbegin());
                
                XData_Forecasted->push_back(XData[0][datasize-1]);
                YData_Forecasted->push_back(XData[0][datasize-1]);
                ZData_Forecasted->push_back(XData[0][datasize-1]);
                Time_Data_Forecast->push_back(Time_Data[0][datasize-1] + forecast_distance);
            };

        }
    };
};

bool Forecaster::compareTimeAxis() {

    if (Time_Data_Forecast->size() != 0) {

        if ((Forecaster::Time_Data_Forecast[0][0] - Forecaster::Time_Data[0][0]) < 50) {
            
            double yee = Forecaster::Time_Data_Forecast[0][0] - Forecaster::Time_Data[0][0];
            return true;

        } else {
            double yee = Forecaster::Time_Data_Forecast[0][0] - Forecaster::Time_Data[0][0];
            return false;
        };

    } else {
        return false;
    };

};

