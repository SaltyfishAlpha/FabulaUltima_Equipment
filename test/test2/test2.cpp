//
// Created by 19144 on 2025/1/24.
//
#include <limits>
#include "equipment/JSONHandler.h"
#include "equipment/Equipment.h"

int main(){
    // Init
    fue::log::Init();
    fue::JSONHandler jsonHandler = fue::JSONHandler("D:\\CLionProjects\\FabulaUltima_Equipment\\test\\test2\\config.json");
    jsonHandler.readJson();

    // input
    int generate_coefficient = 6;

    for (auto& obj : jsonHandler.getDoc().GetArray()) {
        if (!obj.IsObject()) continue;
        if (obj.HasMember("name") && obj["name"].IsString()){
            std::string str = obj["name"].GetString();
            if (str == "rarity") {
                LOG_D("rarity config.");
                rapidjson::Value possibility(obj["possibility"][generate_coefficient].GetArray());

                // generate rarity
                int* cnt = new int[possibility.Size()];
                std::memset(cnt, 0, sizeof(int) * possibility.Size());
                for (int i = 0; i < 10000; i++){
                    int result = fue::generate(possibility);
                    if (result == -1) break;
                    int value;
                    if (result == 0) value = 0;// white
                    else
                        value = fue::RandamEngine::getInstance().random_range(obj["value"][result][0].GetInt(),
                            obj["value"][result][1].GetInt()==-1 ? std::numeric_limits<int>::max() : obj["value"][result][1].GetInt()
                            );
                    LOG_D("Generate: {0}, ${1}", obj["array"][result].GetString(), value);
                    cnt[result]++;
                }
                LOG_D("----------------------");
                for (int i = 0; i < possibility.Size(); i++){
                    LOG_D("{0}: {1}", obj["array"][i].GetString(), cnt[i]);
                }
                LOG_D("----------------------");

                break;
            }
        }
    }


//    rapidjson::Value arr(rapidjson::kArrayType);

    return EXIT_SUCCESS;
}