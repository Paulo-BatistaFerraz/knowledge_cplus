#include <iostream>
#include <string>
#include <vector>
#include <intializer_list>

template <typename... Mixins>
struct OHLC : public Mixins... {
    double Open, High, Low, Close;
};

struct Label { std::string text; };
struct Color { std::string color; };

int main() {
    OHLC<Label, Color> ohlc = {1.0, 2.0, 3.0, 4.0, "Open", "Close", "Red", "Green"};
    std::cout << ohlc.text << std::endl;
    std::cout << ohlc.color << std::endl;
}