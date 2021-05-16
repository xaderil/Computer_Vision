
#include <vector>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"

class JSON_Worker { 

public:

    static void            setCurrentDynamicData(std::vector <double> Time_Axis, 
                                                 std::vector <double> XData, 
                                                 std::vector <double> YData, 
                                                 std::vector <double> ZData, 
                                                              double  current_time);
    static void            generateFileWithDynamicMetroData(std::string path);
    static void            recordStaticMetroData();
    static void            generateFileWithStaticMetroData(std::string path);

    static int             static_data_Recording_Flag;
    static int           ball_trajectory_counter;
private:
    static nlohmann::json* JSON_Static_Data;
    static nlohmann::json* JSON_Dynamic_Data;
};

