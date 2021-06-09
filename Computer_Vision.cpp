#define FRME_HEIGHT 720
#define FRME_WIDTH 1280
#define COLOR "green"
#define CUSTOMIZATION true
#define NUM_OF_CAMERA 0
#define FORECAST_DISTANCE 1000
#define WINDOW_SIZE 30
#define GRAPHS_TYPE 0 // 0 - FLAT, 1 - SPATIAL
#define EXECUTION_TYPE 1 // 0 - REAL TIME, 1 - DELAY FRAMES

#include "opencv2/calib3d.hpp"
// Project Headers
#include "Seeker.hpp"
#include "Ball.hpp"
#include "Forecaster.hpp"
#include "Monitoring.hpp"
#include "JSON_Worker.hpp"


using namespace std::chrono;

                            cv::Mat BGR_frame;
                            cv::Mat HSV_frame;
                            cv::Mat Output_frame;
                            cv::Mat Delay_frame;
                            cv::Mat Difference;
                            cv::Mat Previous_frame;
                            Mat res;
                            ///


void Create_Customization();

int main(int argc, char* argv[]) {

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

    Ball ball(COLOR);

    VideoCapture video(0);
    video.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    video.set(CAP_PROP_FRAME_WIDTH, FRME_WIDTH);
    video.set(CAP_PROP_FRAME_HEIGHT, FRME_HEIGHT);

     if (CUSTOMIZATION)
     {
         Create_Customization();    
     }

    bool trajectory_found_ever = 0;
    bool static_record_switched = 0;
    double time = 0;

            double m[3][3] = {{939.74, 0, 644.63}, {0, 938.12, 366.32}, {0, 0, 1}};
            Mat cameraMatrix = Mat(3,3, CV_64F, m);

            double distcoef[4] = {0.0117, -0.2445,0.0034, 0.0102};
            Mat distCoefs = Mat(1,4, CV_64F, distcoef);

            Mat BGR_Undistorted;
    while (1)
    {
        video.read(BGR_frame);
        if (Delay_frame.empty()) {
            BGR_frame.copyTo(Delay_frame);
            BGR_frame.copyTo(Difference);
            BGR_frame.copyTo(res);
        };

        // undistort(BGR_frame, BGR_Undistorted, cameraMatrix, distCoefs);

        cvtColor(BGR_frame, HSV_frame, COLOR_BGR2HSV);
        inRange(HSV_frame, Scalar(Ball::H_MIN, Ball::S_MIN, Ball::V_MIN), Scalar(Ball::H_MAX, Ball::S_MAX, Ball::V_MAX), Output_frame);
        bool found = seeker.findObject(&Output_frame, &BGR_frame, FRME_WIDTH, FRME_HEIGHT);
        if (found) {
            
            trajectory_found_ever = 1;
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
            seeker.drawObject(&BGR_frame,xRealPos, yRealPos, zRealPos, pxX, pxY);

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

            Mat yee = seeker.getBallImage();
            cout << yee.type() << "  " << yee.size() << endl;
            Delay_frame = 0;
            Delay_frame = Mat::zeros(720, 1280, CV_8UC1);
            cout << Delay_frame.type() << "  " << Delay_frame.size() << endl;
            yee.copyTo(Delay_frame);

        //    Monitoring.setPointsToForecastedData(XData_Forecasted,YData_Forecasted, ZData_Forecasted, Time_Axis_Forecast, current_time);

            if (JSON_Worker::static_data_Recording_Flag) {
                
                static_record_switched = 1;

                ball.staticXData.push_back(ball.getXRealPos());
                ball.staticYData.push_back(ball.getYRealPos());
                ball.staticZData.push_back(ball.getZRealPos());
            } else {
                if (static_record_switched) {

                    JSON_Worker::setCurrentStaticMetroData(ball.staticXData, ball.staticYData, ball.staticZData);
                    JSON_Worker::generateFileWithStaticMetroData("../Matlab_Checker/StaticData.json");
                    JSON_Worker::ball_static_record_counter++;
                    static_record_switched = 0;

                    ball.staticXData.resize(0);
                    ball.staticYData.resize(0);
                    ball.staticZData.resize(0);
                };

            };


            // addWeighted(Difference, 1, Delay_frame, 1, 0, Delay_frame);


        } else {
            
            if (trajectory_found_ever) {

                trajectory_found_ever = 0;

                chrono::milliseconds ms = chrono::duration_cast<chrono::milliseconds>(system_clock::now().time_since_epoch());
                chrono::duration<double, std::milli> current_time = ms - start_time_ms; 
                vector <double> XData = Forecaster::getXData();
                vector <double> YData = Forecaster::getYData();
                vector <double> ZData = Forecaster::getZData();
                vector <double> Time_Axis = Forecaster::getTime_Data();

                JSON_Worker::setCurrentDynamicData(Time_Axis, XData, YData, ZData, current_time.count());
                JSON_Worker::generateFileWithDynamicMetroData("../Matlab_Checker/Trajectories.json");
                JSON_Worker::ball_trajectory_counter++;

            }


            // Nullify all data

            vector <double> XData_Forecasted = Forecaster::getXData_Forecasted();
            Monitor.nullifyAllData(Forecaster::num_of_chart_data, XData_Forecasted.size());
            Forecaster::nullifyData();

            BGR_frame.copyTo(Delay_frame);
            BGR_frame.copyTo(Previous_frame);
            BGR_frame.copyTo(res);
        };

        if (EXECUTION_TYPE == 0) {

            imshow("Cam", BGR_frame);

        } else {

            imshow("Cam", Delay_frame);

        };

        imshow("Customization", Output_frame);

        if (waitKey(15) >= 0) {
            break;
        };
    };
    
    return 0;

};

/// Создание окна кастомизации
void Create_Customization() {
    namedWindow("Customization", WINDOW_AUTOSIZE);
    createTrackbar("H_MIN", "Customization", &Ball::H_MIN, 256, JSON_Worker::rewriteHSVData);
    createTrackbar("H_MAX", "Customization", &Ball::H_MAX, 256, JSON_Worker::rewriteHSVData);
    createTrackbar("S_MIN", "Customization", &Ball::S_MIN, 256, JSON_Worker::rewriteHSVData);
    createTrackbar("S_MAX", "Customization", &Ball::S_MAX, 256, JSON_Worker::rewriteHSVData);
    createTrackbar("V_MIN", "Customization", &Ball::V_MIN, 256, JSON_Worker::rewriteHSVData);
    createTrackbar("V_MAX", "Customization", &Ball::V_MAX, 256, JSON_Worker::rewriteHSVData);
    createTrackbar("F_Coeff", "Customization", &Ball::f, 1500, JSON_Worker::rewriteHSVData);
    createTrackbar("Data recording", "Customization", &JSON_Worker::static_data_Recording_Flag, 1, 0);
};