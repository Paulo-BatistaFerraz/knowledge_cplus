#include <iostream>
#include <unordered_map>
#include <deque>

std::unordered_map<int, std::deque<int>> requests_; // key -> user id, value -> deque of timestamps

int main() {
    requests_[1].push_back(100);
    requests_[1].push_back(200);
    requests_[2].push_back(300);


    if (!requests_[101].empty()) {
        int req = requests_[101].front(); 
        requests_[101].pop_front();
        std::cout << "Request: " << req << std::endl;
    }

    for (const auto& [user, timestamps] : requests_) { // O(1) time complexity for each operation
        std::cout << "User: " << user << " has " << timestamps.size() << " requests" << std::endl;
        std::cout << std::endl;
        for (const auto& timestamp : timestamps) {
            std::cout << "Timestamp: " << timestamp << std::endl;
        }
        std::cout << std::endl;
        
    }

    return 0;
}