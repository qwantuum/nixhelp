# Nix Helper (nixhelp)

Простой хелпер для управления пакетами в Nix/NixOS, написанный на C++.

## Возможности

- 🔍 **Поиск пакетов** - поиск в nixpkgs
- 📦 **Информация о пакете** - просмотр деталей пакета
- ⬇️ **Установка/удаление** - управление пакетами в профиле пользователя
- 🧹 **Очистка** - запуск garbage collection (обычный и глубокий)
- 💾 **Информация о хранилище** - статистика /nix/store
- 🐚 **Shell** - запуск shell с окружением пакета
- 🔄 **Flake update** - обновление flake inputs

## Сборка

```bash
g++ -std=c++17 -Wall -Wextra -O2 -o nixhelp nixhelp.cpp
```

## Использование

### Интерактивный режим
```bash
./nixhelp
```

### Командная строка
```bash
# Поиск пакетов
./nixhelp search firefox

# Информация о пакете
./nixhelp info git

# Установка пакета
./nixhelp install neovim

# Удаление пакета
./nixhelp uninstall neovim

# Очистка (garbage collection)
./nixhelp gc

# Глубокая очистка (с удалением старых генераций)
./nixhelp gc-deep

# Информация о хранилище
./nixhelp store

# Запуск shell с пакетом
./nixhelp shell python3

# Обновление flake
./nixhelp flake-update

# Справка
./nixhelp help
```

## Команды интерактивного режима

| Команда | Описание |
|---------|----------|
| `search <query>` | Поиск пакетов по запросу |
| `info <package>` | Показать информацию о пакете |
| `install <package>` | Установить пакет |
| `uninstall <package>` | Удалить пакет |
| `gc` | Запустить garbage collection |
| `gc-deep` | Глубокая очистка (с -d флагом) |
| `store` | Показать информацию о хранилище |
| `shell <package>` | Запустить shell с пакетом |
| `flake-update` | Обновить flake inputs |
| `help` | Показать справку |
| `quit` / `exit` | Выход из программы |

## Требования

- Nix или NixOS
- C++17 совместимый компилятор
- Установленный `nix` в PATH

## Лицензия

MIT
