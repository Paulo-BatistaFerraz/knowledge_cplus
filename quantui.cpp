#include <iostream>
#include <string>
#include <vector>
#include <intializer_list>


template<typename... Mixins>
struct OHLC : public Mixins... {
    double Open, High, Low, Close;
};

struct Label { std::string text; };
struct Color { std::string color; };
