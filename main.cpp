#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "./headers/nlohmann_json.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

// Structure to represent a command response with explanation and example
struct CommandResponse {
    std::string response;
    std::string explanation;
    std::string example;
};

// Function to retrieve command from JSON
std::vector<CommandResponse> getCommandFromJSON(const std::string& commandKeyword, const std::string& directory) {
    std::vector<CommandResponse> commandResponses;

    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            std::ifstream file(entry.path());
            if (file.is_open()) {
                json jsonData;
                file >> jsonData;
                file.close();

                if (jsonData.contains(commandKeyword)) {
                    for (const auto& response : jsonData[commandKeyword]) {
                        commandResponses.push_back({
                            response["response"],
                            response["explanation"],
                            response.value("example", "")
                        });
                    }
                }
            }
        }
    }

    return commandResponses;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <command>" << std::endl;
        return 1;
    }

    std::string commandKeyword = argv[1];
    std::string directory = "./commands"; // Default directory to search

    // Retrieve command from JSON
    std::vector<CommandResponse> commandResponses = getCommandFromJSON(commandKeyword, directory);
    if (!commandResponses.empty()) {
        std::cout << "Terminal commands for " << commandKeyword << ":" << std::endl;
        for (const auto& response : commandResponses) {
            std::cout << "Command: " << response.response << std::endl;
            std::cout << "Explanation: " << response.explanation << std::endl;
            if (!response.example.empty()) {
                std::cout << "Example: " << response.example << std::endl;
            }
            std::cout << std::endl;
        }
    } else {
        std::cerr << "Command not found, If the command specified is a valid command you can contribute by adding command attribite to the codebase. Visit https://github.com/DevLouix/termaid.git" << std::endl;
    }

    return 0;
}
