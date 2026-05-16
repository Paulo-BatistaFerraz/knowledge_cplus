#include <iostream>
#include <string>
#include <vector>

template <typename ... Mixins>
struct OHLC: public Mixins... {
    double Open, High, Low, Close;
};

struct Label { std::string Text;};
struct Color { int Alpha;};

using LibOHLC = OHLC<Label, Color>;

struct Chart {
    private:
    std::vector<LibOHLC> openHighLowCloses_;
    public:
        Chart(std::initializer_list<LibOHLC> ohlcs) : openHighLowCloses_{ ohlcs } { }
        std::string GetFirstLabel() const {
            return openHighLowCloses_.size()
            ? openHighLowCloses_.front().Text
            : "";
        }
        int GetFirstAlpha() const {
            return openHighLowCloses_.size()
            ? openHighLowCloses_.front().Alpha
            : -1;
        }
};

#include <map>

struct MapOHLC: OHLC<std::map<std::string, double>> {
    MapOHLC(std::initializer_list<std::pair<std::string, double>> values) : OHLC<std::map<std::string, double>>{ values } { }

    std::string GetFirstKey() const {
        return OHLC<std::map<std::string, double>>::data_.begin()->first;
    }
    double GetFirstValue() const {
        return OHLC<std::map<std::string, double>>::data_.begin()->second;
    }
};

int main() {
    Chart chart{
        LibOHLC{ {"Open"}, {100}, 1.0, 2.0, 3.0, 4.0 },
        LibOHLC{ {"Close"}, {200}, 5.0, 6.0, 7.0, 8.0 }
    };
    std::cout << chart.GetFirstLabel() << std::endl;
    std::cout << chart.GetFirstAlpha() << std::endl;
}