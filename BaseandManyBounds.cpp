/*
#include <algorithm>
#include <cstdint>
#include <utility>
#include <vector>
*/

namespace getcracked {

    struct ProcessRegion {
        uint32_t pid;
        uint64_t base;
        uint64_t bounds;
    };
    
    class RegionChecker {
    public:
        void add_process(uint32_t pid, uint64_t base, uint64_t bounds) {
            processes_.push_back({pid, base, bounds});
        }
    
        std::vector<std::pair<uint32_t, uint32_t>> find_overlaps() const {
            std::vector<std::pair<uint32_t, uint32_t>> result;
            size_t size = processes_.size();
            for (size_t currPidInd = 0; currPidInd < size; currPidInd++) {
                for (size_t nextPidInd = currPidInd + 1; nextPidInd < size; nextPidInd++) {
                    if (overlaps(processes_[currPidInd], processes_[nextPidInd])) {
                        auto low = std::min(processes_[currPidInd].pid, processes_[nextPidInd].pid);
                        auto high = std::max(processes_[currPidInd].pid, processes_[nextPidInd].pid);
                        result.push_back({low, high});
                    }
                }
            }
            return result;
        }
    
    private:
        static bool overlaps(const ProcessRegion& a, const ProcessRegion& b) {
            if (a.bounds == 0 || b.bounds == 0) return false;
            return a.base < b.base + b.bounds
                && b.base < a.base + a.bounds;
        }
    
        std::vector<ProcessRegion> processes_;
    };
    
    } // namespace getcracked