//
// Created by 19144 on 2025/1/25.
//
#include "global.h"
#include "equipment/JSONHandler.h"
#include "equipment/Equipment.h"
#include "rapidjson/pointer.h"

int main(){
    // global initial
    fue::log::Init();
    fue::JSONHandler jsonHandler = fue::JSONHandler("D:\\CLionProjects\\FabulaUltima_Equipment\\test\\test3\\config.json");
    jsonHandler.readJson();

    // config initial
    if (!jsonHandler.getDoc().IsObject()) {
        LOG_E("config err");
        return EXIT_FAILURE;
    }

    // input
    int generate_coefficient = 0;
    // output
    rapidjson::Value equip(rapidjson::kObjectType);

    // step.1 Generate Rarity
    int rarity_index;
    std::string rarity;
    int maxvalue, minvalue;
    if (const rapidjson::Value* _rarity = rapidjson::Pointer("/rarity").Get(jsonHandler.getDoc())){
        LOG_D("rarity config.----------------");

        // get attribute
        rarity_index = fue::generate(_rarity->GetObject()["possibility"][generate_coefficient].GetArray());
        rarity = _rarity->GetObject()["array"][rarity_index].GetString();
        minvalue = _rarity->GetObject()["value"][rarity_index][0].GetInt();
        maxvalue = _rarity->GetObject()["value"][rarity_index][1].GetInt() == -1 ?
                std::numeric_limits<int>::max() : _rarity->GetObject()["value"][rarity_index][1].GetInt();

        LOG_D("{0}, {1}, ${2}~{3}", rarity_index, rarity, minvalue, maxvalue);

        // write
        equip.AddMember("rarity", rapidjson::StringRef(rarity.c_str()), jsonHandler.getDoc().GetAllocator());
    } else
        LOG_E("config \"rarity\" lost");

    // step.2 Generate basic equipment
    std::string type, _type;
    if (const rapidjson::Value* __type = rapidjson::Pointer("/type").Get(jsonHandler.getDoc())){
        LOG_D("type config.-----------------");

        // 1. generate equipment type
        int tp = fue::RandamEngine::getInstance().random_range(0, __type->GetArray().Size()-1);
        type = __type->GetArray()[tp]["type"].GetString();
        _type = __type->GetArray()[tp]["_type"].GetString();

        LOG_D("{0}, {1}, {2}", tp, type, _type);

        int tp1 = fue::RandamEngine::getInstance().random_range(0, __type->GetArray()[tp]["array"].Size()-1);

        rapidjson::Value basic_equipment(rapidjson::kObjectType);
        basic_equipment.CopyFrom(__type->GetArray()[tp]["array"][tp1], jsonHandler.getDoc().GetAllocator());

        LOG_D("{0}", fue::objectToString(basic_equipment));
//        LOG_D("{0}", fue::objectToString(__type->GetArray()[tp]["array"][tp1].GetObject()));

        // write
        equip.AddMember("type", rapidjson::StringRef(type.c_str()), jsonHandler.getDoc().GetAllocator());
        equip.AddMember("_type", rapidjson::StringRef(_type.c_str()), jsonHandler.getDoc().GetAllocator());
        for (auto& item : basic_equipment.GetObject()){
            equip.AddMember(item.name, item.value, jsonHandler.getDoc().GetAllocator());
        }
//        LOG_D("{0}", fue::objectToString(basic_equipment));
    } else
        LOG_E("config \"type\" lost");

    // step.3 Generate affix
    if (const rapidjson::Value* affix =
            rapidjson::Pointer(("/affix/"+rarity+"/"+_type).c_str()).Get(jsonHandler.getDoc())){
//        LOG_D("{0}", fue::objectToString(*affix));

        // get combination
        auto [result, resultItems] = fue::knapsackWithOptimizedHashing(affix->GetObject()["value"].GetArray(), minvalue, maxvalue);

        //debug
//        std::ostringstream oss;
//        oss<<"{";
//        for (const auto& item : affix->GetObject()["value"].GetArray())
//            oss<<item.GetInt()<<",";
//        oss<<"}";
//        LOG_D("{0}", oss.str());
//        oss.str(""); oss.clear();
//        oss<<"chosen result: "<<result<<": {";
//        for (int item : resultItems)
//            oss<<item<<",";
//        oss<<"}";
//        LOG_D("{0}", oss.str());

        // TODO: pick affixes
        rapidjson::Value affixes(rapidjson::kArrayType);
        for (int index : resultItems) {
            int tp = fue::RandamEngine::getInstance().random_range(0, affix->GetObject()["affix"][index].Size()-1);
            affixes.PushBack(rapidjson::StringRef(affix->GetObject()["affix"][index][tp].GetString()), jsonHandler.getDoc().GetAllocator());
            LOG_D("{0}: {1}, ${2}", tp, affix->GetObject()["affix"][index][tp].GetString(), affix->GetObject()["value"][index].GetInt());
        }
        equip.AddMember("affix", affixes, jsonHandler.getDoc().GetAllocator());
        equip.AddMember("affixValue", result, jsonHandler.getDoc().GetAllocator());
    }

    // additional rules TODO: make the logic editable
    // if physical weapon, damage + 2*(rarity-1)
    if (equip.HasMember("_type") && equip["_type"].IsString() && std::string(equip["_type"].GetString()) == "physical") {
        LOG_D("Physical weapon, damage = {0} + {1}", equip["damage"].GetInt(), 2*rarity_index);
        equip.AddMember("damageBonus", 2*rarity_index, jsonHandler.getDoc().GetAllocator());
    }

    // output equipment
    LOG_I("Equipment: {0}", fue::objectToString(equip));

//    jsonHandler.writeJson();
    return EXIT_SUCCESS;
}