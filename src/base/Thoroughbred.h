#ifndef BASE_THOROUGHBRED_H
#define BASE_THOROUGHBRED_H

#include <array>
#include "base/Properties.h"

namespace pedsearch {
namespace base {

class Thoroughbred {
protected:
    const std::string name_;
    const Pedigree pedigree_;
    const Blood blood_;

    Thoroughbred(std::string_view name, const Pedigree& pedigree, const Blood& blood) :
        name_(name), pedigree_(pedigree), blood_(blood) {}

public:
    std::string_view getName() const {
        return name_;
    }

    std::string_view getSireName(unsigned int generation) const {
        assertPrint(
            pedigree_.get(generation) != "",
            "Thoroughbred::getSireName: " + name_ + "'s " + std::to_string(generation) + "th sire name is empty"
        );
        return pedigree_.get(generation);
    }

    std::string_view getBloodType() const {
        return blood_.getType();
    }

    unsigned int getBloodIndex() const {
        return blood_.getIndex();
    }
};

}
}

#endif // BASE_THOROUGHBRED_H
