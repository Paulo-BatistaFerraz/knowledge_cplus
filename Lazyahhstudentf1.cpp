#include <iostream>
using namespace std;

int main(){
    int n;
    do {
        std::cout << "Enter a number between 1 and 10: ";
        std::cin >> n;
    } while (n < 1 || n > 10);

}