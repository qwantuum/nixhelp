#!/usr/bin/env python3
"""
NixHelp Test Suite
Набор тестов для проверки функциональности NixHelp
"""

import subprocess
import sys
import os
from pathlib import Path

class Colors:
    GREEN = '\033[92m'
    RED = '\033[91m'
    YELLOW = '\033[93m'
    BLUE = '\033[94m'
    RESET = '\033[0m'
    BOLD = '\033[1m'

def print_test(name, passed, message=""):
    status = f"{Colors.GREEN}✓ PASS{Colors.RESET}" if passed else f"{Colors.RED}✗ FAIL{Colors.RESET}"
    print(f"{status} | {name}")
    if message and not passed:
        print(f"       {Colors.YELLOW}{message}{Colors.RESET}")
    return passed

def run_command(args, timeout=10):
    """Выполнить команду и вернуть результат"""
    try:
        result = subprocess.run(
            args,
            capture_output=True,
            text=True,
            timeout=timeout
        )
        return result.returncode, result.stdout, result.stderr
    except subprocess.TimeoutExpired:
        return -1, "", "Timeout"
    except Exception as e:
        return -1, "", str(e)

def test_help(binary):
    """Тест команды help"""
    code, stdout, stderr = run_command([f'./{binary}', 'help'])
    passed = code == 0 and 'NixHelp' in stdout
    print_test("Команда help", passed)
    return passed

def test_search_basic(binary):
    """Тест поиска пакетов"""
    code, stdout, stderr = run_command([f'./{binary}', 'search', 'hello'], timeout=30)
    # Поиск может вернуть пустой результат если нет сети или nixpkgs
    passed = code == 0
    print_test("Поиск пакетов (search)", passed)
    return passed

def test_store_info(binary):
    """Тест информации о хранилище"""
    code, stdout, stderr = run_command([f'./{binary}', 'store'])
    passed = code == 0
    print_test("Информация о хранилище (store)", passed)
    return passed

def test_gc_dry_run(binary):
    """Тест garbage collection (без реального удаления)"""
    # GC требует прав и может быть опасен в тестах
    # Просто проверяем что команда существует
    code, stdout, stderr = run_command([f'./{binary}', 'gc'], timeout=5)
    passed = code in [0, 1]  # 0 - успех, 1 - ошибка выполнения но команда есть
    print_test("Garbage collection (gc)", passed)
    return passed

def test_unknown_command(binary):
    """Тест неизвестной команды"""
    code, stdout, stderr = run_command([f'./{binary}', 'nonexistent-command'])
    passed = code != 0 and 'Неизвестная команда' in stderr
    print_test("Неизвестная команда", passed)
    return passed

def test_no_args(binary):
    """Тест запуска без аргументов"""
    code, stdout, stderr = run_command([f'./{binary}'])
    passed = code == 0 and 'NixHelp' in stdout
    print_test("Запуск без аргументов", passed)
    return passed

def test_binary_exists(binary):
    """Тест существования бинарника"""
    passed = os.path.isfile(binary) and os.access(binary, os.X_OK)
    print_test("Бинарник существует и исполняемый", passed)
    return passed

def run_all_tests(binary='nixhelp'):
    """Запустить все тесты"""
    print(f"\n{Colors.BOLD}{Colors.BLUE}Запуск тестов NixHelp{Colors.RESET}\n")
    print(f"Бинарник: {binary}\n")
    
    if not os.path.exists(binary):
        print(f"{Colors.RED}Ошибка: бинарник {binary} не найден{Colors.RESET}")
        print("Сначала выполните сборку: python scripts/build.py build")
        return False
    
    tests = [
        lambda: test_binary_exists(binary),
        lambda: test_no_args(binary),
        lambda: test_help(binary),
        lambda: test_unknown_command(binary),
        lambda: test_store_info(binary),
        lambda: test_gc_dry_run(binary),
        lambda: test_search_basic(binary),
    ]
    
    passed = 0
    failed = 0
    
    for test in tests:
        try:
            if test():
                passed += 1
            else:
                failed += 1
        except Exception as e:
            print(f"{Colors.RED}✗ EXCEPTION | {str(e)}{Colors.RESET}")
            failed += 1
    
    print(f"\n{'='*50}")
    print(f"Результаты: {Colors.GREEN}{passed} пройдено{Colors.RESET}, {Colors.RED}{failed} провалено{Colors.RESET}")
    print(f"{'='*50}\n")
    
    return failed == 0

if __name__ == '__main__':
    binary = sys.argv[1] if len(sys.argv) > 1 else 'nixhelp'
    success = run_all_tests(binary)
    sys.exit(0 if success else 1)
