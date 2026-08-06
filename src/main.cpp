#include <iostream>
#include <string>
#include <cstring>
#include "nixhelp_core.h"
#include "nixhelp_tui.h"

void printHelp() {
    std::cout << R"(
NixHelp - Утилита для управления Nix/NixOS

Использование:
  nixhelp <команда> [аргументы]

Команды:
  tui                  Запустить интерактивный TUI интерфейс
  search <запрос>      Поиск пакетов в nixpkgs
  info <пакет>         Информация о пакете
  install <пакет>      Установить пакет
  uninstall <пакет>    Удалить пакет
  gc                   Быстрая очистка (garbage collection)
  gc-deep              Глубокая очистка (--delete-old)
  store                Информация о хранилище
  shell <пакет>        Запустить shell с пакетом
  flake-update         Обновить flake inputs
  help                 Показать эту справку

Примеры:
  nixhelp tui                    # Запустить TUI
  nixhelp search firefox         # Найти firefox
  nixhelp install git            # Установить git
  nixhelp gc                     # Очистить мусор
  nixhelp store                  # Показать инфо о хранилище
)";
}

int main(int argc, char* argv[]) {
    nixhelp::NixHelper helper;
    
    // Проверка доступности nix
    if (!helper.isNixAvailable()) {
        std::cerr << "⚠️  Nix не найден. Убедитесь, что Nix установлен.\n";
    }
    
    if (argc < 2) {
        printHelp();
        return 0;
    }
    
    std::string command = argv[1];
    
    if (command == "help" || command == "--help" || command == "-h") {
        printHelp();
        return 0;
    }
    
    if (command == "tui") {
        nixhelp::TUI tui(helper);
        tui.run();
        return 0;
    }
    
    if (command == "search") {
        if (argc < 3) {
            std::cerr << "Ошибка: укажите запрос для поиска\n";
            std::cerr << "Использование: nixhelp search <запрос>\n";
            return 1;
        }
        std::string query = argv[2];
        auto packages = helper.searchPackages(query);
        
        if (packages.empty()) {
            std::cout << "Ничего не найдено по запросу '" << query << "'\n";
        } else {
            std::cout << "Найдено пакетов: " << packages.size() << "\n\n";
            for (const auto& pkg : packages) {
                std::cout << "📦 " << pkg.name << "\n";
                if (!pkg.description.empty()) {
                    std::cout << "   " << pkg.description << "\n";
                }
            }
        }
        return 0;
    }
    
    if (command == "info") {
        if (argc < 3) {
            std::cerr << "Ошибка: укажите имя пакета\n";
            std::cerr << "Использование: nixhelp info <пакет>\n";
            return 1;
        }
        std::string packageName = argv[2];
        auto info = helper.getPackageInfo(packageName);
        
        std::cout << "📦 Пакет: " << info.name << "\n";
        if (!info.version.empty()) {
            std::cout << "   Версия: " << info.version << "\n";
        }
        if (!info.description.empty()) {
            std::cout << "   Описание: " << info.description << "\n";
        }
        return 0;
    }
    
    if (command == "install") {
        if (argc < 3) {
            std::cerr << "Ошибка: укажите имя пакета\n";
            std::cerr << "Использование: nixhelp install <пакет>\n";
            return 1;
        }
        std::string packageName = argv[2];
        std::cout << "Установка пакета: " << packageName << "...\n";
        
        if (helper.installPackage(packageName)) {
            std::cout << "✅ Пакет успешно установлен\n";
            return 0;
        } else {
            std::cerr << "❌ Ошибка при установке пакета\n";
            return 1;
        }
    }
    
    if (command == "uninstall") {
        if (argc < 3) {
            std::cerr << "Ошибка: укажите имя пакета\n";
            std::cerr << "Использование: nixhelp uninstall <пакет>\n";
            return 1;
        }
        std::string packageName = argv[2];
        std::cout << "Удаление пакета: " << packageName << "...\n";
        
        if (helper.uninstallPackage(packageName)) {
            std::cout << "✅ Пакет успешно удалён\n";
            return 0;
        } else {
            std::cerr << "❌ Ошибка при удалении пакета\n";
            return 1;
        }
    }
    
    if (command == "gc") {
        std::cout << "Выполнение garbage collection...\n";
        if (helper.garbageCollect(false)) {
            std::cout << "✅ Очистка выполнена успешно\n";
            return 0;
        } else {
            std::cerr << "❌ Ошибка при очистке\n";
            return 1;
        }
    }
    
    if (command == "gc-deep") {
        std::cout << "Выполнение глубокой очистки...\n";
        if (helper.garbageCollect(true)) {
            std::cout << "✅ Глубокая очистка выполнена успешно\n";
            return 0;
        } else {
            std::cerr << "❌ Ошибка при очистке\n";
            return 1;
        }
    }
    
    if (command == "store") {
        std::cout << "📊 Информация о хранилище:\n\n";
        std::cout << helper.getStoreInfo() << "\n";
        return 0;
    }
    
    if (command == "shell") {
        if (argc < 3) {
            std::cerr << "Ошибка: укажите имя пакета\n";
            std::cerr << "Использование: nixhelp shell <пакет>\n";
            return 1;
        }
        std::string packageName = argv[2];
        std::cout << "Запуск shell с пакетом: " << packageName << "...\n";
        helper.runShellWithPackage(packageName);
        return 0;
    }
    
    if (command == "flake-update") {
        std::cout << "Обновление flake inputs...\n";
        if (helper.updateFlake()) {
            std::cout << "✅ Flake успешно обновлён\n";
            return 0;
        } else {
            std::cerr << "❌ Ошибка при обновлении flake\n";
            return 1;
        }
    }
    
    std::cerr << "❌ Неизвестная команда: " << command << "\n";
    std::cerr << "Используйте 'nixhelp help' для списка команд.\n";
    return 1;
}
