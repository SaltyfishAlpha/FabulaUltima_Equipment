//
// Created by 19144 on 2025/1/20.
//

#ifndef FUEQUIP_EQUIP_H
#define FUEQUIP_EQUIP_H

#include <numeric>

#include "global.h"
#include "rapidjson/document.h"

namespace fue{
    struct Affix{
        std::string name;
        int count;
        bool isvalid;
        rapidjson::Value array, possibility, value, children;

        Affix(const rapidjson::Value& affix, rapidjson::Document::AllocatorType& allocator) : array(rapidjson::kArrayType), possibility(rapidjson::kArrayType),
                                              value(rapidjson::kArrayType), children(rapidjson::kArrayType){
            if (!affix.IsObject()) {
                LOG_E("TYPE ERROR");
                return;
            }
            if (affix.HasMember("name") && affix["name"].IsString()) this->name = affix["name"].GetString();
            if (affix.HasMember("count") && affix["count"].IsInt()) this->count = affix["count"].GetInt();
            if (affix.HasMember("isvalid") && affix["isvalid"].IsBool()) this->isvalid = affix["isvalid"].IsBool();
            if (affix.HasMember("array") && affix["array"].IsArray()) this->array.CopyFrom(affix["array"], allocator);
            if (affix.HasMember("possibility") && affix["possibility"].IsArray()) this->possibility.CopyFrom(affix["possibility"], allocator);
            if (affix.HasMember("value") && affix["value"].IsArray()) this->value.CopyFrom(affix["value"], allocator);
            if (affix.HasMember("children") && affix["children"].IsArray()) this->children.CopyFrom(affix["children"], allocator);
        }

        bool checkFinish() {
            return !array.IsNull() & !possibility.IsNull() & !value.IsNull() & !children.IsNull();
        }

        bool checkLegal() {
            return count != 0 && array.Size() == count & possibility.Size() == count & value.Size() == count;
        }

        int generate() {
            if (!checkLegal()) {
                LOG_E("Illegal Affix!");
                return -1;
            }
            rapidjson::Value result(array[0].GetType());
            int sum = std::accumulate(possibility.Begin(), possibility.End(), 0,
                                      [](int acc, rapidjson::Value& v){ return acc + v.GetInt(); });
            int cnt = fue::RandamEngine::getInstance().random_range(1, sum);
            for (int i = 0, j = 0; i <= sum; i += possibility[j].GetInt(), j++)
                if (cnt <= i) return j-1;
            return 0;
        }


        void log(){
            LOG_D("--------------------------------");
            LOG_D("name:{0}, count:{1}, isvalid:{2}", name.c_str(), count, isvalid);
            for(auto& child : array.GetArray())
                LOG_D("array: {0}", child.GetString());
            for(auto& child : possibility.GetArray())
                LOG_D("possibility: {0}", child.GetInt());
            for(auto& child : value.GetArray())
                LOG_D("value: {0}", child.GetInt());
            for(auto& child : children.GetArray())
                LOG_D("children: {0}", child.GetInt());
            LOG_D("children is {0}", children.Empty() ? "empty" : "not empty");
            LOG_D("--------------------------------");
        }
    };

}

#endif //FUEQUIP_EQUIP_H
