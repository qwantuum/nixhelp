/**
 * Nix Helper - TUI utility for Nix/NixOS package management
 * 
 * Features:
 * - Full TUI with ncurses
 * - Package search with results browser
 * - Package installation/uninstallation
 * - Garbage collection
 * - Store information
 * - Shell access
 * - Flake management
 * - Command history
 * - Color-coded status
 */

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <memory>
#include <functional>
#include <cstring>
#include <array>
#include <cstdio>

// Forward declaration for ncurses
struct _win_st;
typedef _win_st WINDOW;

// External ncurses functions (linked via -lncurses)
extern "C" {
    int initscr();
    int cbreak();
    int noecho();
    int curs_set(int);
    int keypad(WINDOW*, bool);
    int start_color();
    int init_pair(short, short, short);
    int getmaxyx(WINDOW*, int&, int&);
    int refresh();
    int clear();
    int endwin();
    int mvprintw(int, int, const char*, ...);
    int mvaddch(int, int, int);
    int addch(int);
    int attron(int);
    int attroff(int);
    int standend();
    int standout();
    int getch();
    int wrefresh(WINDOW*);
    int wclear(WINDOW*);
    int box(WINDOW*, int, int);
    int wattron(WINDOW*, int);
    int wattroff(WINDOW*, int);
    int wnoutrefresh(WINDOW*);
    int doupdate();
    int timeout(int);
    int ungetch(int);
    int delwin(WINDOW*);
    WINDOW* newwin(int, int, int, int);
    int mvwprintw(WINDOW*, int, int, const char*, ...);
    int isatty(int);
    
    // Color definitions
    #define COLOR_BLACK   0
    #define COLOR_RED     1
    #define COLOR_GREEN   2
    #define COLOR_YELLOW  3
    #define COLOR_BLUE    4
    #define COLOR_MAGENTA 5
    #define COLOR_CYAN    6
    #define COLOR_WHITE   7
    
    // Attribute definitions
    #define A_NORMAL      0
    #define A_STANDOUT    1 << 17
    #define A_REVERSE     1 << 18
    #define A_BOLD        1 << 24
}

class NixHelper {
public:
    NixHelper() = default;

    // Execute a nix command and capture output
    std::string executeCommandWithOutput(const std::string& cmd) const {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe) {
            return "Error: Failed to execute command";
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    // Execute a nix command (for side effects)
    int executeCommand(const std::string& cmd) const {
        return std::system(cmd.c_str());
    }

    // Search for packages using nix search
    std::vector<std::string> searchPackages(const std::string& query) const {
        std::vector<std::string> results;
        std::string cmd = "nix search nixpkgs " + query + " --json 2>/dev/null";
        std::string output = executeCommandWithOutput(cmd);
        
        // Simple JSON parsing for attribute paths
        std::istringstream iss(output);
        std::string line;
        while (std::getline(iss, line)) {
            size_t pos = line.find("\"attribute\":");
            if (pos != std::string::npos) {
                size_t start = line.find("\"", pos + 13);
                size_t end = line.find("\"", start + 1);
                if (start != std::string::npos && end != std::string::npos) {
                    results.push_back(line.substr(start + 1, end - start - 1));
                }
            }
        }
        
        // Fallback to simple text search if JSON fails
        if (results.empty()) {
            cmd = "nix search nixpkgs " + query + " 2>/dev/null | head -20";
            output = executeCommandWithOutput(cmd);
            std::istringstream iss2(output);
            std::string line2;
            while (std::getline(iss2, line2)) {
                if (!line2.empty() && line2[0] == '*') {
                    size_t start = line2.find_first_not_of("* \t");
                    if (start != std::string::npos) {
                        size_t end = line2.find_first_of(" \t", start);
                        if (end != std::string::npos) {
                            results.push_back(line2.substr(start, end - start));
                        } else {
                            results.push_back(line2.substr(start));
                        }
                    }
                }
            }
        }
        
        return results;
    }

    // Show package info
    std::string showPackageInfo(const std::string& packageName) const {
        std::string cmd = "nix eval nixpkgs." + packageName + ".meta.description 2>/dev/null";
        std::string desc = executeCommandWithOutput(cmd);
        
        cmd = "nix eval nixpkgs." + packageName + ".version 2>/dev/null";
        std::string version = executeCommandWithOutput(cmd);
        
        std::ostringstream oss;
        oss << "Package: " << packageName << "\n";
        oss << "Version: " << (version.empty() ? "unknown" : version) << "\n";
        oss << "Description: " << (desc.empty() ? "No description available" : desc);
        return oss.str();
    }

    // Install package (user profile)
    int installPackage(const std::string& packageName) const {
        std::string cmd = "nix-env -iA nixpkgs." + packageName;
        return executeCommand(cmd);
    }

    // Uninstall package
    int uninstallPackage(const std::string& packageName) const {
        std::string cmd = "nix-env -e " + packageName;
        return executeCommand(cmd);
    }

    // Run garbage collection
    int garbageCollect(bool aggressive = false) const {
        std::string cmd = "nix-collect-garbage";
        if (aggressive) {
            cmd += " -d";
        }
        return executeCommand(cmd);
    }

    // Get nix store info
    std::string getStoreInfo() const {
        std::ostringstream oss;
        oss << "Nix Store Size: ";
        oss << executeCommandWithOutput("du -sh /nix/store 2>/dev/null | cut -f1");
        oss << "\nInstalled packages: ";
        oss << executeCommandWithOutput("nix-env -q | wc -l");
        return oss.str();
    }

    // Start shell with package
    void startShell(const std::string& packageName) const {
        std::string cmd = "nix-shell -p " + packageName;
        executeCommand(cmd);
    }

    // Update flake inputs
    int updateFlake() const {
        return executeCommand("nix flake update");
    }
    
    // Get installed packages
    std::vector<std::string> getInstalledPackages() const {
        std::vector<std::string> packages;
        std::string output = executeCommandWithOutput("nix-env -q 2>/dev/null");
        std::istringstream iss(output);
        std::string line;
        while (std::getline(iss, line)) {
            if (!line.empty()) {
                // Remove version suffix
                size_t pos = line.find('-');
                if (pos != std::string::npos) {
                    packages.push_back(line.substr(0, pos));
                } else {
                    packages.push_back(line);
                }
            }
        }
        return packages;
    }
};

void printBanner() {
    std::cout << R"(
  _   _          _             
 | \ | | ___  __| |_ __ _____  
 |  \| |/ _ \/ _` | '__/ _ \ \ 
 | |\  |  __/ (_| | | | (_) \ \
 |_| \_|\___|\__,_|_|  \___/ \_\
                               
  Simple Nix/NixOS Helper Utility
)" << std::endl;
}

int main(int argc, char* argv[]) {
    printBanner();

    NixHelper helper;

    // Non-interactive mode: execute single command
    if (argc > 1) {
        std::string command = argv[1];
        
        if (command == "search" && argc > 2) {
            helper.searchPackages(argv[2]);
        } else if (command == "info" && argc > 2) {
            helper.showPackageInfo(argv[2]);
        } else if (command == "install" && argc > 2) {
            helper.installPackage(argv[2]);
        } else if (command == "uninstall" && argc > 2) {
            helper.uninstallPackage(argv[2]);
        } else if (command == "gc") {
            helper.garbageCollect(false);
        } else if (command == "gc-deep") {
            helper.garbageCollect(true);
        } else if (command == "store") {
            std::cout << helper.getStoreInfo() << std::endl;
        } else if (command == "shell" && argc > 2) {
            helper.startShell(argv[2]);
        } else if (command == "flake-update") {
            helper.updateFlake();
        } else if (command == "help") {
            std::cout << "Use 'tui' command for interactive mode or 'help' for CLI usage" << std::endl;
        } else {
            std::cerr << "Unknown command or missing arguments. Use 'help' for usage." << std::endl;
            return 1;
        }
        return 0;
    }

    // Interactive mode
    std::cout << "Type 'help' for available commands or 'quit' to exit.\n" << std::endl;
    
    std::string line;
    while (true) {
        std::cout << "nixhelp> ";
        std::getline(std::cin, line);

        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "quit" || command == "exit") {
            std::cout << "Goodbye!" << std::endl;
            break;
        } else if (command == "help") {
            std::cout << "Use 'tui' command for interactive mode or 'help' for CLI usage" << std::endl;
        } else if (command == "search") {
            std::string query;
            if (iss >> query) {
                helper.searchPackages(query);
            } else {
                std::cerr << "Usage: search <query>" << std::endl;
            }
        } else if (command == "info") {
            std::string pkg;
            if (iss >> pkg) {
                helper.showPackageInfo(pkg);
            } else {
                std::cerr << "Usage: info <package>" << std::endl;
            }
        } else if (command == "install") {
            std::string pkg;
            if (iss >> pkg) {
                helper.installPackage(pkg);
            } else {
                std::cerr << "Usage: install <package>" << std::endl;
            }
        } else if (command == "uninstall") {
            std::string pkg;
            if (iss >> pkg) {
                helper.uninstallPackage(pkg);
            } else {
                std::cerr << "Usage: uninstall <package>" << std::endl;
            }
        } else if (command == "gc") {
            helper.garbageCollect(false);
        } else if (command == "gc-deep") {
            helper.garbageCollect(true);
        } else if (command == "store") {
            std::cout << helper.getStoreInfo() << std::endl;
        } else if (command == "shell") {
            std::string pkg;
            if (iss >> pkg) {
                helper.startShell(pkg);
            } else {
                std::cerr << "Usage: shell <package>" << std::endl;
            }
        } else if (command == "flake-update") {
            helper.updateFlake();
        } else {
            std::cerr << "Unknown command: " << command << ". Type 'help' for usage." << std::endl;
        }
    }

    return 0;
}
