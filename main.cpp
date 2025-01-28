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
        std::cout << "�Ƿ�ָ��ϡ�жȺ��������ͣ�[y/n](�˳�����q) ";
        std::cin >> str;
        if (str == "y") {
            int a, b;
            std::cout << "������ϡ�ж�(���) [";
            for (int i = 0; i < jsonHandler.getDoc()["rarity"]["array"].Size(); i++) {
                std::cout << i + 1 << "." << jsonHandler.getDoc()["rarity"]["array"][i].GetString() << " ";
            }
            std::cout << "] ";
            std::cin >> a;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "��������볢����������" << std::endl;
                continue;
            }

            std::cout << "��������������(���) [";
            for (int i = 0; i < jsonHandler.getDoc()["type"].Size(); i++) {
                std::cout << i + 1 << "." << jsonHandler.getDoc()["type"][i]["type"].GetString() << " ";
            }
            std::cout << "] ";
            std::cin >> b;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "��������볢����������" << std::endl;
                continue;
            }

            equip = fue::EquipGenerateWithCertainType(a - 1, b - 1, jsonHandler.getDoc(),
                                                      jsonHandler.getDoc().GetAllocator());

            // output equipment
            LOG_T("Equipment: {0}", fue::objectToString(equip));

            std::cout << fue::equipmentToString(equip) << std::endl;
        } else if (str == "n") {
            int a;
            std::cout << "ѡ�����ɱ���(���) [\n";
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
                std::cout << "��������볢����������" << std::endl;
                continue;
            }

            equip = fue::EquipGenerate(a - 1, jsonHandler.getDoc(), jsonHandler.getDoc().GetAllocator());

            // output equipment
            LOG_T("Equipment: {0}", fue::objectToString(equip));

            std::cout << fue::equipmentToString(equip) << std::endl;
        } else if (str == "q"){
            break;
        }else {
            std::cout << "��������볢����������" << std::endl;
        }
        std::cout << "---------------------------" << std::endl;
    }


    system("pause");

    return EXIT_SUCCESS;
}