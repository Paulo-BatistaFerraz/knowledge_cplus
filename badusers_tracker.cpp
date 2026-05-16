/*
#include <unordered_map>
#include <deque>
*/
class BadUserTracker {
    public:
        const int WINDOW_LENGTH = 300; // 300 seconds
        const int REQUEST_THRESHOLD = 100; // 100 requests per 300 seconds
    
        void makeRequest(int userId, int timestamp) {
            auto& dq = requests_[userId]; // get the deque for the user
            dq.push_back(timestamp);
            // Prune entries that have fallen outside the window
            while (!dq.empty() && dq.front() <= timestamp - WINDOW_LENGTH)
                dq.pop_front();
        }
    
        int badUsers(int timestamp) {
            int count = 0;
            for (auto it = requests_.begin(); it != requests_.end(); ) { // iterate over the map
                auto& dq = it->second; // if the user has no requests, it will be an empty deque
                
                // dq -> second -> deque of timestamps, auto& dq = it->second; is a reference to the deque meaning second is
                while (!dq.empty() && dq.front() <= timestamp - WINDOW_LENGTH)
                    dq.pop_front();
                if (dq.empty()) {
                    // Remove user entirely to prevent unbounded memory growth
                    it = requests_.erase(it); // if the user has no requests, it will be erased from the map
                } else {
                    if (static_cast<int>(dq.size()) > REQUEST_THRESHOLD) // if the user has more than 100 requests, it will be counted as a bad user
                        ++count;
                    ++it;// move the the next user in the map
                }
            }
            return count;
        }
    
    private:
        std::unordered_map<int, std::deque<int>> requests_; // key -> user id, value -> deque of timestamps
    };