// Other Libraries
#include <stdint.h>
#include <chrono>
#include <thread>
// ROOT Libraries
#include "TCanvas.h"
#include "TGraph.h"
#include "TApplication.h"
#include "TAxis.h"
#include "TSystem.h"
// Project Headers
#include "Seeker.hpp"
#include "Ball.hpp"
#include "Forecaster.hpp"

using namespace std::chrono;

constexpr int window_size = 50; 
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

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
};
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
    // ROOT PLOTS INIT
    TApplication rootapp("spectrum", &argc, argv);
    // auto c1 = make_unique<TCanvas>("c1", "Data");
    TCanvas* c1 = new TCanvas("c1", "Data");
    TCanvas & cref = (*c1);
    c1->SetWindowSize(960, 960);
    auto f1 = make_unique<TGraph>(2);
    f1->SetTitle("X");
    f1->GetXaxis()->SetTitle("Time");
    f1->GetYaxis()->SetTitle("X");
    f1->SetMinimum(-500);
    f1->SetMaximum(500);
    auto f2 = make_unique<TGraph>(2);
    f2->SetTitle("Y");
    f2->GetXaxis()->SetTitle("Time");
    f2->GetYaxis()->SetTitle("Y");
    f2->SetMinimum(-500);
    f2->SetMaximum(500);

    auto f3 = make_unique<TGraph>(2);
    f3->SetTitle("Z");
    f3->GetXaxis()->SetTitle("Time");
    f3->GetYaxis()->SetTitle("Z");
    f3->SetMinimum(50);
    f3->SetMaximum(1100);

    c1->Divide(1, 3);
    c1->cd(1);
    f1->Draw();
    c1->cd(2);
    f2->Draw();
    c1->cd(3);
    f3->Draw();

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
    Forecaster::datasize = size_t(window_size);

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

            seeker.drawObject(BGR_ptr,xRealPos, yRealPos, zRealPos, pxX, pxY);

            Forecaster::addData(xRealPos, yRealPos, zRealPos);

            vector <double> XData = Forecaster::getXData();
            vector <double> YData = Forecaster::getYData();
            vector <double> ZData = Forecaster::getZData();

            for(size_t i=0; i<XData.size() ;i++) {
                f1->SetPoint(i, i, XData[i]*1000);
                f2->SetPoint(i, i, YData[i]*1000);
                f3->SetPoint(i, i, ZData[i]*1000);
            };
        };

        imshow("Cam", BGR_frame);
        imshow("Customization", Output_frame);

        if (waitKey(15) >= 0)
        {
            break;
        }
    }
    return 0;
    
};