
#include <vector>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"

class JSON_Worker {

public:
    JSON_Worker() : JSON_Data(new nlohmann::json) {};

    void setData(std::vector <double> Time_Axis, std::vector <double> XData, std::vector <double> YData, std::vector <double> ZData, double current_time) 
    {
        (*JSON_Data)["Time_Axis"] = Time_Axis;
        (*JSON_Data)["XData"] = XData;
        (*JSON_Data)["YData"] = YData;
        (*JSON_Data)["ZData"] = ZData;
        (*JSON_Data)["Current_Time"] = current_time;
    };

    void generateFile(std::string path) {
        std::ofstream file(path);
        file << *(JSON_Data);
    };

    nlohmann::json* JSON_Data;
};