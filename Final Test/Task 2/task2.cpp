#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

/***
 * Разработайте программу, демонстрирующую создание двух потоков в C++, где один поток пишет данные в переменную, 
 * а другой читает из нее, с использованием мьютексов для синхронизации доступа.
*/


void write(int& value, std::mutex& mtx)
{
    while(true)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            value += 1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}


void read(int& value, std::mutex& mtx) 
{
    while(true)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Value : " << value << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}

int main() {
    int value = 0;
    std::mutex mtx;
    std::thread writer(write, std::ref(value), std::ref(mtx));
    std::thread reader(read, std::ref(value), std::ref(mtx));

    writer.join();
    reader.join();
    return 0;
}