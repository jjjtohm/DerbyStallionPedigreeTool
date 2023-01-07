#ifndef BASE_BROODMARE_H
#define BASE_BROODMARE_H

#include "base/Thoroughbred.h"

namespace pedsearch {
namespace base {

class Broodmare : public Thoroughbred {
private:
public:
    Broodmare(std::string_view name, const Pedigree& pedigree, const Blood& blood) :
        Thoroughbred(name, pedigree, blood) {}
};

}
}

#endif // BASE_BROODMARE_H
