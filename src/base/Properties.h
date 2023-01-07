#ifndef BASE_PROPERTIES_H
#define BASE_PROPERTIES_H

#include <map>
#include <string>
#include <string_view>
#include "base/Debug.h"

namespace pedsearch {
namespace base {

class Pedigree {
private:
    const std::string sire_[4]; // 父, 母父, 母母父, 母母母父
public:
    Pedigree(
        std::string_view sire1, std::string_view sire2,
        std::string_view sire3, std::string_view sire4
    ) : sire_{std::string(sire1), std::string(sire2),std::string(sire3), std::string(sire4)} {}

    std::string_view get(unsigned int generation) const {
        assertPrint(
            generation <= 3,
            "Pedigree::get: generation must be lower than 4 but got " + std::to_string(generation)
        );
        return sire_[generation];
    }
};

class Blood {
private:
    static inline const std::map<unsigned int, std::string> indexToType_ = {
        {0, ""}, {1, "Ec"}, {2, "Ph"}, {3, "Ns"}, {4, "Ro"}, {5, "Ne"},
        {6, "Na"}, {7, "Fa"}, {8, "To"}, {9, "Te"}, {10, "Sw"}, {11, "Ha"},
        {12, "Hi"}, {13, "St"}, {14, "Ma"}, {15, "He"}
    };

    static inline const std::map<std::string, unsigned int> typeToIndex_ = {
        {"", 0}, {"Ec", 1}, {"Ph", 2}, {"Ns", 3}, {"Ro", 4}, {"Ne", 5},
        {"Na", 6}, {"Fa", 7}, {"To", 8}, {"Te", 9}, {"Sw", 10}, {"Ha", 11},
        {"Hi", 12}, {"St", 13}, {"Ma", 14}, {"He", 15}
    };

    const std::string type_;
    const unsigned int index_;

public:
    Blood(unsigned int index) : type_(indexToType_.at(index)), index_(index) {}

    Blood(std::string_view type) : type_(type), index_(typeToIndex_.at(type.data())) {}

    std::string_view getType() const { return type_; }

    unsigned int getIndex() const { return index_; }
};

class Distance {
private:
    unsigned int min_;
    unsigned int max_;
public:
    Distance(unsigned int min, unsigned int max) : min_(min), max_(max) {}
    unsigned int getMin() const { return min_; }
    unsigned int getMax() const { return max_; }
};

enum class Growth {
    PRECOCIOUS, NORMAL, PERSISTENT, ALTRICAL, UNKNOWN
};

enum class Grade {
    A, B, C, UNKNOWN
};

enum class Dirt {
    GOOD, NORMAL, BAD, UNKNOWN
};

class BloodEffect {
private:
    bool sprint_;
    bool speed_;
    bool stamina_;
    bool spirit_;
    bool stable_;
    bool temper_;
    bool precocious_;
    bool altrical_;
    bool tough_;
    bool dirt_;
    bool power_;
public:
    BloodEffect(
        bool sprint=false, bool speed=false, bool stamina=false, bool spirit=false,
        bool stable=false, bool temper=false, bool precocious=false, bool altrical=false,
        bool tough=false, bool dirt=false, bool power=false
    ) :
    sprint_(sprint), speed_(speed), stamina_(stamina), spirit_(spirit), stable_(stable),
    temper_(temper), precocious_(precocious), altrical_(altrical), tough_(tough), dirt_(dirt), power_(power) {}

    bool isSprint() const { return sprint_; }
    bool isSpeed() const { return speed_; }
    bool isStamina() const { return stamina_; }
    bool isSpirit() const { return spirit_; }
    bool isStable() const { return stable_; }
    bool isTemper() const { return temper_; }
    bool isPrecocious() const { return precocious_; }
    bool isAltrical() const { return altrical_; }
    bool isTough() const { return tough_; }
    bool isDirt() const { return dirt_; }
    bool isPower() const { return power_; }
};

}
}

#endif // BASE_PROPERTIES_H
