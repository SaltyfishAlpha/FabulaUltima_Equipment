//
// Created by 19144 on 2025/1/24.
//
#include "equipment/JSONHandler.h"
#include "equipment/Equipment.h"

int main(){
    // Init
    fue::log::Init();
    fue::JSONHandler jsonHandler = fue::JSONHandler("D:\\CLionProjects\\FabulaUltima_Equipment\\test\\test2\\config.json");
    jsonHandler.readJson();
    LOG_D("{0}", jsonHandler.getDoc()[0].IsObject());

    for (auto& affixs : jsonHandler.getDoc().GetArray()){
        fue::Affix affix(affixs, jsonHandler.getDoc().GetAllocator());
        LOG_D("{0}.checkFinish(): {1}", affix.name, affix.checkFinish());
        LOG_D("{0}.checkLegal(): {1}", affix.name, affix.checkLegal());
        if (affix.checkFinish() && affix.checkLegal())
            affix.log();

        if (affix.checkLegal()){
            int* cnt = new int[affix.count + 1];
            std::memset(cnt, 0, sizeof(int) * (affix.count + 1));
            for (int i = 0; i < 10000; i++){
                int result = affix.generate();
                if (result == -1) break;
                LOG_D("Generate: {0}: {1}, +${2}", affix.name, affix.array[result].GetString(), affix.value[result].GetInt());
                cnt[result]++;
            }
            LOG_D("----------------------");
            for (int i = 0; i < affix.count; i++){
                LOG_D("{0}: {1}", affix.array[i].GetString(), cnt[i]);
            }
            LOG_D("----------------------");
        }
    }

//    rapidjson::Value arr(rapidjson::kArrayType);

    return EXIT_SUCCESS;
}