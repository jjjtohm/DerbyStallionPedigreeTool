#ifndef BASE_DEFAULTSTALLION_H
#define BASE_DEFAULTSTALLION_H

#include <set>
#include <string>
#include <vector>
#include "base/Debug.h"
#include "base/Properties.h"

namespace pedsearch {
namespace base {

class Index {
private:
    unsigned int index_;
public:
    Index(unsigned int index) : index_(index) {
        assertPrint(
            index <= 15,
            "Index::Index: index must be lower than 16, but got " + std::to_string(index));
    }

    operator unsigned int() const {
        return index_;
    }

    bool operator==(Index i) const {
        return index_ == i.index_;
    }

    bool operator!=(Index i) const {
        return index_ != i.index_;
    }

    bool operator<(Index i) const {
        return index_ < i.index_;
    }

    bool operator<=(Index i) const {
        return index_ <= i.index_;
    }

    bool operator>(Index i) const {
        return index_ > i.index_;
    }

    bool operator>=(Index i) const {
        return index_ >= i.index_;
    }
};

class DefaultStallion {
private:
    const size_t ancestors_[16];
    const unsigned int indices_[8];
    const unsigned int fee_;
    const Distance dist_;
    const Growth growth_;
    const Dirt dirt_;
    const Grade health_;
    const Grade temper_;
    const Grade achievement_;
    const Grade spirit_;
    const Grade stable_;

public:
    DefaultStallion(
        size_t* ancestors, unsigned int* indices) :
        ancestors_{
            ancestors[0], ancestors[1], ancestors[2], ancestors[3], ancestors[4], ancestors[5],
            ancestors[6], ancestors[7], ancestors[8], ancestors[9], ancestors[10], ancestors[11],
            ancestors[12], ancestors[13], ancestors[14], ancestors[15]
        },
        indices_{
            indices[0], indices[1], indices[2], indices[3],
            indices[4], indices[5], indices[6], indices[7]
        },
        fee_(0), dist_(Distance(0, 0)), growth_(Growth::UNKNOWN), dirt_(Dirt::UNKNOWN),
        health_(Grade::UNKNOWN), temper_(Grade::UNKNOWN), achievement_(Grade::UNKNOWN),
        spirit_(Grade::UNKNOWN), stable_(Grade::UNKNOWN) {}

    DefaultStallion(
        size_t* ancestors, unsigned int* indices, unsigned int fee,
        Distance dist, Growth growth, Dirt dirt, Grade health, Grade temper,
        Grade achievement, Grade spirit, Grade stable
    ) :
        ancestors_{
            ancestors[0], ancestors[1], ancestors[2], ancestors[3], ancestors[4], ancestors[5],
            ancestors[6], ancestors[7], ancestors[8], ancestors[9], ancestors[10], ancestors[11],
            ancestors[12], ancestors[13], ancestors[14], ancestors[15]
        },
        indices_{
            indices[0], indices[1], indices[2], indices[3],
            indices[4], indices[5], indices[6], indices[7]
        },
        fee_(fee), dist_(dist), growth_(growth), dirt_(dirt), health_(health), temper_(temper),
        achievement_(achievement), spirit_(spirit), stable_(stable) {}

    size_t getAncestorIndex(Index index) const {
        return ancestors_[index];
    }

    void appendInterestingIndices(std::set<unsigned int>& indices) const {
        indices.insert(indices_[0]);
        indices.insert(indices_[2]);
        indices.insert(indices_[4]);
        indices.insert(indices_[6]);
    }

    void appendWonderfulIndices(std::set<unsigned int>& indices) const {
        indices.insert(indices_[1]);
        indices.insert(indices_[3]);
        indices.insert(indices_[5]);
        indices.insert(indices_[7]);
    }

    std::vector<unsigned int> getInterestingIndices() const {
        std::vector<unsigned int> indices = {indices_[0], indices_[2], indices_[4], indices_[6]};
        return indices;
    }

    unsigned int getFee() const { return fee_; }
    unsigned int getMinDistance() const { return dist_.getMin(); }
    unsigned int getMaxDistance() const { return dist_.getMax(); }
    Growth getGrowth() const { return growth_; }
    Dirt getDirt() const { return dirt_; }
    Grade getHealth() const { return health_; }
    Grade getTemper() const { return temper_; }
    Grade getAchievement() const { return achievement_; }
    Grade getSpirit() const { return spirit_; }
    Grade getStable() const { return stable_; }
};

class DefaultBroodmare {
private:
    const size_t ancestors_[16];
    const unsigned int indices_[4];
    const unsigned int fee_;
    const unsigned int speed_;
    const unsigned int stamina_;
    const unsigned int power_;
    const Dirt dirt_;

public:
    DefaultBroodmare(
        size_t* ancestors, unsigned int* indices
    ) :
        ancestors_{
            ancestors[0], ancestors[1], ancestors[2], ancestors[3], ancestors[4], ancestors[5],
            ancestors[6], ancestors[7], ancestors[8], ancestors[9], ancestors[10], ancestors[11],
            ancestors[12], ancestors[13], ancestors[14], ancestors[15]
        },
        indices_{
            indices[0], indices[1], indices[2], indices[3]
        },
        fee_(0), speed_(0), stamina_(0), power_(0), dirt_(Dirt::UNKNOWN) {}

    DefaultBroodmare(
        size_t* ancestors, unsigned int* indices, unsigned int fee,
        unsigned int speed, unsigned int stamina, unsigned int power, Dirt dirt
    ) :
        ancestors_{
            ancestors[0], ancestors[1], ancestors[2], ancestors[3], ancestors[4], ancestors[5],
            ancestors[6], ancestors[7], ancestors[8], ancestors[9], ancestors[10], ancestors[11],
            ancestors[12], ancestors[13], ancestors[14], ancestors[15]
        },
        indices_{
            indices[0], indices[1], indices[2], indices[3]
        },
        fee_(fee), speed_(speed), stamina_(stamina), power_(power), dirt_(dirt) {}

    size_t getAncestorIndex(Index index) const {
        assertPrint(index != Index(0), "DefaultStallion::getAncestorIndex: 0 is invalid for index.");
        return ancestors_[index];
    }

    void appendInterestingIndices(std::set<unsigned int>& indices) const {
        indices.insert(indices_[0]);
        indices.insert(indices_[1]);
        indices.insert(indices_[2]);
        indices.insert(indices_[3]);
    }

    std::vector<unsigned int> getInterestingIndices() const {
        std::vector<unsigned int> indices = {indices_[0], indices_[1], indices_[2], indices_[3]};
        return indices;
    }

    unsigned int getFee() const { return fee_; }
    unsigned int getSpeed() const { return speed_; }
    unsigned int getStamina() const { return stamina_; }
    unsigned int getPower() const { return power_; }
    Dirt getDirt() const { return dirt_; }
};

}
}

#endif // BASE_DEFAULTSTALLION_H
