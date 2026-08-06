/**
 * Nix Helper - Simple utility for Nix/NixOS package management
 * 
 * Features:
 * - Search for packages in nixpkgs
 * - Show package information
 * - Quick access to common nix commands
 */

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <sstream>

class NixHelper {
public:
    NixHelper() = default;

    // Execute a nix command and return exit code
    int executeCommand(const std::string& cmd) const {
        std::cout << ">>> Executing: " << cmd << std::endl;
        return std::system(cmd.c_str());
    }

    // Search for packages using nix search
    void searchPackages(const std::string& query) const {
        std::cout << "\n🔍 Searching for: " << query << std::endl;
        std::string cmd = "nix search nixpkgs " + query + " 2>/dev/null";
        executeCommand(cmd);
    }

    // Show package info
    void showPackageInfo(const std::string& packageName) const {
        std::cout << "\n📦 Package info: " << packageName << std::endl;
        std::string cmd = "nix show-derivation $(nix eval --raw nixpkgs." + 
                          packageName + ".outPath 2>/dev/null) 2>/dev/null";
        executeCommand(cmd);
    }

    // Install package (user profile)
    void installPackage(const std::string& packageName) const {
        std::cout << "\n⬇️  Installing: " << packageName << std::endl;
        std::string cmd = "nix-env -iA nixpkgs." + packageName;
        executeCommand(cmd);
    }

    // Uninstall package
    void uninstallPackage(const std::string& packageName) const {
        std::cout << "\n🗑️  Uninstalling: " << packageName << std::endl;
        std::string cmd = "nix-env -e " + packageName;
        executeCommand(cmd);
    }

    // Run garbage collection
    void garbageCollect(bool aggressive = false) const {
        std::cout << "\n🧹 Running garbage collection..." << std::endl;
        std::string cmd = "nix-collect-garbage";
        if (aggressive) {
            cmd += " -d";
            std::cout << "(Deleting old generations)" << std::endl;
        }
        executeCommand(cmd);
    }

    // Show nix store info
    void showStoreInfo() const {
        std::cout << "\n💾 Nix Store Info:" << std::endl;
        executeCommand("du -sh /nix/store 2>/dev/null");
        executeCommand("nix-store --query --requisites /run/current-system 2>/dev/null | wc -l");
    }

    // Print help menu
    void printHelp() const {
        std::cout << R"(
╔════════════════════════════════════════════════╗
║         Nix Helper - Quick Command Reference   ║
╠════════════════════════════════════════════════╣
║  search <query>      - Search for packages     ║
║  info <package>      - Show package info       ║
║  install <package>   - Install a package       ║
║  uninstall <package> - Remove a package        ║
║  gc                  - Garbage collect         ║
║  gc-deep             - Deep garbage collect    ║
║  store               - Show store info         ║
║  shell <package>     - Start shell with pkg    ║
║  run <package> <cmd> - Run command in env      ║
║  flake-update        - Update flake inputs     ║
║  help                - Show this help          ║
║  quit                - Exit                    ║
╚════════════════════════════════════════════════╝
)";
    }

    // Start interactive shell with package
    void startShell(const std::string& packageName) const {
        std::cout << "\n🐚 Starting shell with: " << packageName << std::endl;
        std::string cmd = "nix-shell -p " + packageName;
        executeCommand(cmd);
    }

    // Update flake inputs
    void updateFlake() const {
        std::cout << "\n🔄 Updating flake inputs..." << std::endl;
        executeCommand("nix flake update");
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
            helper.showStoreInfo();
        } else if (command == "shell" && argc > 2) {
            helper.startShell(argv[2]);
        } else if (command == "flake-update") {
            helper.updateFlake();
        } else if (command == "help") {
            helper.printHelp();
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
            helper.printHelp();
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
            helper.showStoreInfo();
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
