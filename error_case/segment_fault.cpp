#include <iostream>

using namespace std;

void case_1(void* ptr){
    if (ptr == nullptr){
        cout << "ptr is null" << endl;
        return;
    }
    delete static_cast<int*>(ptr);
    ptr = nullptr;
    cout << "ptr is delete!" << endl;
}

int main(){

    // case 1
    void* ptr = new int(10);
    case_1(ptr);
    case_1(ptr); // double free
    return 0;

}