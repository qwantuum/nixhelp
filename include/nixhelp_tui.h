#ifndef NIXHELP_TUI_H
#define NIXHELP_TUI_H

#include "nixhelp_core.h"
#include <string>
#include <vector>

namespace nixhelp {

class TUI {
public:
    TUI(NixHelper& helper);
    
    // Запуск TUI
    void run();
    
    // Отображение главного меню
    void showMainMenu();
    
    // Поиск пакетов (интерактивный)
    void searchPackagesUI();
    
    // Просмотр установленных пакетов
    void showInstalledPackages();
    
    // Информация о хранилище
    void showStoreInfo();
    
    // Garbage collection меню
    void gcMenu();
    
    // Обновление flake
    void updateFlakeUI();
    
private:
    NixHelper& helper_;
    bool running_;
    
    // Утилиты TUI
    void drawHeader();
    void drawFooter();
    void clearScreen();
    std::string getUserInput(const std::string& prompt);
    void showMessage(const std::string& message);
    bool confirmAction(const std::string& question);
    
    // Навигация
    int selectedIdx_;
    std::vector<std::string> currentResults_;
};

} // namespace nixhelp

#endif // NIXHELP_TUI_H
