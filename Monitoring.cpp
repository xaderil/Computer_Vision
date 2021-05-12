#include "Monitoring.hpp"
int Monitoring::plot_type = 0;

Monitoring::Monitoring(int argc, char* argv[], double forecast_distance, int plot_type) {

    if (plot_type == 0) { // Flat plots

        rootapp = new TApplication("spectrum", &argc, argv);
        c1 = new TCanvas("c1", "Data");
        c1->SetWindowSize(960, 960);
        c1->SetGrid();

        mg1 = new TMultiGraph("X во времени","X from time");
        mg1->GetXaxis()->SetTitle("Time");
        mg1->GetYaxis()->SetTitle("X");
        mg1->GetXaxis()->SetLimits(-(forecast_distance), forecast_distance);

        mg2 = new TMultiGraph("Y во времени","Y from time");
        mg2->GetXaxis()->SetTitle("Time");
        mg2->GetYaxis()->SetTitle("Y");
        mg2->GetXaxis()->SetLimits(-(forecast_distance), forecast_distance);

        mg3 = new TMultiGraph("Z во времени","Z from time");
        mg3->GetXaxis()->SetTitle("Time");
        mg3->GetYaxis()->SetTitle("Z");
        mg3->GetXaxis()->SetLimits(-(forecast_distance), forecast_distance);

        f1 = new TGraph(2);
        f1->SetName("Реальная координата X");
        f1->SetLineColor(2);
        f1->SetLineWidth(2);

        f1_forecast = new TGraph(2);
        f1_forecast->SetName("Прогнозная координата X");
        f1_forecast->SetLineColor(4);
        f1_forecast->SetLineWidth(2);

        f2 = new TGraph(2);
        f2->SetName("Реальная координата Y");
        f2->SetLineColor(2);
        f2->SetLineWidth(2);

        f2_forecast = new TGraph(2);
        f2_forecast->SetName("Прогнозная координата Y");
        f2_forecast->SetLineColor(4);
        f2_forecast->SetLineWidth(2);

        f3 = new TGraph(2);
        f3->SetName("Реальная координата Z");
        f3->SetLineColor(2);
        f3->SetLineWidth(2);

        f3_forecast = new TGraph(2);
        f3_forecast->SetName("Прогнозная координата Z");
        f3_forecast->SetLineColor(4);
        f3_forecast->SetLineWidth(2);


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
    } else { // Spatial plot

        rootapp = new TApplication("spectrum", &argc, argv);
        c = new TCanvas("c", "Data");
        c->SetWindowSize(960, 960);
        g = new TGraph2D(30);
        // g->SetMinimum((-1000.0));

        // g->SetMaximum(1000.0);
        g->SetLineWidth(4);
        g->SetLineColor(4);
        g->Draw("line");

        
    };

};

void Monitoring::nullifyAllData(int current_size, int size_forecasted) {
    if (plot_type == 0) {

        for(int i=current_size; i>0 ;i--) {
            f1->RemovePoint(i);
            f2->RemovePoint(i);
            f3->RemovePoint(i);
        };

        for (int i = size_forecasted; i > 0; i--) {
            f1_forecast->RemovePoint(i);
            f2_forecast->RemovePoint(i);
            f3_forecast->RemovePoint(i);
        }
    } else {
        for (int i = current_size; i > 0; i--) {
        g->RemovePoint(i);
        };
        // g->Set(30);
    };
};

void Monitoring::setPointsToCurrentData(vector <double> XData, vector <double> YData, vector <double> ZData, vector <double> Time_Axis, chrono::duration<double, std::milli> current_time) {
        
    if (plot_type == 0) {
        
        for(int i=0; i<XData.size() ;i++) {
            f1->SetPoint(i, Time_Axis[i]-current_time.count(), XData[i]*1000);
            f2->SetPoint(i, Time_Axis[i]-current_time.count(), YData[i]*1000);
            f3->SetPoint(i, Time_Axis[i]-current_time.count(), ZData[i]*1000);
        };
    } else {

        for (int i=0; i<XData.size(); i++) {

            g->SetPoint(i,XData[i]*1000, YData[i]*1000, ZData[i]*1000);
            g->SetMinimum(-1000);
            g->SetMaximum(1000);
            g->GetXaxis()->SetLimits((-1000.0), 1000);
            g->GetYaxis()->SetLimits((-1000.0), 1000);
            g->GetZaxis()->SetLimits((-1000.0), 1000);

            
        };
    }; 
};





    
void Monitoring::setPointsToForecastedData(vector <double> XData_Forecasted, vector <double> YData_Forecasted, vector <double> ZData_Forecasted, vector <double> Time_Axis_Forecast, chrono::duration<double, std::milli> current_time) {
    for(int i=0; i<XData_Forecasted.size() ;i++) {
        f1_forecast->SetPoint(i, Time_Axis_Forecast[i]-current_time.count(), XData_Forecasted[i]*1000);
        f2_forecast->SetPoint(i, Time_Axis_Forecast[i]-current_time.count(), YData_Forecasted[i]*1000);
        f3_forecast->SetPoint(i, Time_Axis_Forecast[i]-current_time.count(), ZData_Forecasted[i]*1000);
    };
};


void Monitoring::updateGraphs(Monitoring & Monitoring) {
    if (plot_type == 0) {

        while(1) {
            Monitoring.c1->cd(1);
            Monitoring.c1->Update();
            Monitoring.c1->Pad()->Draw();
            Monitoring.c1->cd(2);
            Monitoring.c1->Update();
            Monitoring.c1->Pad()->Draw();
            Monitoring.c1->cd(3);
            Monitoring.c1->Update();
            Monitoring.c1->Pad()->Draw();
        }

    } else {
        while (1) {
            Monitoring.g->Draw("line");
            Monitoring.c->cd();
            Monitoring.c->Update();
            this_thread::sleep_for(std::chrono::milliseconds(70));
        }
    };
};