#ifndef BASE_STALLION_H
#define BASE_STALLION_H

#include "base/Thoroughbred.h"

namespace pedsearch {
namespace base {

class Stallion : public Thoroughbred {
private:
    const BloodEffect effect_;

public:
    Stallion(
        std::string_view name, const Pedigree& pedigree, const Blood& blood, BloodEffect effect
    ) :
        Thoroughbred(name, pedigree, blood), effect_(effect) {}

    bool isSprint() const { return effect_.isSprint(); }
    bool isSpeed() const { return effect_.isSpeed(); }
    bool isStamina() const { return effect_.isStamina(); }
    bool isSpirit() const { return effect_.isSpirit(); }
    bool isStable() const { return effect_.isStable(); }
    bool isTemper() const { return effect_.isTemper(); }
    bool isPrecocious() const { return effect_.isPrecocious(); }
    bool isAltrical() const { return effect_.isAltrical(); }
    bool isTough() const { return effect_.isTough(); }
    bool isDirt() const { return effect_.isDirt(); }
    bool isPower() const { return effect_.isPower(); }
};

}
}

#endif // BASE_STALLION_H
