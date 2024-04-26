#include <iostream>
#include <map>
#include <mutex>
#include <stdexcept>
#include <limits>
#include <memory>

/***
 * Напишите класс на C++, который реализует простую систему управления базой данных студентов. Каждый студент представлен структурой Student, содержащей следующие поля: 
 * id (целочисленный идентификатор), name (строка с именем студента), и age (целочисленный возраст студента). 
 * Ваш класс должен обеспечивать следующие возможности:
 * - Добавление нового студента в базу данных. 
 * - Удаление студента по идентификатору.
 * - Получение информации о студенте по идентификатору. 
 * - Используйте умный указатель std::shared_ptr для хранения и управления объектами студентов в контейнере внутри класса (например, std::vector или std::map). 
*/


struct Student 
{
    int id;
    std::string name;
    int age;
};

class StudentDataBase 
{
private:
    //mutex для потоков в БД, которые могут писать/читать одновременно
    std::mutex mutex;
    std::map<int, std::shared_ptr<Student>> students;

public:
    void add_student(const int& id, const std::string& name, const int& age) 
    {
        if (age < 0 || id < 0)
        {
            throw std::invalid_argument("Student age or id is less then zero.");
        }
        if (name.empty() == true)
        {
            throw std::invalid_argument("Student with empty name");
        }
        if (students.count(id) != 0)
        {
            throw std::invalid_argument("Student id is already used.");
        }
        std::lock_guard<std::mutex> lock(mutex);
        students.emplace(std::pair{ id, std::make_shared<Student>(Student{id, name, age}) });
    }

    void remove_student(const int& id)
    {
        if (id < 0 || students.count(id) == 0)
        {
            throw std::invalid_argument("Wront student id.");
        }
        std::lock_guard<std::mutex> lock(mutex);
        students.erase(id);
    }

    std::shared_ptr<Student> get_info(const int& id)
    {
        if (id < 0 || students.count(id) == 0)
        {
            throw std::invalid_argument("Wront student id.");
        }
        std::lock_guard<std::mutex> lock(mutex);
        return students[id];
    }
};

int main() {
    StudentDataBase DB;
    DB.add_student(1, "Ilya", 23);
    DB.add_student(2, "Ilya", 23); 
    DB.add_student(3, "Ilya", 23);
    DB.remove_student(3);
    std::shared_ptr<Student> student = DB.get_info(2);
    std::cout << "student name: " << student->name << std::endl;
    return 0;
}