// Standard Libraries
#include <stdint.h>
#include <chrono>
#include <thread>
#include <fstream>
// ROOT Libraries
#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TApplication.h"
#include "TAxis.h"
#include "TSystem.h"
#include "TDatime.h"
#include "TStyle.h"
#include "TStopwatch.h"
// nlohmann JSON
#include "json.hpp"
// Project Headers
#include "Seeker.hpp"
#include "Ball.hpp"
#include "Forecaster.hpp"

using namespace std::chrono;

constexpr int window_size = 30;
constexpr double forecast_distance = 1000; // ms
int NUM_OF_CAMERA = 0;
bool CUSTOMIZATION = true;
string COLOR = "green";
int FRAME_WIDTH = 1280;
int FRAME_HEIGHT = 720;

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

/// Обновление графиков в отдельном потоке
void UPDATE_GRAPHS(TCanvas &c1) {
    while(1) {
        c1.cd(1);
        c1.Update();
        c1.Pad()->Draw();
        c1.cd(2);
        c1.Update();
        c1.Pad()->Draw();
        c1.cd(3);
        c1.Update();
        c1.Pad()->Draw();
    }
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
int main(int argc, char* argv[]) {

     Forecaster::datasize = size_t(window_size);
     Forecaster::forecast_distance = forecast_distance;
     // Forecaster::resizeTimeForecast();

     // ROOT PLOTS INIT
     TApplication rootapp("spectrum", &argc, argv);
     //set time offset
     chrono::milliseconds start_time_ms = chrono::duration_cast<chrono::milliseconds>(system_clock::now().time_since_epoch());

     auto c1 = new TCanvas("c1", "Data");
          c1->SetWindowSize(960, 960);
          c1->SetGrid();
    
     auto mg1 = new TMultiGraph("X во времени","X from time");
          mg1->GetXaxis()->SetTitle("Time");
          mg1->GetYaxis()->SetTitle("X");
          mg1->GetXaxis()->SetLimits(-(forecast_distance), forecast_distance);

     auto mg2 = new TMultiGraph("Y во времени","Y from time");
          mg2->GetXaxis()->SetTitle("Time");
          mg2->GetYaxis()->SetTitle("Y");
          mg2->GetXaxis()->SetLimits(-(forecast_distance), forecast_distance);

     auto mg3 = new TMultiGraph("Z во времени","Z from time");
          mg3->GetXaxis()->SetTitle("Time");
          mg3->GetYaxis()->SetTitle("Z");
          mg3->GetXaxis()->SetLimits(-(forecast_distance), forecast_distance);

     auto f1 = new TGraph(2);
          f1->SetName("Реальная координата X");
          f1->SetLineColor(2);
          f1->SetLineWidth(2);

     auto f1_forecast = new TGraph(2);
          f1_forecast->SetName("Прогнозная координата X");
          f1_forecast->SetLineColor(4);
          f1_forecast->SetLineWidth(2);

     auto f2 = new TGraph(2);
          f2->SetName("Реальная координата Y");
          f2->SetLineColor(2);
          f2->SetLineWidth(2);

     auto f2_forecast = new TGraph(2);
          f2_forecast->SetName("Прогнозная координата Y");
          f2_forecast->SetLineColor(4);
          f2_forecast->SetLineWidth(2);

     auto f3 = new TGraph(2);
          f3->SetName("Реальная координата Z");
          f3->SetLineColor(2);
          f3->SetLineWidth(2);

     auto f3_forecast = new TGraph(2);
          f3_forecast->SetName("Прогнозная координата Z");
          f3_forecast->SetLineColor(4);
          f3_forecast->SetLineWidth(2);
     
     double forecast_step = forecast_distance*2/window_size;

     c1->Divide(1, 3);
     mg1->Add(f1 );
     c1->cd(1);
     mg1->Add(f1_forecast);
     mg1->Draw("AL");
     c1->cd(2);
     mg2->Add(f2_forecast);
     mg2->Add(f2);
     mg2->Draw("AL");
     c1->cd(3);
     mg3->Add(f3);
     mg3->Add(f3_forecast);
     mg3->Draw("AL");

     TCanvas & cref = (*c1);
     thread th(UPDATE_GRAPHS, ref(cref));
     th.detach();

     // Take properties of ball color
     Ball ball(COLOR);
     //Ball* ptr = $ball;
     H_MIN = ball.getHSVmin()[0];
     H_MAX = ball.getHSVmax()[0];
     S_MIN = ball.getHSVmin()[1];
     S_MAX = ball.getHSVmax()[1];
     V_MIN = ball.getHSVmin()[2];
     V_MAX = ball.getHSVmax()[2];

     VideoCapture video(0);
     video.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
     video.set(CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
     video.set(CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

     if (CUSTOMIZATION)
     {
         Create_Customization();    
     }

     Seeker seeker;
     double time = 0;
     while (1)
     {
          video.read(BGR_frame);
          cvtColor(BGR_frame, HSV_frame, COLOR_BGR2HSV);
          inRange(HSV_frame, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), Output_frame);
          bool found = seeker.findObject(Out_ptr, BGR_ptr, FRAME_WIDTH, FRAME_HEIGHT);
          if (found) {
               int pxX = seeker.get_pxX();
               int pxY = seeker.get_pxY();
               int pxDiameter = seeker.get_pxDiameter();

               ball.setXPos(pxX);
               ball.setYPos(pxY);
               ball.setZDiameter(pxDiameter);
               ball.calculateZRealPos();
               ball.calculateXRealPos(FRAME_WIDTH, FRAME_HEIGHT);
               ball.calculateYRealPos(FRAME_WIDTH, FRAME_HEIGHT);

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
 
               // cout << XData_Forecasted.size() << endl;

               for(int i=0; i<XData.size() ;i++) {
                    f1->SetPoint(i, Time_Axis[i]-current_time.count(), XData[i]*1000);
                    f2->SetPoint(i, Time_Axis[i]-current_time.count(), YData[i]*1000);
                    f3->SetPoint(i, Time_Axis[i]-current_time.count(), ZData[i]*1000);
               };
               // for(int i=0; i<XData_Forecasted.size() ;i++) {
               //      f1_forecast->SetPoint(i, Time_Axis_Forecast[i]-current_time.count(), XData_Forecasted[i]*1000);
               //      f2_forecast->SetPoint(i, Time_Axis_Forecast[i]-current_time.count(), YData_Forecasted[i]*1000);
               //      f3_forecast->SetPoint(i, Time_Axis_Forecast[i]-current_time.count(), ZData_Forecasted[i]*1000);
               // };

               nlohmann::json j{};
               j["Time_Axis_Forecast"] = Time_Axis;
               j["XData"] = XData;
               j["YData"] = YData;
               j["ZData"] = ZData;
               
               std::ofstream file("Time.json");
               file << j;

          } else {

               // Nullify all data
               for(int i=Forecaster::num_of_chart_data; i>0 ;i--) {
                    f1->RemovePoint(i);
                    f2->RemovePoint(i);
                    f3->RemovePoint(i);
               };
               vector <double> XData_Forecasted = Forecaster::getXData_Forecasted();
               for (int i = XData_Forecasted.size(); i > 0; i--) {
                    f1_forecast->RemovePoint(i);
                    f2_forecast->RemovePoint(i);
                    f3_forecast->RemovePoint(i);
               }
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