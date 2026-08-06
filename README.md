<p align="center">
  <img src="./assets/logo.png" alt="Логотип NixHelp" width="200"/>
</p>

<h1 align="center">NixHelp - Nix/NixOS Helper Utility</h1>

<p align="center">
  <a href="https://github.com/qwantuum/nixhelp/actions/workflows/release.yml">
    <img src="https://github.com/qwantuum/nixhelp/actions/workflows/release.yml/badge.svg" alt="Build Status"/>
  </a>
  <a href="https://github.com/qwantuum/nixhelp/releases/latest">
    <img src="https://img.shields.io/github/v/release/qwantuum/nixhelp?color=blue&label=Release" alt="Latest Release"/>
  </a>
  <a href="https://github.com/qwantuum/nixhelp/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/qwantuum/nixhelp?color=green" alt="License"/>
  </a>
  <a href="https://github.com/qwantuum/nixhelp/stargazers">
    <img src="https://img.shields.io/github/stars/qwantuum/nixhelp?style=social" alt="Stars"/>
  </a>
  <a href="https://github.com/qwantuum/nixhelp/network/members">
    <img src="https://img.shields.io/github/forks/qwantuum/nixhelp?style=social" alt="Forks"/>
  </a>
  <br/>
  <a href="https://nixos.org">
    <img src="https://img.shields.io/badge/NixOS-unstable-blue.svg?logo=nixos" alt="NixOS"/>
  </a>
  <a href="https://search.nixos.org/packages">
    <img src="https://img.shields.io/badge/nixpkgs-available-green.svg?logo=nix" alt="Nix Packages"/>
  </a>
  <a href="https://github.com/qwantuum/nixhelp/issues">
    <img src="https://img.shields.io/github/issues/qwantuum/nixhelp" alt="Issues"/>
  </a>
  <a href="https://github.com/qwantuum/nixhelp/pulls">
    <img src="https://img.shields.io/github/issues-pr/qwantuum/nixhelp" alt="Pull Requests"/>
  </a>
  <br/>
  <a href="https://github.com/qwantuum/nixhelp/commits/main">
    <img src="https://img.shields.io/github/last-commit/qwantuum/nixhelp" alt="Last Commit"/>
  </a>
  <a href="https://github.com/qwantuum/nixhelp/graphs/contributors">
    <img src="https://img.shields.io/github/contributors/qwantuum/nixhelp" alt="Contributors"/>
  </a>
  <a href="https://github.com/qwantuum/nixhelp/pulse">
    <img src="https://img.shields.io/github/commit-activity/m/qwantuum/nixhelp" alt="Commit Activity"/>
  </a>
  <a href="https://github.com/qwantuum/nixhelp/blob/main/README.md">
    <img src="https://img.shields.io/badge/maintenance-active-green.svg" alt="Maintenance"/>
  </a>
  <br/>
  <a href="https://discord.gg/nixos">
    <img src="https://img.shields.io/discord/528304179436208138?label=Discord&logo=discord" alt="Discord"/>
  </a>
  <a href="https://matrix.to/#/#nix:nixos.org">
    <img src="https://img.shields.io/matrix/nix:nixos.org?label=Matrix&logo=matrix" alt="Matrix"/>
  </a>
  <a href="https://reddit.com/r/NixOS">
    <img src="https://img.shields.io/reddit/subreddit-subscribers/NixOS?style=social" alt="Reddit"/>
  </a>
  <br/>
  <img src="https://img.shields.io/badge/version-1.0.0-blue.svg" alt="Version"/>
  <img src="https://img.shields.io/badge/C%2B%2B-17-orange.svg" alt="C++"/>
  <img src="https://img.shields.io/badge/Nix-Flakes-lightblue.svg" alt="Nix Flakes"/>
  <img src="https://img.shields.io/badge/Platform-Linux%20%7C%20macOS-blue.svg" alt="Platform"/>
  <img src="https://img.shields.io/badge/Code%20Style-clang--format-purple.svg" alt="Code Style"/>
  <img src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg" alt="PRs Welcome"/>
</p>

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
=======
hixhelp
