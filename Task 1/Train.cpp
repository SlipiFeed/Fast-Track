#include <iostream>
#include <cstdint>
#include <algorithm>
#include <random>
#include <ctime>
#include <limits>
#include <bitset>
#include <memory>

/**
 * Напишите функцию, которая инвертирует только выбранные биты в участке памяти, 
 * заданном указателем и длиной в байтах. Способ выбора битов для инвертирования 
 * на ваше усмотрение.
*/

#define NUMERIC_TYPE uint8_t

template <class T>
T generateNumber()
{
    static std::mt19937 generator(std::time(0));
    static std::uniform_int_distribution<T> dis(0, std::numeric_limits<T>::max());
    return dis(generator);
}

template <class T>
std::string makeBinary(T number) {
    std::string binary = std::bitset<64>(number).to_string();
    binary.erase(0, binary.find_first_not_of('0'));
    return binary;
}

template <class T>
void invert(void* ptr, const size_t length) 
{
    T inverting_bits = generateNumber<T>();
    std::cout << "Inverting bits: " << makeBinary<T>(inverting_bits) << std::endl;
    T* u_ptr = reinterpret_cast<T*> (ptr);
    for (size_t i = 0; i < length; ++i) 
    {
       u_ptr[i] ^= inverting_bits;
    }
}

template <class T>
void printData(const T* data, const size_t size)
{
    std::cout << "Data: ";
    for(size_t i = 0; i < size; i++)
    {
        std::cout << makeBinary<T>(data[i]) << " ";
    }
    std::cout << std::endl;
}

int main() {
    static_assert(std::is_arithmetic<NUMERIC_TYPE>::value == true,  "Invert function requires integral type.");

    const size_t dataSize = 5;
    auto data = std::make_unique<NUMERIC_TYPE[]>(dataSize);
    
    for (size_t i = 0; i < dataSize; i++)
    {
        data[i] = generateNumber<NUMERIC_TYPE>();
    }

    printData<NUMERIC_TYPE>(data.get(), dataSize);
    invert<NUMERIC_TYPE>(data.get(), dataSize);
    printData<NUMERIC_TYPE>(data.get(), dataSize);
    
    return 0;
}