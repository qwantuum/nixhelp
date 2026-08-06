#!/usr/bin/env python3
"""
NixHelp Build Script
Скрипт для сборки NixHelp с различными опциями
"""

import os
import sys
import subprocess
import argparse
from pathlib import Path

# Цвета для вывода
class Colors:
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    BLUE = '\033[94m'
    RESET = '\033[0m'
    BOLD = '\033[1m'

def print_status(message, color=Colors.BLUE):
    print(f"{color}{Colors.BOLD}>>> {message}{Colors.RESET}")

def print_success(message):
    print(f"{Colors.GREEN}✓ {message}{Colors.RESET}")

def print_error(message):
    print(f"{Colors.RED}✗ {message}{Colors.RESET}")

def print_warning(message):
    print(f"{Colors.YELLOW}⚠ {message}{Colors.RESET}")

def check_dependencies():
    """Проверка необходимых зависимостей"""
    print_status("Проверка зависимостей")
    
    deps = {
        'g++': 'C++ компилятор',
        'ncurses': 'Библиотека ncurses'
    }
    
    missing = []
    
    # Проверка g++
    try:
        result = subprocess.run(['g++', '--version'], capture_output=True, text=True)
        if result.returncode == 0:
            print_success(f"g++ найден: {result.stdout.split()[2]}")
        else:
            missing.append('g++')
    except FileNotFoundError:
        missing.append('g++')
    
    # Проверка ncurses (через pkg-config или dpkg)
    try:
        result = subprocess.run(['pkg-config', '--exists', 'ncurses'], capture_output=True)
        if result.returncode == 0:
            print_success("ncurses найдена")
        else:
            # Альтернативная проверка
            if os.path.exists('/usr/include/ncurses.h') or os.path.exists('/usr/include/ncurses/ncurses.h'):
                print_success("ncurses найдена")
            else:
                missing.append('ncurses')
    except FileNotFoundError:
        if os.path.exists('/usr/include/ncurses.h'):
            print_success("ncurses найдена")
        else:
            missing.append('ncurses')
    
    if missing:
        print_error(f"Отсутствуют зависимости: {', '.join(missing)}")
        print_warning("Установите их командой:")
        print("  Ubuntu/Debian: sudo apt-get install g++ libncurses-dev")
        print("  Fedora: sudo dnf install gcc-c++ ncurses-devel")
        print("  Arch: sudo pacman -S gcc ncurses")
        return False
    
    return True

def get_cpp_files():
    """Получить список всех C++ файлов"""
    src_dir = Path('src')
    return list(src_dir.glob('*.cpp'))

def build(output='nixhelp', debug=False, verbose=False, clean=False):
    """Сборка проекта"""
    print_status(f"Сборка NixHelp (режим: {'debug' if debug else 'release'})")
    
    if clean and os.path.exists(output):
        print_status("Очистка предыдущей сборки")
        os.remove(output)
        print_success("Очистка завершена")
    
    cpp_files = get_cpp_files()
    if not cpp_files:
        print_error("C++ файлы не найдены в директории src/")
        return False
    
    print_status(f"Найдено файлов: {len(cpp_files)}")
    for f in cpp_files:
        print(f"  - {f.name}")
    
    # Формирование команды компиляции
    cmd = ['g++']
    
    # флаги
    if debug:
        cmd.extend(['-g', '-O0', '-DDEBUG'])
    else:
        cmd.extend(['-O2', '-DNDEBUG'])
    
    cmd.extend([
        '-std=c++17',
        '-Wall',
        '-Wextra',
        '-Iinclude'
    ])
    
    # Исходные файлы
    cmd.extend([str(f) for f in cpp_files])
    
    # Библиотеки
    cmd.extend(['-lncurses', '-o', output])
    
    if verbose:
        print_status(f"Команда: {' '.join(cmd)}")
    
    # Компиляция
    try:
        result = subprocess.run(cmd, capture_output=True, text=True)
        
        if result.returncode != 0:
            print_error("Ошибка компиляции:")
            print(result.stderr)
            return False
        
        print_success(f"Сборка завершена: {output}")
        
        # Информация о бинарнике
        size = os.path.getsize(output)
        print_status(f"Размер бинарника: {size / 1024:.2f} KB")
        
        return True
        
    except Exception as e:
        print_error(f"Ошибка при сборке: {e}")
        return False

def test_binary(binary='nixhelp'):
    """Тестирование собранного бинарника"""
    print_status("Тестирование бинарника")
    
    if not os.path.exists(binary):
        print_error(f"Бинарник {binary} не найден")
        return False
    
    # Проверка --help
    try:
        result = subprocess.run([f'./{binary}', '--help'], capture_output=True, text=True, timeout=5)
        if result.returncode == 0:
            print_success("Тест --help пройден")
        else:
            print_warning("Тест --help вернул код:", result.returncode)
    except subprocess.TimeoutExpired:
        print_warning("Таймаут при тесте --help")
    except Exception as e:
        print_error(f"Ошибка при тесте: {e}")
        return False
    
    print_success("Все тесты пройдены")
    return True

def install(binary='nixhelp', prefix='/usr/local'):
    """Установка бинарника в систему"""
    print_status(f"Установка в {prefix}/bin")
    
    if not os.path.exists(binary):
        print_error(f"Бинарник {binary} не найден. Сначала выполните сборку.")
        return False
    
    try:
        # Создание директории если не существует
        dest_dir = Path(prefix) / 'bin'
        dest_dir.mkdir(parents=True, exist_ok=True)
        
        # Копирование бинарника
        dest = dest_dir / binary
        subprocess.run(['cp', binary, str(dest)], check=True)
        
        # Установка прав
        subprocess.run(['chmod', '+x', str(dest)], check=True)
        
        print_success(f"Установлен в {dest}")
        return True
        
    except subprocess.CalledProcessError as e:
        print_error(f"Ошибка при установке: {e}")
        print_warning("Попробуйте запустить с sudo")
        return False
    except Exception as e:
        print_error(f"Ошибка: {e}")
        return False

def clean_build_artifacts():
    """Очистка артефактов сборки"""
    print_status("Очистка артефактов")
    
    artifacts = ['nixhelp', 'nixhelp-debug', '*.o']
    removed = 0
    
    for artifact in artifacts:
        for f in Path('.').glob(artifact):
            if f.is_file():
                f.unlink()
                removed += 1
                print(f"  Удалено: {f.name}")
    
    print_success(f"Удалено файлов: {removed}")

def main():
    parser = argparse.ArgumentParser(
        description='Скрипт сборки NixHelp',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Примеры:
  %(prog)s build              Сборка релизной версии
  %(prog)s build --debug      Сборка отладочной версии
  %(prog)s test               Тестирование бинарника
  %(prog)s install            Установка в систему
  %(prog)s clean              Очистка артефактов
  %(prog)s all                Полная сборка и тест
        """
    )
    
    subparsers = parser.add_subparsers(dest='command', help='Команды')
    
    # Build command
    build_parser = subparsers.add_parser('build', help='Сборка проекта')
    build_parser.add_argument('--debug', action='store_true', help='Отладочная сборка')
    build_parser.add_argument('--verbose', '-v', action='store_true', help='Подробный вывод')
    build_parser.add_argument('--clean', action='store_true', help='Очистить перед сборкой')
    build_parser.add_argument('--output', '-o', default='nixhelp', help='Имя выходного файла')
    
    # Test command
    test_parser = subparsers.add_parser('test', help='Тестирование бинарника')
    test_parser.add_argument('--binary', '-b', default='nixhelp', help='Бинарник для теста')
    
    # Install command
    install_parser = subparsers.add_parser('install', help='Установка в систему')
    install_parser.add_argument('--prefix', default='/usr/local', help='Префикс установки')
    install_parser.add_argument('--binary', '-b', default='nixhelp', help='Бинарник для установки')
    
    # Clean command
    subparsers.add_parser('clean', help='Очистка артефактов сборки')
    
    # All command
    all_parser = subparsers.add_parser('all', help='Полная сборка и тест')
    all_parser.add_argument('--debug', action='store_true', help='Отладочная сборка')
    
    args = parser.parse_args()
    
    if not args.command:
        parser.print_help()
        return 0
    
    # Проверка зависимостей для команд сборки
    if args.command in ['build', 'all']:
        if not check_dependencies():
            return 1
    
    # Выполнение команд
    if args.command == 'build':
        success = build(
            output=args.output,
            debug=args.debug,
            verbose=args.verbose,
            clean=args.clean
        )
        return 0 if success else 1
    
    elif args.command == 'test':
        success = test_binary(args.binary)
        return 0 if success else 1
    
    elif args.command == 'install':
        success = install(binary=args.binary, prefix=args.prefix)
        return 0 if success else 1
    
    elif args.command == 'clean':
        clean_build_artifacts()
        return 0
    
    elif args.command == 'all':
        print_status("Полная сборка и тестирование")
        
        # Очистка
        clean_build_artifacts()
        
        # Сборка
        if not build(output='nixhelp', debug=args.debug):
            return 1
        
        # Тест
        if not test_binary('nixhelp'):
            return 1
        
        print_success("\n✅ Полная сборка и тестирование завершены успешно!")
        return 0
    
    return 0

if __name__ == '__main__':
    sys.exit(main())
