#include "misc.h"

int print_hello_world()
{
    std::cout << "main\n";
    return 1;
}

unsigned int Factorial(unsigned int number)
{
    return number <= 1 ? number : Factorial(number - 1) * number;
}
