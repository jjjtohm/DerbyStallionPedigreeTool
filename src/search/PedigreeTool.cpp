#include <regex>
#include "search/PedigreeTool.h"

namespace pedsearch {
namespace search {

    void PedigreeTool::readDefaultBroodmares(std::string_view path) {
        std::ifstream istream(path.data());
        if (!istream) {
            throw std::runtime_error(
                "PedigreeTool::readDefaultBroodmares: cannot open " + std::string(path) + "."
            );
        }
        json broodmaresList = json::parse(istream);
        for (size_t i = 0; i < broodmaresList.size(); i++) {
            std::string_view name = broodmaresList[i]["name"].get<std::string_view>();

            size_t ancestors[16];
            ancestors[0] = ignoreStallionIndex_;
            ancestors[1] = stallionMap_.at(broodmaresList[i]["ancestors"][0].get<std::string>());
            base::Stallion s = stallions_[ancestors[1]];
            ancestors[2] = stallionMap_.at(s.getSireName(0).data());
            ancestors[6] = stallionMap_.at(s.getSireName(1).data());
            ancestors[8] = stallionMap_.at(s.getSireName(2).data());
            base::Stallion ss = stallions_[ancestors[2]];
            ancestors[3] = stallionMap_.at(ss.getSireName(0).data());
            ancestors[5] = stallionMap_.at(ss.getSireName(1).data());
            base::Stallion sss = stallions_[ancestors[3]];
            ancestors[4] = stallionMap_.at(sss.getSireName(0).data());
            base::Stallion sds = stallions_[ancestors[6]];
            ancestors[7] = stallionMap_.at(sds.getSireName(0).data());
            ancestors[9] = stallionMap_.at(broodmaresList[i]["ancestors"][1].get<std::string>());
            base::Stallion ds = stallions_[ancestors[9]];
            ancestors[10] = stallionMap_.at(ds.getSireName(0).data());
            ancestors[12] = stallionMap_.at(ds.getSireName(1).data());
            base::Stallion dss = stallions_[ancestors[10]];
            ancestors[11] = stallionMap_.at(dss.getSireName(0).data());
            ancestors[13] = stallionMap_.at(broodmaresList[i]["ancestors"][2].get<std::string>());
            base::Stallion dds = stallions_[ancestors[13]];
            ancestors[14] = stallionMap_.at(dds.getSireName(0).data());
            ancestors[15] = stallionMap_.at(broodmaresList[i]["ancestors"][3].get<std::string>());

            unsigned int indices[8];
            for (size_t j = 0; j < 4; j++) {
                indices[j] = broodmaresList[i]["indices"][j].get<unsigned int>();
            }

            unsigned int fee = broodmaresList[i]["fee"].get<unsigned int>();
            unsigned int speed = broodmaresList[i]["speed"].get<unsigned int>();
            unsigned int stamina = broodmaresList[i]["stamina"].get<unsigned int>();
            unsigned int power = broodmaresList[i]["power"].get<unsigned int>();
            base::Dirt dirt;
            std::string_view d = broodmaresList[i]["dirt"].get<std::string_view>();
            if (d == "◎") {
                dirt = base::Dirt::GOOD;
            } else if (d == "○") {
                dirt = base::Dirt::NORMAL;
            } else if (d == "?") {
                dirt = base::Dirt::UNKNOWN;
            } else {
                throw std::runtime_error(
                    "PedigreeTool::readDefaultBroodmares: unknown dirt \"" + std::string(d) + "\"."
                );
            }

            defaultBroodmares_.push_back(
                base::DefaultBroodmare(ancestors, indices, fee, speed, stamina, power, dirt)
            );
            defaultBroodmareMap_.insert(std::make_pair(name, defaultBroodmares_.size() - 1));
        }
    }

    void PedigreeTool::readDefaultStallions(std::string_view path) {
        std::ifstream istream(path.data());
        if (!istream) {
            throw std::runtime_error(
                "PedigreeTool::readDefaultStallions: cannot open " + std::string(path) + "."
            );
        }
        json stallionsList = json::parse(istream);
        for (size_t i = 0; i < stallionsList.size(); i++) {
            std::string_view name = stallionsList[i]["name"].get<std::string_view>();

            size_t ancestors[16];
            ancestors[0] = stallionMap_.at(name.data());
            ancestors[1] = stallionMap_.at(stallionsList[i]["ancestors"][0].get<std::string>());
            base::Stallion s = stallions_[ancestors[1]];
            ancestors[2] = stallionMap_.at(s.getSireName(0).data());
            ancestors[6] = stallionMap_.at(s.getSireName(1).data());
            ancestors[8] = stallionMap_.at(s.getSireName(2).data());
            base::Stallion ss = stallions_[ancestors[2]];
            ancestors[3] = stallionMap_.at(ss.getSireName(0).data());
            ancestors[5] = stallionMap_.at(ss.getSireName(1).data());
            base::Stallion sss = stallions_[ancestors[3]];
            ancestors[4] = stallionMap_.at(sss.getSireName(0).data());
            base::Stallion sds = stallions_[ancestors[6]];
            ancestors[7] = stallionMap_.at(sds.getSireName(0).data());
            ancestors[9] = stallionMap_.at(stallionsList[i]["ancestors"][1].get<std::string>());
            base::Stallion ds = stallions_[ancestors[9]];
            ancestors[10] = stallionMap_.at(ds.getSireName(0).data());
            ancestors[12] = stallionMap_.at(ds.getSireName(1).data());
            base::Stallion dss = stallions_[ancestors[10]];
            ancestors[11] = stallionMap_.at(dss.getSireName(0).data());
            ancestors[13] = stallionMap_.at(stallionsList[i]["ancestors"][2].get<std::string>());
            base::Stallion dds = stallions_[ancestors[13]];
            ancestors[14] = stallionMap_.at(dds.getSireName(0).data());
            ancestors[15] = stallionMap_.at(stallionsList[i]["ancestors"][3].get<std::string>());

            unsigned int indices[8];
            for (size_t j = 0; j < 8; j++) {
                indices[j] = stallionsList[i]["indices"][j].get<unsigned int>();
            }

            unsigned int fee = stallionsList[i]["fee"].get<unsigned int>();
            base::Distance dist(
                stallionsList[i]["min"].get<unsigned int>(),
                stallionsList[i]["max"].get<unsigned int>()
            );

            base::Growth growth;
            std::string_view tmp = stallionsList[i]["growth"].get<std::string_view>();
            if (tmp == "早熟") {
                growth = base::Growth::PRECOCIOUS;
            } else if (tmp == "普通") {
                growth = base::Growth::NORMAL;
            } else if (tmp == "持続") {
                growth = base::Growth::PERSISTENT;
            } else if (tmp == "晩成") {
                growth = base::Growth::ALTRICAL;
            } else {
                throw std::runtime_error(
                    "PedigreeTool::readDefaultStallions: unknown growth \"" + std::string(tmp) + "\"."
                );
            }

            base::Dirt dirt;
            tmp = stallionsList[i]["dirt"].get<std::string_view>();
            if (tmp == "◎") {
                dirt = base::Dirt::GOOD;
            } else if (tmp == "○") {
                dirt = base::Dirt::NORMAL;
            } else if (tmp == "△") {
                dirt = base::Dirt::BAD;
            } else {
                throw std::runtime_error(
                    "PedigreeTool::readDefaultStallions: unknown dirt \"" + std::string(tmp) + "\"."
                );
            }

            base::Grade health;
            tmp = stallionsList[i]["health"].get<std::string_view>();
            if (tmp == "A") {
                health = base::Grade::A;
            } else if (tmp == "B") {
                health = base::Grade::B;
            } else if (tmp == "C") {
                health = base::Grade::C;
            } else {
                throw std::runtime_error(
                    "PedigreeTool::readDefaultStallions: unknown grade \"" + std::string(tmp) + "\"."
                );
            }

            base::Grade temper;
            tmp = stallionsList[i]["temper"].get<std::string_view>();
            if (tmp == "A") {
                temper = base::Grade::A;
            } else if (tmp == "B") {
                temper = base::Grade::B;
            } else if (tmp == "C") {
                temper = base::Grade::C;
            } else {
                throw std::runtime_error(
                    "PedigreeTool::readDefaultStallions: unknown grade \"" + std::string(tmp) + "\"."
                );
            }

            base::Grade achievement;
            tmp = stallionsList[i]["achievement"].get<std::string_view>();
            if (tmp == "A") {
                achievement = base::Grade::A;
            } else if (tmp == "B") {
                achievement = base::Grade::B;
            } else if (tmp == "C") {
                achievement = base::Grade::C;
            } else {
                throw std::runtime_error(
                    "PedigreeTool::readDefaultStallions: unknown grade \"" + std::string(tmp) + "\"."
                );
            }

            base::Grade spirit;
            tmp = stallionsList[i]["spirit"].get<std::string_view>();
            if (tmp == "A") {
                spirit = base::Grade::A;
            } else if (tmp == "B") {
                spirit = base::Grade::B;
            } else if (tmp == "C") {
                spirit = base::Grade::C;
            } else {
                throw std::runtime_error(
                    "PedigreeTool::readDefaultStallions: unknown grade \"" + std::string(tmp) + "\"."
                );
            }

            base::Grade stable;
            tmp = stallionsList[i]["stable"].get<std::string_view>();
            if (tmp == "A") {
                stable = base::Grade::A;
            } else if (tmp == "B") {
                stable = base::Grade::B;
            } else if (tmp == "C") {
                stable = base::Grade::C;
            } else {
                throw std::runtime_error(
                    "PedigreeTool::readDefaultStallions: unknown grade \"" + std::string(tmp) + "\"."
                );
            }

            defaultStallions_.push_back(
                base::DefaultStallion(
                    ancestors, indices, fee, dist, growth, dirt,
                    health, temper, achievement, spirit, stable
                )
            );
            defaultStallionMap_.insert(std::make_pair(name, defaultStallions_.size() - 1));
        }
    }

    void PedigreeTool::readStallions(std::string_view path) {
        std::ifstream istream(path.data());
        if (!istream) {
            throw std::runtime_error(
                "PedigreeTool::readStallions: cannot open " + std::string(path) + "."
            );
        }

        stallions_.push_back(
            base::Stallion("", base::Pedigree("","","",""), base::Blood(0), base::BloodEffect())
        );
        stallionMap_.insert(std::make_pair("", stallions_.size() - 1));

        json stallionsList = json::parse(istream);
        for (size_t i = 0; i < stallionsList.size(); i++) {
            std::string_view name = stallionsList[i]["name"].get<std::string_view>();

            base::Pedigree pedigree(
                stallionsList[i]["ancestors"][0].get<std::string_view>(),
                stallionsList[i]["ancestors"][1].get<std::string_view>(),
                stallionsList[i]["ancestors"][2].get<std::string_view>(),
                stallionsList[i]["ancestors"][3].get<std::string_view>()
            );

            base::Blood blood(stallionsList[i]["blood"].get<unsigned int>());

            bool sprint = false;
            bool speed = false;
            bool stamina = false;
            bool spirit = false;
            bool stable = false;
            bool temper = false;
            bool precocious = false;
            bool altrical = false;
            bool tough = false;
            bool dirt = false;
            bool power = false;

            for (size_t j = 0; j < stallionsList[i]["effects"].size(); j++) {
                std::string_view e = stallionsList[i]["effects"][j].get<std::string_view>();
                if (e == "短距離") {
                    sprint = true;
                } else if (e == "速力") {
                    speed = true;
                } else if (e == "長距離") {
                    stamina = true;
                } else if (e == "底力") {
                    spirit = true;
                } else if (e == "堅実") {
                    stable = true;
                } else if (e == "気性難") {
                    temper = true;
                } else if (e == "早熟") {
                    precocious = true;
                } else if (e == "晩成") {
                    altrical = true;
                } else if (e == "丈夫") {
                    tough = true;
                } else if (e == "ダート") {
                    dirt = true;
                } else if (e == "パワー") {
                    power = true;
                } else {
                    throw std::runtime_error(
                        "PedigreeTool::readStallions: unknown effect \"" + std::string(e) + "\"."
                    );
                }
            }
            base::BloodEffect effect(
                sprint, speed, stamina, spirit, stable, temper,
                precocious, altrical, tough, dirt, power
            );

            stallions_.push_back(base::Stallion(name, pedigree, blood, effect));
            stallionMap_.insert(
                std::make_pair(stallionsList[i]["name"].get<std::string>(), stallions_.size() - 1)
            );
        }
    }

    void PedigreeTool::readElaborated(std::string_view path) {
        std::ifstream istream(path.data());
        if (!istream) {
            throw std::runtime_error(
                "PedigreeTool::readElaborated: cannot open " + std::string(path) + "."
            );
        }
        json elaboratedList = json::parse(istream);
        for (size_t i = 0; i < elaboratedList.size(); i++) {
            elaboratedPairs_.insert(
                stallionMap_.at(elaboratedList[i][0].get<std::string>()),
                stallionMap_.at(elaboratedList[i][1].get<std::string>())
            );
        }
    }

    PedigreeTool::PedigreeTool(
        std::string_view path, std::string_view defaultStallions, std::string_view defaultBroodmares,
        std::string_view stallions, std::string_view elaborated
    ) {
        try {
            std::string pathString(path);
            std::smatch matched;
            std::string dirname;
            if (std::regex_match(pathString, matched, std::regex("^(.*)/[^/]+$"))) {
                dirname = matched[1].str();
            } else {
                throw std::runtime_error("PedigreeTool::PedigreeTool: " + pathString + "is not found.");
            }

            readStallions(dirname + "/" + stallions.data());
            readDefaultBroodmares(dirname + "/" + defaultBroodmares.data());
            readDefaultStallions(dirname + "/" + defaultStallions.data());
            readElaborated(dirname + "/" + elaborated.data());
        } catch (std::runtime_error e) {
            throw e;
        }
    }

    PedigreeAnalysis PedigreeTool::analyze(
        std::string_view stallion, std::string_view broodmare,
        bool interesting, bool wonderful, bool elaborated, bool cross, bool nitro
    ) const {
        auto itS = defaultStallionMap_.find(stallion.data());
        if (itS == defaultStallionMap_.end()) {
            throw std::runtime_error(
                "PedigreeTool::analyze: The stallion \"" + std::string(stallion) + "\" is unknown."
            );
        }
        auto itB = defaultBroodmareMap_.find(broodmare.data());
        if (itB == defaultBroodmareMap_.end()) {
            throw std::runtime_error(
                "PedigreeTool::analyze: The broodmare \"" + std::string(broodmare) + "\" is unknown."
            );
        }
        return PedigreeAnalyzer::analyze(
            defaultStallions_[(*itS).second], defaultBroodmares_[(*itB).second],
            stallions_, elaboratedPairs_, ignoreStallionIndex_, interesting, wonderful, elaborated,
            cross, nitro
        );
    }

    PedigreeAnalysis PedigreeTool::analyze(
        std::string_view stallion, base::DefaultBroodmare broodmare,
        bool interesting, bool wonderful, bool elaborated, bool cross, bool nitro
    ) const {
        auto itS = defaultStallionMap_.find(stallion.data());
        if (itS == defaultStallionMap_.end()) {
            throw std::runtime_error(
                "PedigreeTool::analyze: The stallion \"" + std::string(stallion) + "\" is unknown."
            );
        }

        return PedigreeAnalyzer::analyze(
            defaultStallions_[(*itS).second], broodmare,
            stallions_, elaboratedPairs_, ignoreStallionIndex_, interesting, wonderful, elaborated,
            cross, nitro
        );
    }

    PedigreeAnalysis PedigreeTool::analyze(
        base::DefaultStallion stallion, base::DefaultBroodmare broodmare,
        bool interesting, bool wonderful, bool elaborated, bool cross, bool nitro
    ) const noexcept {
        return PedigreeAnalyzer::analyze(
            stallion, broodmare, stallions_, elaboratedPairs_, ignoreStallionIndex_,
            interesting, wonderful, elaborated, cross, nitro
        );
    }

    void PedigreeTool::getDefaultStallionsSet(std::set<std::string_view>& stallionsSet) const noexcept {
        for (auto it = defaultStallionMap_.begin(); it != defaultStallionMap_.end(); ++it) {
            stallionsSet.insert((*it).first);
        }
    }

    void PedigreeTool::getDefaultBroodmaresSet(std::set<std::string_view>& broodmaresSet) const noexcept {
        for (auto it = defaultBroodmareMap_.begin(); it != defaultBroodmareMap_.end(); ++it) {
            broodmaresSet.insert((*it).first);
        }
    }

    void PedigreeTool::getEffects(size_t id, std::vector<unsigned int>& effects) const {
        try {
            base::Stallion stallion = stallions_[id];
            effects.resize(11);
            effects[0] = stallion.isSprint() ? 1 : 0;
            effects[1] = stallion.isSpeed() ? 1 : 0;
            effects[2] = stallion.isStamina() ? 1 : 0;
            effects[3] = stallion.isSpirit() ? 1 : 0;
            effects[4] = stallion.isStable() ? 1 : 0;
            effects[5] = stallion.isTemper() ? 1 : 0;
            effects[6] = stallion.isPrecocious() ? 1 : 0;
            effects[7] = stallion.isAltrical() ? 1 : 0;
            effects[8] = stallion.isTough() ? 1 : 0;
            effects[9] = stallion.isDirt() ? 1 : 0;
            effects[10] = stallion.isPower() ? 1 : 0;
        } catch (std::runtime_error e) {
            throw e;
        }
    }

    base::DefaultBroodmare PedigreeTool::makeDefaultBroodmare(
        std::string_view stallion, std::string_view broodmare
    ) const {
        auto itS = defaultStallionMap_.find(stallion.data());
        if (itS == defaultStallionMap_.end()) {
            throw std::runtime_error(
                "PedigreeTool::makeDefaultBroodmare: The stallion \"" + std::string(stallion) + "\" is unknown."
            );
        }
        auto itB = defaultBroodmareMap_.find(broodmare.data());
        if (itB == defaultBroodmareMap_.end()) {
            throw std::runtime_error(
                "PedigreeTool::makeDefaultBroodmare: The broodmare \"" + std::string(broodmare) + "\" is unknown."
            );
        }

        size_t sId = (*itS).second;
        size_t bId = (*itB).second;
        size_t ancestors[16];
        unsigned int indices[4];

        ancestors[0] = ignoreStallionIndex_;
        ancestors[1] = defaultStallions_[sId].getAncestorIndex(0);
        ancestors[2] = defaultStallions_[sId].getAncestorIndex(1);
        ancestors[3] = defaultStallions_[sId].getAncestorIndex(2);
        ancestors[4] = defaultStallions_[sId].getAncestorIndex(3);
        ancestors[5] = defaultStallions_[sId].getAncestorIndex(6);
        ancestors[6] = defaultStallions_[sId].getAncestorIndex(9);
        ancestors[7] = defaultStallions_[sId].getAncestorIndex(10);
        ancestors[8] = defaultStallions_[sId].getAncestorIndex(13);
        ancestors[9] = defaultBroodmares_[bId].getAncestorIndex(1);
        ancestors[10] = defaultBroodmares_[bId].getAncestorIndex(2);
        ancestors[11] = defaultBroodmares_[bId].getAncestorIndex(3);
        ancestors[12] = defaultBroodmares_[bId].getAncestorIndex(6);
        ancestors[13] = defaultBroodmares_[bId].getAncestorIndex(9);
        ancestors[14] = defaultBroodmares_[bId].getAncestorIndex(10);
        ancestors[15] = defaultBroodmares_[bId].getAncestorIndex(13);

        std::vector<unsigned int> sIndex = defaultStallions_[sId].getInterestingIndices();
        std::vector<unsigned int> bIndex = defaultBroodmares_[bId].getInterestingIndices();
        indices[0] = sIndex[0];
        indices[1] = sIndex[2];
        indices[2] = bIndex[0];
        indices[3] = bIndex[2];

        return base::DefaultBroodmare(ancestors, indices);
    }

    base::DefaultBroodmare PedigreeTool::makeDefaultBroodmare(
        std::string_view stallion, base::DefaultBroodmare broodmare
    ) const {
        auto itS = defaultStallionMap_.find(stallion.data());
        if (itS == defaultStallionMap_.end()) {
            throw std::runtime_error(
                "PedigreeTool::makeDefaultBroodmare: The stallion \"" + std::string(stallion) + "\" is unknown."
            );
        }

        size_t sId = (*itS).second;
        size_t ancestors[16];
        unsigned int indices[4];

        ancestors[0] = ignoreStallionIndex_;
        ancestors[1] = defaultStallions_[sId].getAncestorIndex(0);
        ancestors[2] = defaultStallions_[sId].getAncestorIndex(1);
        ancestors[3] = defaultStallions_[sId].getAncestorIndex(2);
        ancestors[4] = defaultStallions_[sId].getAncestorIndex(3);
        ancestors[5] = defaultStallions_[sId].getAncestorIndex(6);
        ancestors[6] = defaultStallions_[sId].getAncestorIndex(9);
        ancestors[7] = defaultStallions_[sId].getAncestorIndex(10);
        ancestors[8] = defaultStallions_[sId].getAncestorIndex(13);
        ancestors[9] = broodmare.getAncestorIndex(1);
        ancestors[10] = broodmare.getAncestorIndex(2);
        ancestors[11] = broodmare.getAncestorIndex(3);
        ancestors[12] = broodmare.getAncestorIndex(6);
        ancestors[13] = broodmare.getAncestorIndex(9);
        ancestors[14] = broodmare.getAncestorIndex(10);
        ancestors[15] = broodmare.getAncestorIndex(13);

        std::vector<unsigned int> sIndex = defaultStallions_[sId].getInterestingIndices();
        std::vector<unsigned int> bIndex = broodmare.getInterestingIndices();
        indices[0] = sIndex[0];
        indices[1] = sIndex[2];
        indices[2] = bIndex[0];
        indices[3] = bIndex[2];

        return base::DefaultBroodmare(ancestors, indices);
    }

}
}
