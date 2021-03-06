#define FRME_HEIGHT 720
#define FRME_WIDTH 1280
#define COLOR "green"
#define CUSTOMIZATION false
#define NUM_OF_CAMERA 0 
#define FORECAST_DISTANCE 1000 // IN MILLISECONDS
#define WINDOW_SIZE 30 // POINTS IN EACH GRAPH
#define GRAPHS_TYPE 0 // 0 - FLAT, 1 - SPATIAL
#define BACKGROUND 1 // 0 - DISABLED, 1 - ENABLED
#define EXECUTION_TYPE 1 // 0 - REAL TIME, 1 - WITH FREEZED BALL TRAJECTORY

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
                            cv::Mat Delay_Frame;


void Create_Customization();

int main(int argc, char* argv[]) {
    
    Forecaster::datasize = size_t(WINDOW_SIZE);
    Forecaster::forecast_distance = FORECAST_DISTANCE;

    Monitoring::plot_type = GRAPHS_TYPE;
    Monitoring Monitor(argc, argv, FORECAST_DISTANCE, GRAPHS_TYPE);

    //set time offset
    chrono::milliseconds start_time_ms = chrono::duration_cast<chrono::milliseconds>(system_clock::now().time_since_epoch());

    int pictureNum = 0;

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
            if (EXECUTION_TYPE == 1) {
                seeker.enableFreezedTrajectory(&BGR_frame);
            }
            if (BACKGROUND == 0) {
                seeker.disableBackground(&BGR_frame);   
            }
            chrono::milliseconds ms = chrono::duration_cast<chrono::milliseconds>(system_clock::now().time_since_epoch());
            // seeker.drawObject(&BGR_frame,xRealPos, yRealPos, zRealPos, pxX, pxY);
            chrono::duration<double, std::milli> current_time = ms - start_time_ms; 
            BGR_frame.copyTo(Delay_Frame);
            Forecaster::addData(xRealPos, yRealPos, zRealPos, pxX, pxY, current_time.count());
            

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


        } else {
            
            if (trajectory_found_ever) {

                trajectory_found_ever = 0;
                pictureNum++;
                chrono::milliseconds ms = chrono::duration_cast<chrono::milliseconds>(system_clock::now().time_since_epoch());
                chrono::duration<double, std::milli> current_time = ms - start_time_ms; 
                vector <double> XData = Forecaster::getXData();
                vector <double> YData = Forecaster::getYData();
                vector <double> ZData = Forecaster::getZData();
                vector <double> UData = Forecaster::getUData();
                vector <double> VData = Forecaster::getVData();
                vector <double> Time_Axis = Forecaster::getTime_Data();

                JSON_Worker::setCurrentDynamicData(Time_Axis, XData, YData, ZData, UData, VData, current_time.count());
                JSON_Worker::generateFileWithDynamicMetroData("../Matlab_Checker/Trajectories.json");
                JSON_Worker::ball_trajectory_counter++;
                
                // Nullify all data
                seeker.freezedTrajectory.setTo(0);
                vector <double> XData_Forecasted = Forecaster::getXData_Forecasted();
                Monitor.nullifyAllData(Forecaster::num_of_chart_data, XData_Forecasted.size());
                Forecaster::nullifyData();

                if (EXECUTION_TYPE == 1) {
                    string Name = "Pictures/trajectoryPicture" + to_string(pictureNum) + ".png";
                    imwrite(Name, Delay_Frame);
                }

            }

        };

        imshow("Cam", BGR_frame);

        if (CUSTOMIZATION == 1) {
            imshow("Customization", Output_frame);
        }

        if (waitKey(15) >= 0) {
            break;
        };
    };
    
    return 0;

};

/// ???????????????? ???????? ????????????????????????
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