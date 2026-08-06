#include "nixhelp_tui.h"
#include <ncurses.h>
#include <string>
#include <vector>
#include <algorithm>

namespace nixhelp {

TUI::TUI(NixHelper& helper) : helper_(helper), running_(true), selectedIdx_(0) {}

void TUI::clearScreen() {
    clear();
    refresh();
}

void TUI::drawHeader() {
    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "========================================");
    mvprintw(1, 2, "NixHelp - Nix/NixOS Helper TUI");
    mvprintw(2, 0, "========================================");
    attroff(COLOR_PAIR(1));
    refresh();
}

void TUI::drawFooter() {
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);
    
    attron(COLOR_PAIR(2));
    mvprintw(maxy - 1, 0, "↑↓/j,k: Навигация | Enter: Выбор | q: Выход");
    attroff(COLOR_PAIR(2));
    refresh();
}

std::string TUI::getUserInput(const std::string& prompt) {
    echo();
    mvprintw(LINES - 3, 0, "%s", prompt.c_str());
    clrtoeol();
    refresh();
    
    char input[256];
    getnstr(input, sizeof(input));
    noecho();
    
    return std::string(input);
}

void TUI::showMessage(const std::string& message) {
    clear();
    drawHeader();
    
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);
    
    int y = 4;
    for (const auto& line : std::vector<std::string>({message})) {
        mvprintw(y++, 2, "%s", line.c_str());
        if (y >= maxy - 3) break;
    }
    
    mvprintw(maxy - 2, 0, "Нажмите любую клавишу для продолжения...");
    getch();
}

bool TUI::confirmAction(const std::string& question) {
    mvprintw(LINES - 3, 0, "%s (y/n): ", question.c_str());
    clrtoeol();
    refresh();
    
    int ch = getch();
    return (ch == 'y' || ch == 'Y');
}

void TUI::showMainMenu() {
    clear();
    drawHeader();
    
    std::vector<std::string> menuItems = {
        "🔍 Поиск пакетов",
        "📦 Установленные пакеты",
        "💾 Информация о хранилище",
        "🧹 Garbage Collection",
        "🔄 Обновить Flake",
        "❌ Выход"
    };
    
    int startY = 5;
    for (size_t i = 0; i < menuItems.size(); i++) {
        if (static_cast<int>(i) == selectedIdx_) {
            attron(A_REVERSE);
            mvprintw(startY + i, 4, "> %s", menuItems[i].c_str());
            attroff(A_REVERSE);
        } else {
            mvprintw(startY + i, 4, "  %s", menuItems[i].c_str());
        }
    }
    
    drawFooter();
}

void TUI::searchPackagesUI() {
    clear();
    drawHeader();
    
    std::string query = getUserInput("Введите запрос для поиска: ");
    if (query.empty()) {
        return;
    }
    
    auto packages = helper_.searchPackages(query);
    
    clear();
    drawHeader();
    mvprintw(4, 2, "Результаты поиска '%s':", query.c_str());
    
    int y = 6;
    for (size_t i = 0; i < packages.size() && i < 20; i++) {
        mvprintw(y++, 4, "• %s", packages[i].name.c_str());
        if (y >= LINES - 3) break;
    }
    
    if (packages.empty()) {
        mvprintw(6, 4, "Ничего не найдено");
    }
    
    mvprintw(LINES - 2, 0, "Нажмите любую клавишу для возврата...");
    getch();
}

void TUI::showInstalledPackages() {
    clear();
    drawHeader();
    
    mvprintw(4, 2, "Установленные пакеты:");
    
    std::string output = helper_.executeCommand("nix profile list 2>/dev/null");
    auto lines = helper_.splitLines(output);
    
    int y = 6;
    for (size_t i = 0; i < lines.size() && i < 20; i++) {
        mvprintw(y++, 4, "• %s", lines[i].c_str());
        if (y >= LINES - 3) break;
    }
    
    if (lines.empty()) {
        mvprintw(6, 4, "Нет установленных пакетов или ошибка получения списка");
    }
    
    mvprintw(LINES - 2, 0, "Нажмите любую клавишу для возврата...");
    getch();
}

void TUI::showStoreInfo() {
    clear();
    drawHeader();
    
    std::string info = helper_.getStoreInfo();
    
    mvprintw(4, 2, "Информация о хранилище:");
    mvprintw(6, 4, "%s", info.c_str());
    
    mvprintw(LINES - 2, 0, "Нажмите любую клавишу для возврата...");
    getch();
}

void TUI::gcMenu() {
    clear();
    drawHeader();
    
    mvprintw(4, 4, "Garbage Collection:");
    mvprintw(6, 6, "1. Быстрая очистка");
    mvprintw(7, 6, "2. Глубокая очистка (--delete-old)");
    mvprintw(8, 6, "3. Отмена");
    
    mvprintw(LINES - 2, 0, "Выберите действие (1-3): ");
    refresh();
    
    int choice = getch();
    
    if (choice == '1') {
        if (confirmAction("Выполнить быструю очистку?")) {
            helper_.garbageCollect(false);
            showMessage("Очистка выполнена");
        }
    } else if (choice == '2') {
        if (confirmAction("Выполнить глубокую очистку?")) {
            helper_.garbageCollect(true);
            showMessage("Глубокая очистка выполнена");
        }
    }
}

void TUI::updateFlakeUI() {
    if (confirmAction("Обновить flake inputs?")) {
        clear();
        drawHeader();
        mvprintw(4, 2, "Обновление flake...");
        refresh();
        
        bool success = helper_.updateFlake();
        
        if (success) {
            showMessage("Flake успешно обновлён");
        } else {
            showMessage("Ошибка при обновлении flake");
        }
    }
}

void TUI::run() {
    // Инициализация ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    
    // Цвета
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_BLUE);
        init_pair(3, COLOR_GREEN, COLOR_BLACK);
    }
    
    selectedIdx_ = 0;
    
    while (running_) {
        showMainMenu();
        
        int ch = getch();
        
        switch (ch) {
            case KEY_UP:
            case 'k':
            case 'K':
                if (selectedIdx_ > 0) selectedIdx_--;
                break;
                
            case KEY_DOWN:
            case 'j':
            case 'J':
                if (selectedIdx_ < 5) selectedIdx_++;
                break;
                
            case KEY_ENTER:
            case 10:
            case 13:
                switch (selectedIdx_) {
                    case 0: searchPackagesUI(); break;
                    case 1: showInstalledPackages(); break;
                    case 2: showStoreInfo(); break;
                    case 3: gcMenu(); break;
                    case 4: updateFlakeUI(); break;
                    case 5: running_ = false; break;
                }
                break;
                
            case 'q':
            case 'Q':
            case 27: // ESC
                running_ = false;
                break;
        }
    }
    
    endwin();
}

} // namespace nixhelp
