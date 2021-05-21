#include "JSON_Worker.hpp" 

nlohmann::json* JSON_Worker::JSON_Static_Data             = new nlohmann::json;
nlohmann::json* JSON_Worker::JSON_Dynamic_Data            = new nlohmann::json;
nlohmann::json* JSON_Worker::JSON_HSV_Data                = new nlohmann::json;
int             JSON_Worker::static_data_Recording_Flag   = 0;
int             JSON_Worker::ball_trajectory_counter      = 1;
int             JSON_Worker::ball_static_record_counter   = 1;

void JSON_Worker::setCurrentDynamicData(std::vector <double> Time_Axis, 
                                        std::vector <double> XData, 
                                        std::vector <double> YData, 
                                        std::vector <double> ZData, 
                                                     double  current_time) {
    
    (*JSON_Dynamic_Data)[std::to_string(ball_trajectory_counter)]["Time_Axis"] = Time_Axis;
    (*JSON_Dynamic_Data)[std::to_string(ball_trajectory_counter)]["XData"] = XData;
    (*JSON_Dynamic_Data)[std::to_string(ball_trajectory_counter)]["YData"] = YData;
    (*JSON_Dynamic_Data)[std::to_string(ball_trajectory_counter)]["ZData"] = ZData;
    (*JSON_Dynamic_Data)[std::to_string(ball_trajectory_counter)]["Current_Time"] = current_time;
};

void JSON_Worker::generateFileWithDynamicMetroData(std::string path) {

    std::ofstream file(path);
    file << *(JSON_Dynamic_Data);

};

void JSON_Worker::setCurrentStaticMetroData(std::vector <double> XStaticData, std::vector <double> YStaticData, std::vector <double> ZStaticData) {
    (*JSON_Static_Data)[std::to_string(ball_static_record_counter)]["StaticXData"] = XStaticData;
    (*JSON_Static_Data)[std::to_string(ball_static_record_counter)]["StaticYData"] = YStaticData;
    (*JSON_Static_Data)[std::to_string(ball_static_record_counter)]["StaticZData"] = ZStaticData;
    
};

void JSON_Worker::generateFileWithStaticMetroData(std::string path) {

    std::ofstream file(path);
    file << *(JSON_Static_Data);

};

void JSON_Worker::rewriteHSVData(int num, void* data) {
	

    (*JSON_HSV_Data)["H_MIN"] = Ball::H_MIN;
    (*JSON_HSV_Data)["H_MAX"] = Ball::H_MAX;
    (*JSON_HSV_Data)["S_MIN"] = Ball::S_MIN;
    (*JSON_HSV_Data)["S_MAX"] = Ball::S_MAX;
    (*JSON_HSV_Data)["V_MIN"] = Ball::V_MIN;
    (*JSON_HSV_Data)["V_MAX"] = Ball::V_MAX;
    (*JSON_HSV_Data)["F"] = Ball::f;
    writeHSVFile();

};

void JSON_Worker::writeHSVFile() {
    std::ofstream file("HSV_Color_Data.json");
    file << *(JSON_HSV_Data);
}

void JSON_Worker::readHSV() {

    std::ifstream HSV_Data("HSV_Color_Data.json");
    (*JSON_HSV_Data) << HSV_Data;
    Ball::H_MIN = (*JSON_HSV_Data)["H_MIN"];
    Ball::H_MAX = (*JSON_HSV_Data)["H_MAX"];
    Ball::S_MIN = (*JSON_HSV_Data)["S_MIN"];
    Ball::S_MAX = (*JSON_HSV_Data)["S_MAX"];
    Ball::V_MIN = (*JSON_HSV_Data)["V_MIN"];
    Ball::V_MAX = (*JSON_HSV_Data)["V_MAX"];
    Ball::f = (*JSON_HSV_Data)["F"];

};

