#ifndef BASE_THOROUGHBREDMAP_H
#define BASE_THOROUGHBREDMAP_H

#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include "base/Debug.h"
#include "base/Broodmare.h"
#include "base/DefaultStallion.h"
#include "base/Stallion.h"
#include "base/Thoroughbred.h"

namespace pedsearch {
namespace base {

template <class T> class ThoroughbredMap {
private:
    std::unordered_map<std::string, T> map_;

public:
    ThoroughbredMap() {
        assertPrint(
            (bool)(std::is_base_of<Thoroughbred, T>::value),
            "ThoroughbredMap::ThoroughbredMap: not derived from Thoroughbred."
        );
    }

    const T& at(std::string_view name) const {
        assertPrint(
            map_.find(name.data()) != map_.end(),
            "ThoroughbredMap::at: " + std::string(name) + " does not exist."
        );
        return map_.at(name.data());
    }

    void insert(const T& x) {
        map_.insert(std::make_pair(x.getName(), x));
    }

    void getKeys(std::set<std::string_view>& set) const {
        set.clear();
        for (auto it = map_.begin(); it != map_.end(); ++it) {
            set.insert((*it).first);
        }
    }
};

}
}

#endif // BASE_THOROUGHBREDMAP_H
