#include <iostream>
#include <vector>
#include <string>
#include <initializer_list>

struct Item {
    std::string name;
    int qty;
};

struct Inventory {
    private:
    std::vector<Item> items_;
    public:
        Inventory(std::initializer_list<Item> items): items_ { items } { };
        std::size_t GetSize() const { return items_.size(); }
        std::string firstName() const { return items_.empty() ? "" : items_.front().name; }
};



int main() {
    Inventory inv{
        Item{"Apple", 10},
        Item{"Banana", 20},
        Item{"Cherry", 30}
    };

    std::cout << inv.GetSize() << std::endl; // result: 3
    std::cout << inv.firstName() << std::endl; // result: Apple
    //second name
    std::cout << inv.secondName() << std::endl; // result: Banana
    

}
