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
        this_thread::sleep_for(chrono::milliseconds(30));
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

    Seeker seeker(window_size);

    while (1)
    {
        video.read(BGR_frame);
        // cout << BGR_frame.size << endl;
        cvtColor(BGR_frame, HSV_frame, COLOR_BGR2HSV);
        inRange(HSV_frame, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), Output_frame);
        
        bool found = seeker.findObject(Out_ptr, BGR_ptr);

        vector<double> xdata = seeker.getXData();
        vector<double> ydata = seeker.getYData();
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
        float ZRealPos = ball.getZRealPos();

        if (found) {
            seeker.drawObject(BGR_ptr,xRealPos, yRealPos, ZRealPos, pxX, pxY);
        };

        // ball.calculateXRealPos()
        for(size_t i=0;i<window_size;i++) {
            f1->SetPoint(i, xdata[i], ydata[i]);
        };
        // c1->cd(1);
        // c1->Update();
        imshow("Cam", BGR_frame);
        imshow("Customization", Output_frame);
        if (waitKey(15) >= 0)
        {
            break;
        }
    }
    return 0;
    
};