//
// Created by 19144 on 2025/1/20.
//

#ifndef FUEQUIP_JSONHANDLER_H
#define FUEQUIP_JSONHANDLER_H

#include "global.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"

#include <fstream>

namespace fue{

    class JSONHandler {
    public:
        JSONHandler(const std::string& filePath): filePath(filePath) {}

        bool readJson() {
            std::ifstream inputFile(filePath);
            if (!inputFile.is_open()) {
                LOG_E("Failed to open file for reading: {0}", filePath);
                return false;
            }

            rapidjson::IStreamWrapper isw(inputFile);
            rapidjson::AutoUTFInputStream<unsigned, rapidjson::IStreamWrapper> autoStream(isw);
            jsonData.ParseStream(autoStream);

            if (jsonData.HasParseError()) {
                LOG_E("Error parsing JSON from file");
                return false;
            }

            inputFile.close();
            return true;
        }

        bool writeJson() {
            std::ofstream outputFile(filePath);
            if (!outputFile.is_open()) {
                LOG_E("Failed to open file for writing: {0}", filePath);
                return false;
            }

            rapidjson::OStreamWrapper osw(outputFile);
            typedef rapidjson::AutoUTFOutputStream<unsigned, rapidjson::OStreamWrapper> OutputStream;
            OutputStream autoStream(osw, rapidjson::kUTF8, false);
            rapidjson::PrettyWriter<OutputStream> writer(autoStream);
            jsonData.Accept(writer);

            outputFile.close();
            return true;
        }

        rapidjson::Document& getDoc(){
            return jsonData;
        }

    private:
        std::string filePath;
        rapidjson::Document jsonData;
    };
}

#endif //FUEQUIP_JSONHANDLER_H
