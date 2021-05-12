#define FRME_HEIGHT 720
#define FRME_WIDTH 1280
#define COLOR "green"
#define CUSTOMIZATION true
#define NUM_OF_CAMERA 0
#define FORECAST_DISTANCE 1000
#define WINDOW_SIZE 30
#define GRAPHS_TYPE 1 // 0 - FLAT, 1 - SPATIAL

// Standard Libraries
#include <boost/asio.hpp>
// Project Headers
#include "Seeker.hpp"
#include "Ball.hpp"
#include "Forecaster.hpp"
#include "Monitoring.hpp"
#include "JSON_Worker.hpp"

using namespace std::chrono;

        /// НА УДАЛЕНИЕ
        int H_MIN = 0;
        int H_MAX = 256;
        int S_MIN = 0;
        int S_MAX = 256;
        int V_MIN = 0;
        int V_MAX = 256;

        cv::Mat BGR_frame;
        cv::Mat HSV_frame;
        cv::Mat Output_frame;
        cv::Mat* Out_ptr = &Output_frame;
        cv::Mat* BGR_ptr = &BGR_frame;
        ///


void Create_Customization();

int main(int argc, char* argv[]) {

    JSON_Worker JSON_Worker;

    Forecaster::datasize = size_t(WINDOW_SIZE);
    Forecaster::forecast_distance = FORECAST_DISTANCE;

    Monitoring::plot_type = GRAPHS_TYPE;
    Monitoring Monitor(argc, argv, FORECAST_DISTANCE, GRAPHS_TYPE);

    //set time offset
    chrono::milliseconds start_time_ms = chrono::duration_cast<chrono::milliseconds>(system_clock::now().time_since_epoch());
    
    // Create another worker
    Monitoring & MonitoringRef = Monitor;
    thread th(Monitoring::updateGraphs, ref(MonitoringRef));
    th.detach();

    Seeker seeker;

        // Take properties of ball color  /// Почистить кусок
        Ball ball(COLOR);
        H_MIN = ball.getHSVmin()[0];
        H_MAX = ball.getHSVmax()[0];
        S_MIN = ball.getHSVmin()[1];
        S_MAX = ball.getHSVmax()[1];
        V_MIN = ball.getHSVmin()[2];
        V_MAX = ball.getHSVmax()[2];
        ///

    VideoCapture video(0);
    video.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    video.set(CAP_PROP_FRAME_WIDTH, FRME_WIDTH);
    video.set(CAP_PROP_FRAME_HEIGHT, FRME_HEIGHT);

     if (CUSTOMIZATION)
     {
         Create_Customization();    
     }

    double time = 0;
    while (1)
    {
        video.read(BGR_frame);
        cvtColor(BGR_frame, HSV_frame, COLOR_BGR2HSV);
        inRange(HSV_frame, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), Output_frame);
        bool found = seeker.findObject(Out_ptr, BGR_ptr, FRME_WIDTH, FRME_HEIGHT);
        if (found) {

            int pxX = seeker.get_pxX();
            int pxY = seeker.get_pxY();
            int pxDiameter = seeker.get_pxDiameter();

            ball.setXPos(pxX);
            ball.setYPos(pxY);
            ball.setZDiameter(pxDiameter);
            ball.calculateZRealPos();
            ball.calculateXRealPos(FRME_WIDTH, FRME_HEIGHT);
            ball.calculateYRealPos(FRME_WIDTH, FRME_HEIGHT);

            float xRealPos = ball.getXRealPos();
            float yRealPos = ball.getYRealPos();
            float zRealPos = ball.getZRealPos();
            auto start = std::chrono::steady_clock::now();
            seeker.drawObject(BGR_ptr,xRealPos, yRealPos, zRealPos, pxX, pxY);

            chrono::milliseconds ms = chrono::duration_cast<chrono::milliseconds>(system_clock::now().time_since_epoch());
            chrono::duration<double, std::milli> current_time = ms - start_time_ms; 

            Forecaster::addData(xRealPos, yRealPos, zRealPos, current_time.count());
            
            // double diff = current_time.count() - time;
            // cout << diff << endl;

            vector <double> Time_Axis = Forecaster::getTime_Data();
            
            Forecaster::makeForecast(current_time.count(), Time_Axis[0]);

            vector <double> XData = Forecaster::getXData();
            vector <double> YData = Forecaster::getYData();
            vector <double> ZData = Forecaster::getZData();
            time = current_time.count(); 
            vector <double> XData_Forecasted = Forecaster::getXData_Forecasted();
            vector <double> YData_Forecasted = Forecaster::getYData_Forecasted();
            vector <double> ZData_Forecasted = Forecaster::getZData_Forecasted();

            vector <double> Time_Axis_Forecast = Forecaster::getTime_Data_Forecast();

            Monitor.setPointsToCurrentData(XData, YData, ZData, Time_Axis, current_time);
        //    Monitoring.setPointsToForecastedData(XData_Forecasted,YData_Forecasted, ZData_Forecasted, Time_Axis_Forecast, current_time);
            
            JSON_Worker.setData(Time_Axis, XData, YData, ZData, current_time.count());
            JSON_Worker.generateFile("../Matlab_Checker/Data.json");



        } else {

            // Nullify all data
            vector <double> XData_Forecasted = Forecaster::getXData_Forecasted();
            Monitor.nullifyAllData(Forecaster::num_of_chart_data, XData_Forecasted.size());
            Forecaster::nullifyData();
        }

        imshow("Cam", BGR_frame);
        imshow("Customization", Output_frame);

        if (waitKey(15) >= 0)
        {
            break;
        }
        }
        return 0;

};

/// Создание окна кастомизации
void Create_Customization() {
    namedWindow("Customization", WINDOW_AUTOSIZE);
    createTrackbar("H_MIN", "Customization", &H_MIN, 256, 0);
    createTrackbar("H_MAX", "Customization", &H_MAX, 256, 0);
    createTrackbar("S_MIN", "Customization", &S_MIN, 256, 0);
    createTrackbar("S_MAX", "Customization", &S_MAX, 256, 0);
    createTrackbar("V_MIN", "Customization", &V_MIN, 256, 0);
    createTrackbar("V_MAX", "Customization", &V_MAX, 256, 0);
    createTrackbar("F_Coeff", "Customization", &Ball::f, 1500, 0);
};