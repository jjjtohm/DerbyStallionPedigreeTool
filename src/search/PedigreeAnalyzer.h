#ifndef SEARCH_PEDIGREEANALYZER_H
#define SEARCH_PEDIGREEANALYZER_H

#include <algorithm>
#include <array>
#include <cmath>
#include <initializer_list>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>
#include "base/Debug.h"
#include "base/Broodmare.h"
#include "base/DefaultStallion.h"
#include "base/ElaboratedPairs.h"
#include "base/Stallion.h"
#include "base/ThoroughbredMap.h"

namespace pedsearch {
namespace search {

class Cross {
private:
    friend class PedigreeAnalyzer;
    friend class PedigreeAnalysis;
    std::unordered_map<size_t, std::vector<unsigned int> > crosses_;

    Cross() {}

    void append(size_t id, unsigned int generation) {
        if (crosses_.find(id) == crosses_.end()) {
            crosses_.emplace(id, std::vector<unsigned int>(1, generation));
        } else {
            crosses_[id].push_back(generation);
        }
    }

public:
    unsigned int getNumCrosses() const {
        return (unsigned int)crosses_.size();
    }

    void getCrossIndices(std::set<size_t>& crossIndices) const {
        crossIndices.clear();
        for (auto it = crosses_.begin(); it != crosses_.end(); ++it) {
            crossIndices.insert((*it).first);
        }
    }

    std::vector<unsigned int> getGenerations(size_t id) const {
        assertPrint(
            crosses_.find(id) != crosses_.end(),
            "Cross::getGenerations: no cross of " + std::to_string(id)
        );
        return crosses_.at(id);
    }

    double getBloodVolume(size_t id) const {
        assertPrint(
            crosses_.find(id) != crosses_.end(),
            "Cross::getBloodVolume: no cross of " + std::to_string(id)
        );
        std::vector<unsigned int> gens = crosses_.at(id);
        double sum = 0;
        for (auto it = gens.begin(); it != gens.end(); ++it) {
            sum += 50.0 / (*it);
        }
        return sum;
    }

    bool hasCross(size_t id) {
        if (crosses_.find(id) == crosses_.end()) {
            return false;
        } else {
            return true;
        }
    }
};

class Nitro {
private:
    friend class PedigreeAnalyzer;
    friend class PedigreeAnalysis;
    unsigned int sprint_;
    unsigned int speed_;
    unsigned int stamina_;
    unsigned int spirit_;
    unsigned int power_;

    Nitro() : sprint_(0), speed_(0), stamina_(0), spirit_(0), power_(0) {}

public:
    int getSpeedNitro() const {
        return 2 * sprint_ + speed_;
    }

    int getStaminaNitro() const {
        return stamina_ + spirit_ - sprint_;
    }

    int getPowerNitro() const {
        return power_;
    }
};

class PedigreeAnalysis {
private:
    friend class PedigreeAnalyzer;
    bool isInteresting_;
    bool isWonderful_;
    bool isElaborated_;
    Cross cross_;
    Nitro nitro_;

    PedigreeAnalysis() : isInteresting_(false), isWonderful_(false), isElaborated_(false) {}

public:
    bool isInteresting() const {
        return isInteresting_;
    }

    bool isWonderful() const {
        return isWonderful_;
    }

    bool isElaborated() const {
        return isElaborated_;
    }

    const Cross& getCross() const {
        return cross_;
    }

    const Nitro& getNitro() const {
        return nitro_;
    }
};

class PedigreeAnalyzer {
private:
    static inline unsigned int indexToGeneration(base::Index index) {
        switch (index) {
            case 0:
                return 1;
            case 1:
                return 2;
            case 2:
            case 9:
                return 3;
            case 3:
            case 6:
            case 10:
            case 13:
                return 4;
            case 4:
            case 5:
            case 7:
            case 8:
            case 11:
            case 12:
            case 14:
            case 15:
                return 5;
        }
        return 0;
    }

    static inline base::Index indexSkipForCrossSearch(base::Index index) {
        switch (index) {
            case 0:
                return 15;
            case 1:
                return 8;
            case 2:
                return 5;
            case 3:
            case 4:
                return 4;
            case 5:
                return 5;
            case 6:
            case 7:
                return 7;
            case 8:
                return 8;
            case 9:
                return 12;
            case 10:
            case 11:
                return 11;
            case 12:
                return 12;
            case 13:
            case 14:
                return 14;
            case 15:
                return 15;
        }
        return base::Index(0);
    }

    static inline void appendInvalidIndexPairs(
        base::Index index1, base::Index index2,
        std::set<std::pair<base::Index, base::Index> >& pairs
    ) {
        assertPrint(
            index2 >= base::Index(1),
            "PedigreeAnalyzer::appendInvalidIndexPairs: index2 must be larger than 1 but got " + std::to_string((unsigned int)index2)
        );
        
        unsigned int gen = indexToGeneration(index1);
        std::vector<unsigned int> indices1;
        if (gen == 1) {
            indices1 = {1,2,3,9,4,7,10,13,4,5,7,8,11,12,14,15};
        } else if (gen == 2) {
            indices1 = {2,3,6,4,5,7,8};
        } else if (gen == 3) {
            indices1 = {1+index1, 2+index1, 3+index1};
        } else if (gen == 4) {
            indices1 = {1+index1};
        }

        gen = indexToGeneration(index2);
        std::vector<unsigned int> indices2;
        if (gen == 2) {
            indices2 = {2,3,6,4,5,7,8};
        } else if (gen == 3) {
            indices2 = {1+index2, 2+index2, 3+index2};
        } else if (gen == 4) {
            indices2 = {1+index2};
        }

        for (int i = 0; i < (int)std::min(indices1.size(), indices2.size()); i++) {
            pairs.insert(std::make_pair(indices1[i], indices2[i]));
        }
    }

public:
    static inline PedigreeAnalysis analyze(
        const base::DefaultStallion& stallion, const base::DefaultBroodmare& broodmare,
        const std::vector<base::Stallion>& stallionVector, const base::ElaboratedPairs& pairs,
        size_t ignoreIndex, bool interesting=true, bool wonderful=true, bool elaborated=true,
        bool cross=true, bool nitro=true
    ) {
        PedigreeAnalysis result;

        // 面白い配合の判定
        if (interesting) {
            std::set<unsigned int> indices;
            stallion.appendInterestingIndices(indices);
            broodmare.appendInterestingIndices(indices);
            if (indices.size() >= 7) {
                result.isInteresting_ = true;
            }
        }

        // 見事な配合の判定
        if (wonderful) {
            std::set<unsigned int> sireIndices;
            std::set<unsigned int> broodmareIndices;
            stallion.appendWonderfulIndices(sireIndices);
            broodmare.appendInterestingIndices(broodmareIndices);
            if (sireIndices == broodmareIndices) {
                result.isWonderful_ = true;
            }
        }

        // 凝った配合の判定
        if (elaborated) {
            std::set<std::string_view> stallionAncestors;
            std::set<std::string_view> broodmareAncestors;
            std::vector<base::Index> indices = {1,2,3,6,9,10,13};

            bool finished = false;
            for (base::Index index1: indices) {
                for (base::Index index2: indices) {
                    if (pairs.hasPair(stallion.getAncestorIndex(index1), broodmare.getAncestorIndex(index2))) {
                        result.isElaborated_ = true;
                        finished = true;
                        break;
                    }
                }
                if (finished) {
                    break;
                }
            }
        }

        // クロスの判定とニトロの数え上げ
        if (!cross && nitro) {
            std::set<size_t> stallionsSet;
            size_t id;
            for (unsigned int i = 1; i <= 15; i++) {
                id = stallion.getAncestorIndex(i);
                if (id != ignoreIndex && stallionsSet.find(id) == stallionsSet.end()) {
                    if (stallionVector[id].isSprint()) {
                        result.nitro_.sprint_++;
                    }
                    if (stallionVector[id].isSpeed()) {
                        result.nitro_.speed_++;
                    }
                    if (stallionVector[id].isStamina()) {
                        result.nitro_.stamina_++;
                    }
                    if (stallionVector[id].isSpirit()) {
                        result.nitro_.spirit_++;
                    }
                    if (stallionVector[id].isPower()) {
                        result.nitro_.power_++;
                    }
                }
                if (id != ignoreIndex) {
                    stallionsSet.insert(id);
                }
            }

            for (unsigned int i = 1; i <= 15; i++) {
                id = broodmare.getAncestorIndex(i);
                if (id != ignoreIndex && stallionsSet.find(id) == stallionsSet.end()) {
                    if (stallionVector[id].isSprint()) {
                        result.nitro_.sprint_++;
                    }
                    if (stallionVector[id].isSpeed()) {
                        result.nitro_.speed_++;
                    }
                    if (stallionVector[id].isStamina()) {
                        result.nitro_.stamina_++;
                    }
                    if (stallionVector[id].isSpirit()) {
                        result.nitro_.spirit_++;
                    }
                    if (stallionVector[id].isPower()) {
                        result.nitro_.power_++;
                    }
                }
                if (id != ignoreIndex) {
                    stallionsSet.insert(id);
                }
            }
        } else if (cross) {
            std::set<size_t> stallionsSet;
            size_t id1, id2;
            std::vector<unsigned int> generation;
            std::set<std::pair<base::Index, base::Index> > invalidPairs;
            for (unsigned int i = 0; i <= 15; i++) {
                id1 = stallion.getAncestorIndex(i);
                bool hasCross = false;

                if (id1 != ignoreIndex && nitro && i != 0 && stallionsSet.find(id1) == stallionsSet.end()) {
                    stallionsSet.insert(id1);
                    if (stallionVector[id1].isSprint()) {
                        result.nitro_.sprint_++;
                    }
                    if (stallionVector[id1].isSpeed()) {
                        result.nitro_.speed_++;
                    }
                    if (stallionVector[id1].isStamina()) {
                        result.nitro_.stamina_++;
                    }
                    if (stallionVector[id1].isSpirit()) {
                        result.nitro_.spirit_++;
                    }
                    if (stallionVector[id1].isPower()) {
                        result.nitro_.power_++;
                    }
                }

                if (result.cross_.hasCross(id1)) {
                    result.cross_.append(id1, indexToGeneration(i));
                } else {
                    for (unsigned int j = 1; j <= 15; j++) {
                        id2 = broodmare.getAncestorIndex(j);
                        if (id2 != ignoreIndex && nitro && i == 0 && stallionsSet.find(id2) == stallionsSet.end()) {
                            stallionsSet.insert(id2);
                            if (stallionVector[id2].isSprint()) {
                                result.nitro_.sprint_++;
                            }
                            if (stallionVector[id2].isSpeed()) {
                                result.nitro_.speed_++;
                            }
                            if (stallionVector[id2].isStamina()) {
                                result.nitro_.stamina_++;
                            }
                            if (stallionVector[id2].isSpirit()) {
                                result.nitro_.spirit_++;
                            }
                            if (stallionVector[id2].isPower()) {
                                result.nitro_.power_++;
                            }
                        }

                        if (id1 != ignoreIndex && id1 == id2 && invalidPairs.find(std::make_pair(i,j)) == invalidPairs.end()) {
                            hasCross = true;
                            result.cross_.append(id1, indexToGeneration(j));
                            appendInvalidIndexPairs(i, j, invalidPairs);
                            if (i != 0 || !nitro) {
                                // ニトロを数え上げる場合はi==0のときスキップできない
                                j = indexSkipForCrossSearch(j);
                            }
                        }
                    }

                    if (hasCross) {
                        result.cross_.append(id1, indexToGeneration(i));
                    }
                }
            }
        }

        return result;
    }
};

}
}

#endif // SEARCH_PEDIGREEANALYZER_H
