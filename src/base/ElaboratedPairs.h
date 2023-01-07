#ifndef BASE_ELABORATEDPAIRS_H
#define BASE_ELABORATEDPAIRS_H

#include <set>
#include <string>
#include <unordered_map>

namespace pedsearch {
namespace base {

class ElaboratedPairs {
private:
    std::unordered_map<size_t, std::set<size_t> > map_;

public:
    ElaboratedPairs() {}

    void insert(size_t stallionSide, size_t broodmareSide) {
        map_[stallionSide].insert(broodmareSide);
    }

    bool hasPair(size_t stallionSide, size_t broodmareSide) const {
        auto it = map_.find(stallionSide);
        if (it == map_.end()) {
            return false;
        } else {
            auto it2 = (*it).second.find(broodmareSide);
            if (it2 == (*it).second.end()) {
                return false;
            } else {
                return true;
            }
        }
    }
};

}
}

#endif // BASE_ELABORATEDPAIRS_H
