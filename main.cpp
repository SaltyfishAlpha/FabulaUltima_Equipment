#include <iostream>
//#include "editor/FUwindow.h"
#include "equipment/Equipment.h"
#include "equipment/JSONHandler.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
    //global initial
    fue::log::Init();
//    std::unique_ptr<fue::FUwindow> window = fue::FUwindow::Create(SCR_WIDTH, SCR_HEIGHT, "SandBox");

    fue::JSONHandler jsonHandler = fue::JSONHandler("assets/config.json");
    jsonHandler.readJson();

//    while (!window->ibWindowShouldClose()) {
//        window->ibPollEvents();
//        window->ibSwapBuffer();
//    }

    while(1) {

        rapidjson::Value equip(rapidjson::kObjectType);

        std::string str;
        std::cout << "是否指定稀有度和武器类型？[y/n](退出程序：q) ";
        std::cin >> str;
        if (str == "y") {
            int a, b;
            std::cout << "请输入稀有度(序号) [";
            for (int i = 0; i < jsonHandler.getDoc()["rarity"]["array"].Size(); i++) {
                std::cout << i + 1 << "." << jsonHandler.getDoc()["rarity"]["array"][i].GetString() << " ";
            }
            std::cout << "] ";
            std::cin >> a;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "输入错误，请尝试重新输入" << std::endl;
                continue;
            }

            std::cout << "请输入武器类型(序号) [";
            for (int i = 0; i < jsonHandler.getDoc()["type"].Size(); i++) {
                std::cout << i + 1 << "." << jsonHandler.getDoc()["type"][i]["type"].GetString() << " ";
            }
            std::cout << "] ";
            std::cin >> b;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "输入错误，请尝试重新输入" << std::endl;
                continue;
            }

            equip = fue::EquipGenerateWithCertainType(a - 1, b - 1, jsonHandler.getDoc(),
                                                      jsonHandler.getDoc().GetAllocator());

            // output equipment
            LOG_T("Equipment: {0}", fue::objectToString(equip));

            std::cout << fue::equipmentToString(equip) << std::endl;
        } else if (str == "n") {
            int a;
            std::cout << "选择生成比例(序号) [\n";
            for (int i = 0; i < jsonHandler.getDoc()["rarity"]["possibility"].Size(); i++) {
                std::cout << i + 1 << ". [";
                for (const auto &item: jsonHandler.getDoc()["rarity"]["possibility"][i].GetArray())
                    std::cout << item.GetInt() << ", ";
                std::cout << "]\n";
            }
            std::cout << "] ";
            std::cin >> a;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "输入错误，请尝试重新输入" << std::endl;
                continue;
            }

            equip = fue::EquipGenerate(a - 1, jsonHandler.getDoc(), jsonHandler.getDoc().GetAllocator());

            // output equipment
            LOG_T("Equipment: {0}", fue::objectToString(equip));

            std::cout << fue::equipmentToString(equip) << std::endl;
        } else if (str == "q"){
            break;
        }else {
            std::cout << "输入错误，请尝试重新输入" << std::endl;
        }
        std::cout << "---------------------------" << std::endl;
    }


    system("pause");

    return EXIT_SUCCESS;
}