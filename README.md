# NixHelp - Nix/NixOS Helper Utility

[![Build Status](https://github.com/qwantuum/nixhelp/actions/workflows/release.yml/badge.svg)](https://github.com/qwantuum/nixhelp/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

**NixHelp** — это утилита для удобного управления пакетами в Nix/NixOS с современным TUI интерфейсом и мощным CLI.

## 🚀 Возможности

### CLI режим
- 🔍 **Поиск пакетов** — быстрый поиск в nixpkgs
- 📦 **Установка/удаление** — управление пакетами через nix profile
- 🧹 **Garbage Collection** — очистка хранилища (быстрая и глубокая)
- 💾 **Информация о хранилище** — статистика использования
- 🐚 **Shell с пакетом** — временная среда с пакетом
- 🔄 **Flake update** — обновление flake inputs

### TUI режим
- 🎨 **Цветной интерфейс** — приятный терминальный UI
- ⌨️ **Навигация** — стрелки или vim-клавиши (j/k)
- 🖱️ **Поддержка мыши** — клики для выбора
- 📊 **Интерактивный поиск** — ввод запроса в реальном времени
- ✅ **Подтверждения** — защита от случайных действий

## 📦 Установка

### Через Nix Flakes (рекомендуется)

```bash
nix profile install github:qwantuum/nixhelp
```

### Из исходников

```bash
# Клонирование репозитория
git clone https://github.com/qwantuum/nixhelp.git
cd nixhelp

# Сборка через Python скрипт
python3 scripts/build.py all

# Или вручную
g++ -std=c++17 -O2 -o nixhelp src/main.cpp src/nixhelp_core.cpp src/nixhelp_tui.cpp -Iinclude -lncurses

# Установка в систему
sudo python3 scripts/build.py install
```

### Зависимости
- GCC с поддержкой C++17
- ncurses library (`libncurses-dev` на Debian/Ubuntu)

## 💡 Использование

### CLI команды

```bash
# Показать справку
nixhelp help

# Запустить TUI интерфейс
nixhelp tui

# Поиск пакетов
nixhelp search firefox

# Информация о пакете
nixhelp info git

# Установить пакет
nixhelp install git

# Удалить пакет
nixhelp uninstall git

# Garbage collection
nixhelp gc           # Быстрая очистка
nixhelp gc-deep      # Глубокая очистка

# Информация о хранилище
nixhelp store

# Shell с пакетом
nixhelp shell neovim

# Обновить flake
nixhelp flake-update
```

### TUI интерфейс

```bash
nixhelp tui
```

**Навигация:**
- `↑↓` или `j/k` — перемещение по меню
- `Enter` — выбор пункта
- `q` или `Esc` — выход
- Мышь — поддержка кликов

## 🛠 Разработка

### Структура проекта

```
nixhelp/
├── src/
│   ├── main.cpp           # Точка входа, CLI парсер
│   ├── nixhelp_core.cpp   # Основная логика Nix
│   └── nixhelp_tui.cpp    # TUI интерфейс (ncurses)
├── include/
│   ├── nixhelp_core.h     # Заголовки ядра
│   └── nixhelp_tui.h      # Заголовки TUI
├── scripts/
│   ├── build.py           # Скрипт сборки
│   └── test.py            # Набор тестов
├── config/
│   └── nixhelp.conf       # Конфигурация
├── flake.nix              # Nix Flake
├── package.json           # NPM-style метаданные
└── README.md              # Документация
```

### Сборка

```bash
# Полная сборка и тесты
python3 scripts/build.py all

# Только сборка
python3 scripts/build.py build

# Отладочная сборка
python3 scripts/build.py build --debug

# Запуск тестов
python3 scripts/test.py

# Очистка
python3 scripts/build.py clean

# Установка
python3 scripts/build.py install
```

### Тесты

```bash
python3 scripts/test.py
```

Тесты проверяют:
- Существование бинарника
- Команду help
- Неизвестные команды
- Информацию о хранилище
- Garbage collection
- Поиск пакетов

## 📄 Лицензия

MIT License — см. файл [LICENSE](LICENSE) для деталей.

## 🤝 Вклад

Pull requests приветствуются! Для крупных изменений пожалуйста откройте issue сначала.

## 🔗 Ссылки

- [GitHub Repository](https://github.com/qwantuum/nixhelp)
- [NixOS Wiki](https://wiki.nixos.org/)
- [Nix Packages Search](https://search.nixos.org/packages)

---

Сделано с ❤️ для сообщества Nix/NixOS
