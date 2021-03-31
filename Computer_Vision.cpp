#define FPS_SMOOTHING 0.9
// OpenCV Libraries
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
// #include <sciplot/sciplot.hpp>
// Other Libraries
#include <iostream>
#include <stdint.h>
#include <chrono>
#include <thread>
// #include "armadillo"
// ROOT Libraries
#include "TCanvas.h"
#include "TGraph.h"
#include "TApplication.h"
#include "TAxis.h"
#include "TSystem.h"
// Project Headers
#include "Ball.hpp"
#include "Seeker.hpp"
using namespace std;
using namespace cv;
using namespace std::chrono;


constexpr int window_size = 50;
int NUM_OF_CAMERA = 0;
int FRAME_WIDTH = 640;
int FRAME_HEIGHT = 480;
bool CUSTOMIZATION = true;
string COLOR = "green";

int MODE = 0;
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

void Change_Mode(int state, void*) {
    MODE = state;
};
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
};

void Create_Customization() {
    namedWindow("Customization", WINDOW_NORMAL);
    createTrackbar("H_MIN", "Customization", &H_MIN, 256, 0);
    createTrackbar("H_MAX", "Customization", &H_MAX, 256, 0);
    createTrackbar("S_MIN", "Customization", &S_MIN, 256, 0);
    createTrackbar("S_MAX", "Customization", &S_MAX, 256, 0);
    createTrackbar("V_MIN", "Customization", &V_MIN, 256, 0);
    createTrackbar("V_MAX", "Customization", &V_MAX, 256, 0);
    createTrackbar("MODE", "Customization", 0, 1, Change_Mode);
};
int main(int argc, char* argv[])
{
    // ROOT PLOTS INIT
    TApplication rootapp("spectrum", &argc, argv);
    auto c1 = make_unique<TCanvas>("c1", "Data");
    c1->SetWindowSize(960, 960);
    auto f1 = make_unique<TGraph>(window_size);
    f1->SetTitle("Yee");
    f1->GetXaxis()->SetTitle("Num");
    f1->GetYaxis()->SetTitle("Area");
    f1->SetMinimum(0);
    f1->SetMaximum(40000);

    auto f2 = make_unique<TGraph>(window_size);
    f2->SetTitle("Waveform");
    f2->GetXaxis()->SetTitle("Num");
    f2->GetYaxis()->SetTitle("X");
    f2->SetMinimum(-1);
    f2->SetMaximum(1);

    c1->Divide(1, 2);
    c1->cd(1);
    f1->Draw();
    c1->cd(2);
    f2->Draw();
    Ball ball(COLOR);
    //Ball* ptr = $ball;
    H_MIN = ball.getHSVmin()[0];
    H_MAX = ball.getHSVmax()[0];
    S_MIN = ball.getHSVmin()[1];
    S_MAX = ball.getHSVmax()[1];
    V_MIN = ball.getHSVmin()[2];
    V_MAX = ball.getHSVmax()[2];

    VideoCapture video(0);
    if (CUSTOMIZATION)
    {
        Create_Customization();
    }
    Seeker seeker(window_size);
    //Seeker yee(link);
    while (1)
    {
        if (MODE == 0)
        // BGR отображение на выходе
        {   
            while (MODE == 0)
            {
                video.read(BGR_frame);
                cvtColor(BGR_frame, HSV_frame, COLOR_BGR2HSV);
                inRange(HSV_frame, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), Output_frame);
                seeker.findObject(Out_ptr, BGR_ptr);
                vector<double> xdata = seeker.getXData();
                vector<double> ydata = seeker.getYData();
                for(size_t i=0;i<window_size;i++) {
                    f1->SetPoint(i, xdata[i], ydata[i]);
                };
                c1->cd(1);
		        c1->Update();
		        c1->Pad()->Draw();
                imshow("Cam", BGR_frame);
                if (waitKey(15) >= 0)
                {
                    break;
                }
            }
        }
        if (MODE == 1)
        // HSV отображение на выходе
        {
            while (MODE == 1)
            {
                video.read(BGR_frame);
                cvtColor(BGR_frame, HSV_frame, COLOR_BGR2HSV);
                inRange(HSV_frame, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), Output_frame);
                seeker.findObject(Out_ptr, BGR_ptr);
                imshow("Cam", Output_frame);
                if (waitKey(15) >= 0)
                {
                    break;
                }
            }
        }
    }
    return 0;
    
};