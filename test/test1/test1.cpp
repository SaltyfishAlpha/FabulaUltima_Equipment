//
// Created by 19144 on 2025/1/20.
//
#include "iostream"

#include "equipment/JSONHandler.h"

int main(){
    fue::log::Init();

    LOG_D("Hello world\n");
    fue::JSONHandler handler = fue::JSONHandler("D:\\CLionProjects\\FabulaUltima_Equipment\\test\\test1\\a.json");
    handler.readJson();
    LOG_D("Success");

    LOG_D("{0} : {1} : {2}", "project", handler.getDoc().HasMember("project"), handler.getDoc()["project"].GetString());

    for (auto itr = handler.getDoc().MemberBegin(); itr != handler.getDoc().MemberEnd(); ++itr){
        LOG_D("Type of member {0} is {1}", itr->name.GetString(), itr->value.GetType());
    }

    std::string name, value;
    std::cout<<"Input: ";
    std::cin>>name>>value;

    handler.getDoc().AddMember(rapidjson::StringRef(name.c_str()),
                  rapidjson::StringRef(value.c_str()), handler.getDoc().GetAllocator());

    for (auto itr = handler.getDoc().MemberBegin(); itr != handler.getDoc().MemberEnd(); ++itr){
        LOG_D("Type of member {0} is {1}", itr->name.GetString(), itr->value.GetType());
    }

    handler.writeJson();

    return EXIT_SUCCESS;
}