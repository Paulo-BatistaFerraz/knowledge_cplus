#include <iostream>
using namespace std;

struct Trade {
    std::string symbol;
    int quantity;
    double price;
    Trade(std::string s, int q, double p):
        symbol(std::move(s)), quantity(q), price(p) {}
};

int main(){
    std::vector<Trade> trades = {Trade("AAPL", 100, 1150.25), Trade("GOOG", 200, 2800.75), Trade("MSFT", 150, 3200.50)};
    for (const Trade& trade : trades) {
        std::cout << trade.symbol << " " << trade.quantity << " " << trade.price << std::endl;
    }
    return 0;
}