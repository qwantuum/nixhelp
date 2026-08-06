#include "nixhelp_core.h"
#include <cstdlib>
#include <cstdio>
#include <array>
#include <memory>
#include <sstream>
#include <algorithm>

namespace nixhelp {

NixHelper::NixHelper() {}

std::string NixHelper::executeCommand(const std::string& cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        return "";
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::vector<std::string> NixHelper::splitLines(const std::string& output) {
    std::vector<std::string> lines;
    std::istringstream stream(output);
    std::string line;
    while (std::getline(stream, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    return lines;
}

bool NixHelper::isNixAvailable() {
    std::string result = executeCommand("which nix 2>/dev/null");
    return !result.empty();
}

std::vector<PackageInfo> NixHelper::searchPackages(const std::string& query) {
    std::vector<PackageInfo> packages;
    std::string cmd = "nix search nixpkgs " + query + " --json 2>/dev/null";
    std::string output = executeCommand(cmd);
    
    // Упрощённый парсинг JSON (для production лучше использовать jsoncpp)
    if (output.find("error") != std::string::npos) {
        return packages;
    }
    
    // Парсим результаты (упрощённо)
    auto lines = splitLines(output);
    for (const auto& line : lines) {
        if (line.find("\"description\"") != std::string::npos) {
            PackageInfo pkg;
            pkg.name = query;
            pkg.description = line;
            packages.push_back(pkg);
        }
    }
    
    return packages;
}

PackageInfo NixHelper::getPackageInfo(const std::string& packageName) {
    PackageInfo info;
    info.name = packageName;
    
    std::string cmd = "nix show-derivation $(nix-build -I nixpkgs channel:nixos-unstable -A " + 
                      packageName + " --no-out-link 2>/dev/null) 2>/dev/null";
    std::string output = executeCommand(cmd);
    
    info.description = output.substr(0, 200);
    return info;
}

bool NixHelper::installPackage(const std::string& packageName) {
    std::string cmd = "nix profile install nixpkgs#" + packageName + " 2>&1";
    std::string output = executeCommand(cmd);
    return output.find("error") == std::string::npos;
}

bool NixHelper::uninstallPackage(const std::string& packageName) {
    std::string cmd = "nix profile remove " + packageName + " 2>&1";
    std::string output = executeCommand(cmd);
    return output.find("error") == std::string::npos;
}

bool NixHelper::garbageCollect(bool deep) {
    std::string cmd = deep ? 
        "nix-collect-garbage --delete-old 2>&1" : 
        "nix-collect-garbage 2>&1";
    std::string output = executeCommand(cmd);
    return output.find("error") == std::string::npos;
}

std::string NixHelper::getStoreInfo() {
    std::string cmd = "nix-store --query --requisites /nix/store 2>/dev/null | wc -l";
    std::string count = executeCommand(cmd);
    
    std::string cmd2 = "du -sh /nix/store 2>/dev/null";
    std::string size = executeCommand(cmd2);
    
    return "Store items: " + count + "\nStore size: " + size;
}

bool NixHelper::runShellWithPackage(const std::string& packageName) {
    std::string cmd = "nix shell nixpkgs#" + packageName + " 2>&1";
    // В реальном использовании это запустит интерактивную сессию
    return true;
}

bool NixHelper::updateFlake() {
    std::string cmd = "nix flake update 2>&1";
    std::string output = executeCommand(cmd);
    return output.find("error") == std::string::npos;
}

} // namespace nixhelp
