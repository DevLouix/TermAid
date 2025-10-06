/*
 * TermAid - Terminal Command Aid and Logger
 * Cross-platform C++ implementation
 * 
 * Supports: Linux, macOS, Windows
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <algorithm>
#include <ctime>
#include <sstream>
#include "json.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

class TermAid {
private:
    fs::path base_dir;
    fs::path commands_dir;
    fs::path history_file;
    fs::path aliases_file;
    json command_db;
    json aliases;
    
    void load_commands() {
        if (!fs::exists(commands_dir)) {
            fs::create_directories(commands_dir);
            return;
        }
        
        for (const auto& entry : fs::recursive_directory_iterator(commands_dir)) {
            if (entry.path().extension() == ".json") {
                try {
                    std::ifstream file(entry.path());
                    json data;
                    file >> data;
                    command_db.update(data);
                } catch (const std::exception& e) {
                    std::cerr << "Warning: Could not parse " << entry.path() << std::endl;
                }
            }
        }
    }
    
    void load_aliases() {
        if (fs::exists(aliases_file)) {
            try {
                std::ifstream file(aliases_file);
                file >> aliases;
            } catch (...) {
                init_default_aliases();
            }
        } else {
            init_default_aliases();
        }
    }
    
    void init_default_aliases() {
        aliases = {
            {"ll", "ls -la"},
            {"la", "ls -A"},
            {"l", "ls -CF"},
            {"..", "cd .."},
            {"...", "cd ../.."}
        };
    }
    
    void save_aliases() {
        std::ofstream file(aliases_file);
        file << aliases.dump(2);
    }
    
    std::string resolve_alias(const std::string& command) {
        std::istringstream iss(command);
        std::string first_word;
        iss >> first_word;
        
        if (aliases.contains(first_word)) {
            std::string resolved = aliases[first_word];
            std::string rest;
            std::getline(iss, rest);
            if (!rest.empty()) {
                resolved += rest;
            }
            return resolved;
        }
        return command;
    }
    
    std::pair<std::string, std::vector<std::string>> parse_command(const std::string& command) {
        std::string resolved = resolve_alias(command);
        std::istringstream iss(resolved);
        std::vector<std::string> parts;
        std::string part;
        
        while (iss >> part) {
            parts.push_back(part);
        }
        
        if (parts.empty()) {
            return {"", {}};
        }
        
        std::string base_cmd = parts[0];
        std::vector<std::string> args(parts.begin() + 1, parts.end());
        
        // Handle sudo, time, etc.
        if (base_cmd == "sudo" || base_cmd == "time" || base_cmd == "nice") {
            if (args.size() > 0) {
                base_cmd = args[0];
                args.erase(args.begin());
            }
        }
        
        return {base_cmd, args};
    }
    
    json get_command_explanation(const std::string& command) {
        auto [base_cmd, args] = parse_command(command);
        
        if (base_cmd.empty()) {
            return nullptr;
        }
        
        json best_match = nullptr;
        int best_score = 0;
        
        for (auto& [key, cmd_list] : command_db.items()) {
            if (!cmd_list.is_array()) continue;
            
            for (auto& cmd_info : cmd_list) {
                if (!cmd_info.contains("response")) continue;
                
                std::string response = cmd_info["response"];
                std::istringstream iss(response);
                std::string first_word;
                iss >> first_word;
                
                if (first_word != base_cmd) continue;
                
                int score = 1;
                
                // Prefer exact matches
                if (response == command) {
                    score += 20;
                }
                
                // Match with arguments
                if (!args.empty() && response.find('<') != std::string::npos) {
                    score += 2;
                }
                
                if (score > best_score) {
                    best_score = score;
                    best_match = cmd_info;
                }
            }
        }
        
        return best_match;
    }
    
    void log_command(const std::string& command, const std::string& explanation = "", 
                     bool executed = false, int exit_code = 0) {
        json history;
        
        if (fs::exists(history_file)) {
            try {
                std::ifstream file(history_file);
                file >> history;
            } catch (...) {
                history = json::array();
            }
        } else {
            history = json::array();
        }
        
        auto now = std::time(nullptr);
        char timestamp[100];
        std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", std::localtime(&now));
        
        json entry = {
            {"timestamp", timestamp},
            {"command", command},
            {"explanation", explanation},
            {"executed", executed},
            {"exit_code", exit_code}
        };
        
        history.push_back(entry);
        
        // Keep only last 1000 entries
        if (history.size() > 1000) {
            history.erase(history.begin(), history.begin() + (history.size() - 1000));
        }
        
        std::ofstream file(history_file);
        file << history.dump(2);
    }

public:
    TermAid() {
        base_dir = fs::current_path();
        commands_dir = base_dir / "commands";
        history_file = base_dir / ".termaid_history.json";
        aliases_file = base_dir / ".termaid_aliases.json";
        
        load_commands();
        load_aliases();
    }
    
    void list_commands() {
        std::cout << "\nAvailable Commands in Database:\n";
        std::cout << std::string(60, '=') << "\n";
        
        std::set<std::string> all_commands;
        
        for (auto& [key, cmd_list] : command_db.items()) {
            if (!cmd_list.is_array()) continue;
            
            for (auto& cmd_info : cmd_list) {
                if (cmd_info.contains("response")) {
                    std::string response = cmd_info["response"];
                    std::istringstream iss(response);
                    std::string first_word;
                    iss >> first_word;
                    all_commands.insert(first_word);
                }
            }
        }
        
        for (const auto& cmd : all_commands) {
            std::cout << "  • " << cmd << "\n";
        }
        
        std::cout << "\nTotal: " << all_commands.size() << " commands\n";
        std::cout << std::string(60, '=') << "\n";
    }
    
    void search_commands(const std::string& keyword) {
        std::cout << "\nTerminal commands for '" << keyword << "':\n";
        std::cout << std::string(60, '=') << "\n";
        
        std::string keyword_lower = keyword;
        std::transform(keyword_lower.begin(), keyword_lower.end(), keyword_lower.begin(), ::tolower);
        
        bool found = false;
        
        for (auto& [key, cmd_list] : command_db.items()) {
            if (!cmd_list.is_array()) continue;
            
            std::string key_lower = key;
            std::transform(key_lower.begin(), key_lower.end(), key_lower.begin(), ::tolower);
            
            for (auto& cmd_info : cmd_list) {
                std::string response = cmd_info.value("response", "");
                std::string explanation = cmd_info.value("explanation", "");
                
                std::string response_lower = response;
                std::string explanation_lower = explanation;
                std::transform(response_lower.begin(), response_lower.end(), response_lower.begin(), ::tolower);
                std::transform(explanation_lower.begin(), explanation_lower.end(), explanation_lower.begin(), ::tolower);
                
                if (key_lower.find(keyword_lower) != std::string::npos ||
                    response_lower.find(keyword_lower) != std::string::npos ||
                    explanation_lower.find(keyword_lower) != std::string::npos) {
                    
                    found = true;
                    std::cout << "\nCommand: " << response << "\n";
                    std::cout << "Explanation: " << explanation << "\n";
                    if (cmd_info.contains("example")) {
                        std::cout << "Example: " << cmd_info["example"] << "\n";
                    }
                    std::cout << std::string(60, '-') << "\n";
                }
            }
        }
        
        if (!found) {
            std::cout << "No commands found for: " << keyword << "\n";
        }
    }
    
    void explain_command(const std::string& command) {
        // Check for piped commands
        if (command.find('|') != std::string::npos) {
            explain_piped_command(command);
            return;
        }
        
        // Check for alias
        std::string resolved = resolve_alias(command);
        if (resolved != command) {
            std::cout << "\nAlias resolved: " << command << " → " << resolved << "\n";
        }
        
        auto [base_cmd, args] = parse_command(resolved);
        
        if (base_cmd.empty()) {
            std::cout << "Could not parse command\n";
            return;
        }
        
        std::cout << "\n" << std::string(60, '=') << "\n";
        std::cout << "Command: " << resolved << "\n";
        std::cout << std::string(60, '=') << "\n";
        
        json explanation = get_command_explanation(resolved);
        
        if (!explanation.is_null()) {
            std::cout << "\nExplanation: " << explanation.value("explanation", "No explanation available") << "\n";
            if (explanation.contains("example")) {
                std::cout << "Example: " << explanation["example"] << "\n";
            }
        } else {
            std::cout << "\nBase command: " << base_cmd << "\n";
            std::cout << "No detailed explanation available in database.\n";
            std::cout << "Try: man " << base_cmd << " or " << base_cmd << " --help\n";
        }
        
        if (!args.empty()) {
            std::cout << "\nArguments:";
            for (const auto& arg : args) {
                std::cout << " " << arg;
            }
            std::cout << "\n";
        }
        
        std::cout << std::string(60, '=') << "\n\n";
    }
    
    void explain_piped_command(const std::string& command) {
        std::cout << "\n" << std::string(60, '=') << "\n";
        std::cout << "Piped Command: " << command << "\n";
        std::cout << std::string(60, '=') << "\n\n";
        
        std::vector<std::string> parts;
        std::istringstream iss(command);
        std::string part;
        
        while (std::getline(iss, part, '|')) {
            // Trim whitespace
            part.erase(0, part.find_first_not_of(" \t"));
            part.erase(part.find_last_not_of(" \t") + 1);
            parts.push_back(part);
        }
        
        std::cout << "This command has " << parts.size() << " parts connected by pipes:\n\n";
        
        for (size_t i = 0; i < parts.size(); ++i) {
            std::cout << "Part " << (i + 1) << ": " << parts[i] << "\n";
            json explanation = get_command_explanation(parts[i]);
            if (!explanation.is_null()) {
                std::cout << "  → " << explanation.value("explanation", "No explanation available") << "\n";
            } else {
                auto [base_cmd, _] = parse_command(parts[i]);
                std::cout << "  → Command: " << base_cmd << " (no detailed explanation available)\n";
            }
            std::cout << "\n";
        }
        
        std::cout << "How pipes work:\n";
        std::cout << "  • Each command's output becomes the next command's input\n";
        std::cout << "  • Data flows left to right through the pipeline\n";
        std::cout << std::string(60, '=') << "\n\n";
    }
    
    void show_history(int limit = 20) {
        if (!fs::exists(history_file)) {
            std::cout << "No command history found.\n";
            return;
        }
        
        json history;
        try {
            std::ifstream file(history_file);
            file >> history;
        } catch (...) {
            std::cout << "Could not read history file.\n";
            return;
        }
        
        std::cout << "\nCommand History (last " << limit << " entries):\n";
        std::cout << std::string(80, '=') << "\n";
        
        int start = std::max(0, (int)history.size() - limit);
        for (int i = start; i < history.size(); ++i) {
            auto& entry = history[i];
            std::string status = entry.value("executed", false) ? 
                (entry.value("exit_code", -1) == 0 ? "✓" : "✗") : "○";
            
            std::cout << status << " [" << entry.value("timestamp", "N/A") << "] " 
                     << entry.value("command", "N/A") << "\n";
            
            if (entry.contains("explanation") && !entry["explanation"].get<std::string>().empty()) {
                std::cout << "   → " << entry["explanation"] << "\n";
            }
        }
        
        std::cout << std::string(80, '=') << "\n";
    }
    
    void manage_aliases(const std::string& action, const std::string& alias_name = "", 
                       const std::string& alias_cmd = "") {
        if (action == "list") {
            if (aliases.empty()) {
                std::cout << "No aliases defined.\n";
                return;
            }
            
            std::cout << "\nDefined Aliases:\n";
            std::cout << std::string(60, '=') << "\n";
            for (auto& [name, cmd] : aliases.items()) {
                std::cout << "  " << std::left << std::setw(15) << name << " → " << cmd << "\n";
            }
            std::cout << std::string(60, '=') << "\n";
        }
        else if (action == "add") {
            aliases[alias_name] = alias_cmd;
            save_aliases();
            std::cout << "✓ Alias added: " << alias_name << " → " << alias_cmd << "\n";
        }
        else if (action == "remove") {
            if (aliases.contains(alias_name)) {
                aliases.erase(alias_name);
                save_aliases();
                std::cout << "✓ Alias removed: " << alias_name << "\n";
            } else {
                std::cout << "Alias not found: " << alias_name << "\n";
            }
        }
    }
};

void print_usage() {
    std::cout << "TermAid - Terminal Command Aid and Logger\n\n";
    std::cout << "Usage: termaid <action> [arguments]\n\n";
    std::cout << "Actions:\n";
    std::cout << "  list                    List all available commands\n";
    std::cout << "  search <keyword>        Search for commands\n";
    std::cout << "  explain <command>       Explain a command\n";
    std::cout << "  history [limit]         Show command history\n";
    std::cout << "  alias list              List all aliases\n";
    std::cout << "  alias add <name> <cmd>  Add alias\n";
    std::cout << "  alias remove <name>     Remove alias\n\n";
    std::cout << "Examples:\n";
    std::cout << "  termaid list\n";
    std::cout << "  termaid search docker\n";
    std::cout << "  termaid explain \"ls -la\"\n";
    std::cout << "  termaid history 50\n";
    std::cout << "  termaid alias add ll \"ls -la\"\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }
    
    TermAid termaid;
    std::string action = argv[1];
    
    if (action == "list") {
        termaid.list_commands();
    }
    else if (action == "search" && argc >= 3) {
        termaid.search_commands(argv[2]);
    }
    else if (action == "explain" && argc >= 3) {
        std::string command;
        for (int i = 2; i < argc; ++i) {
            if (i > 2) command += " ";
            command += argv[i];
        }
        termaid.explain_command(command);
    }
    else if (action == "history") {
        int limit = (argc >= 3) ? std::stoi(argv[2]) : 20;
        termaid.show_history(limit);
    }
    else if (action == "alias") {
        if (argc < 3) {
            termaid.manage_aliases("list");
        } else {
            std::string sub_action = argv[2];
            if (sub_action == "list") {
                termaid.manage_aliases("list");
            } else if (sub_action == "add" && argc >= 5) {
                std::string name = argv[3];
                std::string cmd;
                for (int i = 4; i < argc; ++i) {
                    if (i > 4) cmd += " ";
                    cmd += argv[i];
                }
                termaid.manage_aliases("add", name, cmd);
            } else if (sub_action == "remove" && argc >= 4) {
                termaid.manage_aliases("remove", argv[3]);
            }
        }
    }
    else {
        print_usage();
        return 1;
    }
    
    return 0;
}
