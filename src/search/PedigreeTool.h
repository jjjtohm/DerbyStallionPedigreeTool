#ifndef SEARCH_PEDIGREETOOL_H
#define SEARCH_PEDIGREETOOL_H

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include "base/Debug.h"
#include "base/Broodmare.h"
#include "base/DefaultStallion.h"
#include "base/ElaboratedPairs.h"
#include "base/Properties.h"
#include "base/Stallion.h"
#include "base/Thoroughbred.h"
#include "base/ThoroughbredMap.h"
#include "extra/json.hpp"
#include "search/PedigreeAnalyzer.h"

namespace pedsearch {
namespace search {

class PedigreeTool {
private:
    using json = nlohmann::json;
    std::unordered_map<std::string, size_t> defaultBroodmareMap_;
    std::vector<base::DefaultBroodmare> defaultBroodmares_;
    std::unordered_map<std::string, size_t> defaultStallionMap_;
    std::vector<base::DefaultStallion> defaultStallions_;
    std::unordered_map<std::string, size_t> stallionMap_;
    std::vector<base::Stallion> stallions_;
    base::ElaboratedPairs elaboratedPairs_;
    size_t ignoreStallionIndex_ = 0;

    void readDefaultBroodmares(std::string_view path);

    void readDefaultStallions(std::string_view path);

    void readStallions(std::string_view path);

    void readElaborated(std::string_view path);

public:
    PedigreeTool(
        std::string_view defaultStallions, std::string_view defaultBroodmares,
        std::string_view stallions, std::string_view elaborated
    );

    PedigreeAnalysis analyze(
        std::string_view stallion, std::string_view broodmare,
        bool interesting=true, bool wonderful=true, bool elaborated=true,
        bool cross=true, bool nitro=true
    ) const;

    PedigreeAnalysis analyze(
        std::string_view stallion, base::DefaultBroodmare broodmare,
        bool interesting=true, bool wonderful=true, bool elaborated=true,
        bool cross=true, bool nitro=true
    ) const;

    PedigreeAnalysis analyze(
        base::DefaultStallion stallion, base::DefaultBroodmare broodmare,
        bool interesting=true, bool wonderful=true, bool elaborated=true,
        bool cross=true, bool nitro=true
    ) const noexcept;

    void getDefaultStallionsSet(std::set<std::string_view>& stallionsSet) const noexcept;

    void getDefaultBroodmaresSet(std::set<std::string_view>& broodmaresSet) const noexcept;

    void getEffects(size_t id, std::vector<unsigned int>& effects) const;

    base::DefaultBroodmare makeDefaultBroodmare(
        std::string_view stallion, std::string_view broodmare
    ) const;

    base::DefaultBroodmare makeDefaultBroodmare(
        std::string_view stallion, base::DefaultBroodmare broodmare
    ) const;
};

}
}

#endif // SEARCH_PEDIGREETOOL_H
