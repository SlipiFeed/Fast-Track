#define LIST_WITHOUT_TAIL
#include "task3.h"

int main()
{
    SinglyLinkedList<int> myList{5,4,7,8,9,1,2,3,4,5,6,1};
    myList.print();
    myList.push_after(7, 6);
    myList.print();
    myList.push_after(333, 9);
    myList.print();
    myList.push_back(333);
    myList.print();
    myList.push_back(333);
    myList.print();
    myList.push_front(888);
    myList.print();
    myList.push_front(388833);
    myList.print();
    myList.push_after(388833, 6);
    myList.print();
    myList.push_after(388833, 777);
    myList.print();
    myList.push_after(333, 9);
    myList.print();
    std::cout << "========================================" << std::endl;
    myList.print();
    myList.remove(7);
    myList.print();
    myList.remove_all(333);
    myList.print();
    myList.remove_back();
    myList.print();
    myList.remove_back();
    myList.print();
    myList.remove_back();
    myList.print();
    myList.remove_front();
    myList.print();
    myList.remove_front();
    myList.print();
    myList.remove_front();
    myList.print();
    myList.remove_front();
    myList.print();
    return 0;
}