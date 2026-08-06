#ifndef NIXHELP_CORE_H
#define NIXHELP_CORE_H

#include <string>
#include <vector>

namespace nixhelp {

struct PackageInfo {
    std::string name;
    std::string version;
    std::string description;
    std::string path;
};

class NixHelper {
public:
    NixHelper();
    
    // Поиск пакетов
    std::vector<PackageInfo> searchPackages(const std::string& query);
    
    // Информация о пакете
    PackageInfo getPackageInfo(const std::string& packageName);
    
    // Установка пакета
    bool installPackage(const std::string& packageName);
    
    // Удаление пакета
    bool uninstallPackage(const std::string& packageName);
    
    // Garbage collection
    bool garbageCollect(bool deep = false);
    
    // Информация о хранилище
    std::string getStoreInfo();
    
    // Shell с пакетом
    bool runShellWithPackage(const std::string& packageName);
    
    // Обновление flake
    bool updateFlake();
    
    // Проверка доступности nix
    bool isNixAvailable();
    
    // Публичные методы для TUI
    std::string executeCommand(const std::string& cmd);
    std::vector<std::string> splitLines(const std::string& output);
};

} // namespace nixhelp

#endif // NIXHELP_CORE_H
