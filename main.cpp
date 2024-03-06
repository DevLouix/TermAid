#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

// Structure to represent a command response with explanation
struct CommandResponse {
    std::string response;
    std::string explanation;
};

// Structure to represent a command and its responses
struct Command {
    std::string name;
    std::vector<CommandResponse> responses;
};

// Function to retrieve command from array
Command getCommandFromArray(const std::string& input) {
    // Array of commands and their corresponding responses with explanations
    std::unordered_map<std::string, Command> commandMap = {
        {"move", {"mv", 
            {
                {"mv", "for moving file"},
                {"mv -f", "force move"},
                {"mv file/path/or/folderpath destination/path", "Move file or folder to destination path"}
            }
        }},
        {"copy", {"cp",
            {
                {"cp", "for copying file"},
                {"cp -r", "recursive copy"},
                {"cp file/path/or/folderpath destination/path", "Copy file or folder to destination path"}
            }
        }},
        {"delete", {"rm",
            {
                {"rm", "for deleting file"},
                {"rm -r", "recursive delete"},
                {"rm -f file/path", "Force delete file or folder"}
            }
        }},
        // Add more commands as needed
    };
    
    // Look up the command in the array
    auto it = commandMap.find(input);
    if (it != commandMap.end()) {
        return it->second;
    } else {
        return {"", {}}; // Return empty command if not found
    }
}

int main() {
    std::string userInput;
    std::cout << "Enter command: ";
    std::cin >> userInput;
    
    // Retrieve command from array
    Command command = getCommandFromArray(userInput);
    if (!command.name.empty()) {
        std::cout << "Command found: " << command.name << std::endl;
        for (const auto& response : command.responses) {
            std::cout << response.response << " - " << response.explanation << std::endl;
        }
    } else {
        std::cout << "Command not found" << std::endl;
    }

    return 0;
}
