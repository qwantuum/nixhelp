# Nix Helper TUI

Полноценный TUI (Text User Interface) хелпер для Nix/NixOS на C++ с использованием ncurses.

## Возможности

### TUI Режим
- 🎨 Цветной интерфейс с ncurses
- 🔍 Поиск пакетов в реальном времени
- 📦 Просмотр установленных пакетов
- 💾 Информация о хранилище
- 🧹 Garbage collection (быстрая и глубокая очистка)
- 🔄 Обновление flake inputs
- ⌨️ Навигация стрелками или vim-клавишами (j/k)

### CLI Режим
```bash
./nixhelp search <query>       # Поиск пакетов
./nixhelp info <package>       # Информация о пакете
./nixhelp install <package>    # Установка пакета
./nixhelp uninstall <package>  # Удаление пакета
./nixhelp gc                   # Garbage collection
./nixhelp gc-deep              # Глубокая очистка
./nixhelp store                # Информация о хранилище
./nixhelp shell <package>      # Shell с пакетом
./nixhelp flake-update         # Обновить flake
./nixhelp tui                  # Запустить TUI
./nixhelp help                 # Справка
```

## Установка через Flakes

### 1. Добавьте в inputs вашего flake.nix:
```nix
inputs.nixhelp.url = "github:yourusername/nixhelp";
```

### 2. Добавьте в outputs:
```nix
outputs = { self, nixpkgs, nixhelp, ... }: {
  nixosConfigurations.mySystem = nixpkgs.lib.nixosSystem {
    system = "x86_64-linux";
    modules = [
      ./configuration.nix
      {
        environment.systemPackages = [ nixhelp.packages.x86_64-linux.default ];
      }
    ];
  };
}
```

### 3. Или установите напрямую:
```bash
nix profile install github:yourusername/nixhelp
```

### 4. Или используйте через nix run:
```bash
nix run github:yourusername/nixhelp
nix run github:yourusername/nixhelp -- search firefox
nix run github:yourusername/nixhelp -- tui
```

## Сборка из исходников

### Требования
- GCC с поддержкой C++17
- ncurses library

### Компиляция
```bash
g++ -std=c++17 -O2 -o nixhelp nixhelp.cpp -lncurses
```

### Разработка через flake
```bash
nix develop
# В shell: g++ -std=c++17 -O2 -o nixhelp nixhelp.cpp -lncurses
```

## Структура проекта

```
.
├── flake.nix       # Nix flake для сборки и установки
├── nixhelp.cpp     # Исходный код (TUI + CLI)
├── nixhelp         # Скомпилированный бинарник
└── README.md       # Документация
```

## Управление в TUI

- **↑/↓ или j/k** - Навигация по меню
- **Enter/Пробел** - Выбрать пункт
- **q или ESC** - Назад/Выход
- **В режиме поиска:**
  - Печатайте для поиска
  - **i** - Установить выбранный пакет
  - **↑/↓** - Навигация по результатам

## Лицензия

MIT
