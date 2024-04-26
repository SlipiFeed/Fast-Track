#include <iostream>

/***
 * Массив int[10], взять указатель (тип можно варировать) на первый элемент и с помощью арифметики указателей вывести 4 элемент массива.
*/

int main() 
{
    int intArray[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int* firstElementPtr = intArray;
    std::cout << "1-й элемент массива: " << *firstElementPtr << std::endl;
    std::cout << "4-й элемент массива: " << *(firstElementPtr + 3) << std::endl;
    return 0;
}