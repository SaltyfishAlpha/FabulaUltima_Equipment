//
// Created by 19144 on 2025/1/20.
//

#ifndef FUEQUIP_EQUIP_H
#define FUEQUIP_EQUIP_H

#include <numeric>
#include <sstream>
#include <set>

#include "global.h"
#include "rapidjson/document.h"
#include "rapidjson/pointer.h"

namespace fue{
    static const char* kTypeNames[] =
            { "Null", "False", "True", "Object", "Array", "String", "Number" };

    int generate(const rapidjson::Value& possibility) {
        int sum = std::accumulate(possibility.Begin(), possibility.End(), 0,
                                  [](int acc, const rapidjson::Value& v){ return acc + v.GetInt(); });
        int cnt = fue::RandamEngine::getInstance().random_range(1, sum);
        for (int i = 0, j = 0; i <= sum; i += possibility[j].GetInt(), j++)
            if (cnt <= i) return j-1;
        return 0;
    }

    std::string objectToString(const rapidjson::Value& obj) {
        std::ostringstream oss;
        if (obj.IsObject()) {
            oss << "{";
            for (const auto& elem : obj.GetObject()) {
                if (elem.name.GetType()==rapidjson::Type::kNullType) oss<<"\"\": ";
                else oss << "\"" << elem.name.GetString() << "\": ";
                switch (elem.value.GetType()) {
                    case rapidjson::Type::kStringType:
                        oss << "\"" << elem.value.GetString() << "\", ";
                        break;

                    case rapidjson::Type::kNumberType:
                        oss << elem.value.GetInt() << ", ";
                        break;

                    case rapidjson::Type::kArrayType:
                    case rapidjson::Type::kObjectType:
                        oss << objectToString(elem.value) << ", ";
                        break;

                    default:
                        oss << "[" << kTypeNames[elem.value.GetType()] << "], ";
                        break;
                }
            }
            oss << "}";
            return oss.str();
        } else if (obj.IsArray()){
            oss << "[";
            for (const auto& elem : obj.GetArray()) {
                switch (elem.GetType()) {
                    case rapidjson::Type::kStringType:
                        oss << "\"" << elem.GetString() << "\", ";
                        break;

                    case rapidjson::Type::kNumberType:
                        oss << elem.GetInt() << ", ";
                        break;

                    case rapidjson::Type::kArrayType:
                    case rapidjson::Type::kObjectType:
                        oss << objectToString(elem) << ", ";
                        break;

                    default:
                        oss << "[" << kTypeNames[elem.GetType()] << "], ";
                        break;
                }
            }
            oss << "]";
            return oss.str();
        } else {
            LOG_D("Output Error");
            return "";
        }
    }

    std::pair<int, std::set<int>> knapsackWithOptimizedHashing(const rapidjson::Value& items, int min, int max){
        if (!items.IsArray()) {
            LOG_E("TypeErr");
            return {0, {}}; // defult
        }

        typedef std::set<int> CombinationSet;
        typedef std::unordered_map<int, std::set<CombinationSet>> StateTable;

        // Hash DP
        StateTable dp;
        dp[0].insert({{}});

        // state transform
        for (int index = 0; index < items.GetArray().Size(); index++){
            StateTable newDp = dp;
            for (const auto& [currentSum, combinations] : dp) {
                int newSum = currentSum + items[index].GetInt();
                if (newSum <= max) {
                    for (const auto& combination : combinations) {
                        CombinationSet newCombination = combination;
                        newCombination.insert(index);
                        newDp[newSum].insert(newCombination);
                    }
                }
            }
            dp = newDp;
        }

//        // debug
//        int cnt = 0;
//        for (const auto& [sum, combinations] : dp) {
//            std::ostringstream oss;
//            oss << "" << sum << ": {";
//            for (const auto& combination: combinations) {
//                oss << "{";
//                for (int item: combination)
//                    oss << item << ",";
//                oss << "},";
//            }
//            oss << "}";
//            LOG_D("{0}", oss.str());
//            cnt += combinations.size();
//        }
//        LOG_D("total size: {0}", cnt);

        // all valid solutions
        std::vector<std::pair<int, CombinationSet>> validSolutions;
        for (const auto& [sum, combinations] : dp) {
            if (sum >= min && sum <= max) {
                for (const auto& combination : combinations) {
                    validSolutions.push_back({sum, combination});
                }
            }
        }

        if (validSolutions.empty()) {
            LOG_D("No solution.");
            return {0, {}};
        }

//        //debug
//        for (const auto& [sum, combination] : validSolutions) {
//            std::ostringstream oss;
//            oss<<""<<sum<<": {";
//            for (int item : combination)
//                oss<<item<<",";
//            oss<<"}";
//            LOG_D("{0}", oss.str());
//        }
//        LOG_D("solution size: {0}", validSolutions.size());

        int randIndex = fue::RandamEngine::getInstance().random_range(0, validSolutions.size()-1);
        return validSolutions[randIndex];
    }

    rapidjson::Value EquipGenerateWithCertainType(int rarity_index, int equipType, const rapidjson::Document& config,
                                   rapidjson::Document::AllocatorType& allocator) {
        // output
        rapidjson::Value equip(rapidjson::kObjectType);

        // step.1 Generate Rarity
        std::string rarity;
        int maxvalue, minvalue;
        if (const rapidjson::Value* _rarity = rapidjson::Pointer("/rarity").Get(config)){
            LOG_D("rarity config.----------------");
            // get attribute
            rarity = _rarity->GetObject()["array"][rarity_index].GetString();
            minvalue = _rarity->GetObject()["value"][rarity_index][0].GetInt();
            maxvalue = _rarity->GetObject()["value"][rarity_index][1].GetInt() == -1 ?
                       std::numeric_limits<int>::max() : _rarity->GetObject()["value"][rarity_index][1].GetInt();

            LOG_D("{0}, {1}, ${2}~{3}", rarity_index, rarity, minvalue, maxvalue);

            // write
            equip.AddMember("rarity", rapidjson::Value(rarity.c_str(), allocator).Move(), allocator);
        } else
            LOG_E("config \"rarity\" lost");

        // step.2 Generate basic equipment
        std::string _type;
        if (const rapidjson::Value* __type = rapidjson::Pointer("/type").Get(config)){
            LOG_D("type config.-----------------");

            // 1. generate equipment type
            std::string type = __type->GetArray()[equipType]["type"].GetString();
            _type = __type->GetArray()[equipType]["_type"].GetString();

            LOG_D("{0}, {1}, {2}", equipType, type, _type);

            int tp1 = fue::RandamEngine::getInstance().random_range(0, __type->GetArray()[equipType]["array"].Size()-1);

            rapidjson::Value basic_equipment(rapidjson::kObjectType);
            basic_equipment.CopyFrom(__type->GetArray()[equipType]["array"][tp1], allocator);

            LOG_D("{0}", fue::objectToString(basic_equipment));

            // write
            equip.AddMember("type", rapidjson::Value(type.c_str(), allocator).Move(), allocator);
            equip.AddMember("_type", rapidjson::Value(_type.c_str(), allocator).Move(), allocator);
            for (auto& item : basic_equipment.GetObject()){
                equip.AddMember(item.name, item.value, allocator);
            }
        } else
            LOG_E("config \"type\" lost");

        // step.3 Generate affix
        if (const rapidjson::Value* affix =
                rapidjson::Pointer(("/affix/"+rarity+"/"+_type).c_str()).Get(config)){
            // get combination
            auto [result, resultItems] = fue::knapsackWithOptimizedHashing(affix->GetObject()["value"].GetArray(), minvalue, maxvalue);

            // TODO: pick affixes
            rapidjson::Value affixes(rapidjson::kArrayType);
            for (int index : resultItems) {
                int tp = fue::RandamEngine::getInstance().random_range(0, affix->GetObject()["affix"][index].Size()-1);
                affixes.PushBack(rapidjson::StringRef(affix->GetObject()["affix"][index][tp].GetString()), allocator);
                LOG_D("{0}: {1}, ${2}", tp, affix->GetObject()["affix"][index][tp].GetString(), affix->GetObject()["value"][index].GetInt());
            }
            equip.AddMember("affix", affixes, allocator);
            equip.AddMember("affixValue", result, allocator);
        }

        // additional rules TODO: make the logic editable
        // if physical weapon, damage + 2*(rarity-1)
        if (equip.HasMember("_type") && equip["_type"].IsString() && std::string(equip["_type"].GetString()) == "physical") {
            LOG_D("Physical weapon, damage = {0} + {1}", equip["damage"].GetInt(), 2*rarity_index);
            equip.AddMember("damageBonus", 2*rarity_index, allocator);
        }

        return equip;
    }

    rapidjson::Value EquipGenerate(int generate_coefficient, const rapidjson::Document& config,
                                   rapidjson::Document::AllocatorType& allocator) {

        // step.1 Generate Rarity
        int rarity_index = 0;
        if (const rapidjson::Value* _rarity = rapidjson::Pointer("/rarity").Get(config)){
            rarity_index = fue::generate(_rarity->GetObject()["possibility"][generate_coefficient].GetArray());
        }

        // step.2 Generate basic equipment
        int equipType = 0;
        if (const rapidjson::Value* __type = rapidjson::Pointer("/type").Get(config)){
            equipType = fue::RandamEngine::getInstance().random_range(0, __type->GetArray().Size()-1);
        }

        return EquipGenerateWithCertainType(rarity_index, equipType, config, allocator);
    }

    rapidjson::Value EquipGenerateWithCertainType(std::string rarity, std::string type, const rapidjson::Document& config,
                                   rapidjson::Document::AllocatorType& allocator) {

        // step.1 Generate Rarity
        int rarity_index = 0;
        if (const rapidjson::Value* _rarity = rapidjson::Pointer("/rarity").Get(config)){
            for (int i = 0; i < _rarity->GetObject()["array"].GetArray().Size(); i++){
                if (_rarity->GetObject()["array"][i].GetString() == rarity) {
                    rarity_index = i;
                    break;
                }
            }
        }

        // step.2 Generate basic equipment
        int equipType = 0;
        if (const rapidjson::Value* __type = rapidjson::Pointer("/type").Get(config)){
            for (int i = 0; i < __type->GetArray().Size(); i++){
                if (__type->GetArray()[i]["type"].GetString() == type) {
                    equipType = i;
                    break;
                }
            }
        }

        return EquipGenerateWithCertainType(rarity_index, equipType, config, allocator);
    }

    std::string equipmentToString(const rapidjson::Value& equip){
        if (!equip.IsObject()) {
            LOG_E("Type Err");
            return "";
        }
        std::ostringstream oss;
        oss<<"生成装备：";
        oss<<"\n【"<<equip["name"].GetString()<<"】";
        oss<<"\n品质："<<equip["rarity"].GetString();
        oss<<"\n类型："<<equip["type"].GetString();
        oss<<"\n价格："<<equip["value"].GetInt()
            + (equip.HasMember("affixValue") ? equip["affixValue"].GetInt() : 0);
        if (equip.HasMember("hitBonus"))
            oss<<"\n命中加值："<<equip["hitBonus"].GetInt();
        if (equip.HasMember("damage"))
            oss<<"\n伤害：HR+"<<equip["damage"].GetInt()
                             + (equip.HasMember("damageBonus") ? equip["damageBonus"].GetInt() : 0);
        if (equip.HasMember("damageType"))
            oss<<"\n伤害类型："<<equip["damageType"].GetString();
        if (equip.HasMember("phyDef"))
            oss<<"\n物防："<<equip["phyDef"].GetString();
        if (equip.HasMember("mgcDef"))
            oss<<"\n魔防："<<equip["mgcDef"].GetString();
        if (equip.HasMember("initiative"))
            oss<<"\n先攻："<<equip["initiative"].GetInt();

        if (equip.HasMember("affix")) {
            oss << "\n额外词条：";
            for (const auto &item: equip["affix"].GetArray()) {
                oss << "\n· " << item.GetString();
            }
        }
        return oss.str();
    }
}

#endif //FUEQUIP_EQUIP_H
