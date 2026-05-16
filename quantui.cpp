#include <iostream>
#include <string>
#include <vector>
#include <initializer_list>

template <typename... Mixins>
struct OHLC : public Mixins... {
    double Open, High, Low, Close;
};


struct Label { std::string Text; };
struct Color { int Alpha;       };

using LibOHLC = OHLC<Label, Color>;

struct Chart {
private:
    std::vector<LibOHLC> openHighLowCloses_;
public:
    Chart(std::initializer_list<LibOHLC> ohlcs) : openHighLowCloses_{ ohlcs } { }

    std::string GetFirstLabel() const {
        return openHighLowCloses_.size()
            ? openHighLowCloses_.at(0).Text
            : "";
    }

    int GetFirstAlpha() const {
        return openHighLowCloses_.size()
            ? openHighLowCloses_.at(0).Alpha
            : -1;
    }
};

int main() {
    auto ohlcs = {
        LibOHLC{ Label{ "gc"   }, Color{ 3 }, 1, 2, 3, 4 },
        LibOHLC{ Label{ "tttt" }, Color{ 4 }, 1, 2, 3, 4 },
        LibOHLC{ Label{ "mmm"  }, Color{ 5 }, 1, 2, 3, 4 }
    };

    Chart c(ohlcs);

    std::cout << "First label: " << c.GetFirstLabel() << '\n';
    std::cout << "First alpha: " << c.GetFirstAlpha() << '\n';
    return 0;
}