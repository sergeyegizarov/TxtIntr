#include <cmath>
#include <cstdlib>
#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// Функция вывода справки по использованию программы
void printHelp(const char* progName)
{
    std::cout << "Мультипликативный калькулятор (Вариант 4)\n";
    std::cout << "Использование:\n";
    std::cout << "  " << progName << " -o <операция> <операнд1> <операнд2> <операнд3> <операнд4> <операнд5>\n";
    std::cout << "  " << progName << " --operation <операция> <число1> <число2> <число3> ...\n\n";
    std::cout << "Параметры:\n";
    std::cout << "  -o, --operation <имя>   Задать операцию:\n";
    std::cout << "                            mult / multiply  — Умножение всех чисел\n ";
	std::cout << "                            div  / divide    — Деление первого числа на остальные\n";
    std::cout << "  -h, --help              Вывести эту справку\n\n";
    std::cout << "Ограничение на операнды:\n";
    std::cout << "  Количество позиционных операндов должно быть от 3 до 5.\n";
    std::cout << "Примеры:\n";
    std::cout << "  " << progName << " -o mult 2 3 4\n";
    std::cout << "  " << progName << " --operation divide 100 2 5 2\n";
}

int main(int argc, char* argv[])
{
    // Если программа запущена без аргументов
    if(argc == 1) {
        printHelp(argv[0]);
        return 0;
    }

    std::string operation = "";

    // Структура длинных опций для getopt_long
    static struct option long_options[] = { { "operation", required_argument, 0, 'o' },
                                            { "help", no_argument, 0, 'h' },
                                            { 0, 0, 0, 0 } };

    int opt;
    int option_index = 0;

    while((opt = getopt_long(argc, argv, "o:h", long_options, &option_index)) != -1) {
        switch(opt) {
        case 'o':
            operation = optarg;
            break;
        case 'h':
            printHelp(argv[0]);
            return 0;
        case '?':
            // Неизвестный параметр или пропущен аргумент
            std::cerr << "Ошибка: Некорректные параметры командной строки.\n";
            std::cerr << "Используйте " << argv[0] << " -h для справки.\n";
            return 1;
        default:
            break;
        }
    }

    // Проверка, была ли указана операция
    if(operation.empty()) {
        std::cerr << "Ошибка: Не указана операция. Используйте параметр -o или --operation.\n";
        std::cerr << "Используйте " << argv[0] << " -h для справки.\n";
        return 1;
    }

    // Сбор позиционных аргументов (операндов)
    std::vector<double> operands;
    for(int i = optind; i < argc; ++i) {
        char* endptr;
        double val = std::strtod(argv[i], &endptr);

        // Проверка корректности преобразования строки в число
        if(*endptr != '\0') {
            std::cerr << "Ошибка: Некорректный операнд '" << argv[i] << "'. Операнды должны быть числами.\n";
            return 1;
        }
        operands.push_back(val);
    }

    // Проверка количества операндов (по ТЗ: от 3 до 5)
    if(operands.size() < 3 || operands.size() > 5) {
        std::cerr << "Ошибка: Неверное количество операндов (" << operands.size() << ").\n";
        std::cerr << "Мультипликативный калькулятор принимает от 3 до 5 операндов.\n";
        return 1;
    }

    // Выполнение операции
    if(operation == "mult" || operation == "multiply") {
        double result = 1.0;
        for(double val : operands) {
            result *= val;
        }
        std::cout << "Результат умножения: " << result << std::endl;
    } else if(operation == "div" || operation == "divide") {
        double result = operands[0];

        // Проверка деления на ноль
        for(size_t i = 1; i < operands.size(); ++i) {
            if(operands[i] == 0.0) {
                std::cerr << "Ошибка: Деление на ноль (операнд #" << (i + 1) << " равен 0).\n";
                return 1;
            }
            result /= operands[i];
        }
        std::cout << "Результат деления: " << result << std::endl;
    } else {
        std::cerr << "Ошибка: Неизвестная операция '" << operation << "'.\n";
        std::cerr << "Поддерживаемые операции: mult, multiply, div, divide.\n";
        return 1;
    }

    return 0;
}