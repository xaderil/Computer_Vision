
#include <vector>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"

class Ball;

class JSON_Worker { 

public:

    static void            setCurrentDynamicData(std::vector <double> Time_Axis, 
                                                 std::vector <double> XData, 
                                                 std::vector <double> YData, 
                                                 std::vector <double> ZData, 
                                                            double  current_time);
    static void            setCurrentStaticMetroData(std::vector <double> XStaticData, std::vector <double> YStaticData, std::vector <double> ZStaticData);
    static void            generateFileWithDynamicMetroData(std::string path);
    static void            generateFileWithStaticMetroData(std::string path);

    static int             static_data_Recording_Flag;
    static int             ball_trajectory_counter;
    static int             ball_static_record_counter;    
private:
    static nlohmann::json* JSON_Static_Data;
    static nlohmann::json* JSON_Dynamic_Data;
};

