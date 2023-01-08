#include <iostream>
#include <initializer_list>
#include "search/PedigreeTool.h"

// 父,母,凝った,面白,見事,危険,短距離,速力,長距離,底力,安定,気性難,早熟,晩成,丈夫,ダート,パワー
void printPedigreeAnalysis(
    const pedsearch::search::PedigreeAnalysis& result,
    const std::initializer_list<std::string_view>& parents,
    const pedsearch::search::PedigreeTool& tool
) {
    for (std::string_view name: parents) {
        std::cout << name << ",";
    }

    if (result.isElaborated()) {
        std::cout << "1,";
    } else {
        std::cout << "0,";
    }

    if (result.isInteresting()) {
        std::cout << "1,";
    } else {
        std::cout << "0,";
    }

    if (result.isWonderful()) {
        std::cout << "1,";
    } else {
        std::cout << "0,";
    }

    std::set<size_t> crosses;
    result.getCross().getCrossIndices(crosses);
    if (crosses.size() == 0) {
        std::cout << "0,0,0,0,0,0,0,0,0,0,0,0,";
    } else {
        bool isDanger = false;
        std::vector<unsigned int> effects(11, 0);
        for (auto it = crosses.begin(); it != crosses.end(); ++it) {
            if (result.getCross().getBloodVolume(*it) >= 50.0) {
                isDanger = true;
            }

            std::vector<unsigned int> tmp;
            tool.getEffects(*it, tmp);

            for (size_t i = 0; i < 11; i++) {
                effects[i] += tmp[i];
            }
        }

        if (isDanger) {
            std::cout << "1,";
        } else {
            std::cout << "0,";
        }
        for (size_t i = 0; i < 11; i++) {
            std::cout << effects[i] << ",";
        }
    }

    std::cout << result.getNitro().getSpeedNitro() << ",";
    std::cout << result.getNitro().getStaminaNitro() << ",";
    std::cout << result.getNitro().getPowerNitro() << std::endl;;
}

void oneGeneration(std::string stallion, std::string broodmare) {
    try {
        pedsearch::search::PedigreeTool tool(
            "database/default_stallions.json",
            "database/default_broodmares.json",
            "database/stallions.json",
            "database/elaborated.json"
        );

        std::set<std::string_view> stallions;
        std::set<std::string_view> broodmares;
        if (stallion == "all") {
            tool.getDefaultStallionsSet(stallions);
        } else {
            stallions.insert(stallion);
        }

        if (broodmare == "all") {
            tool.getDefaultBroodmaresSet(broodmares);
        } else {
            broodmares.insert(broodmare);
        }

        std::cout << "父,母,凝った,面白,見事,危険,短距離,速力,長距離,底力,安定,気性難,早熟,晩成,丈夫,ダート,パワー,SP,ST,PW" << std::endl;
        for (auto itS = stallions.begin(); itS != stallions.end(); ++itS) {
            for (auto itB = broodmares.begin(); itB != broodmares.end(); ++itB) {
                pedsearch::search::PedigreeAnalysis result = tool.analyze(
                    *itS, *itB, true, true, true, true, true
                );
                printPedigreeAnalysis(result, {*itS, *itB}, tool);
            }
        }
    } catch (std::runtime_error e) {
        std::cerr << e.what() << std::endl;
    }
}

void twoGeneration(
    std::string secondStallion, std::string firstStallion, std::string broodmare
) {
    try {
        pedsearch::search::PedigreeTool tool(
            "database/default_stallions.json",
            "database/default_broodmares.json",
            "database/stallions.json",
            "database/elaborated.json"
        );

        std::set<std::string_view> firstStallions;
        std::set<std::string_view> secondStallions;
        std::set<std::string_view> broodmares;
        if (firstStallion == "all") {
            tool.getDefaultStallionsSet(firstStallions);
        } else {
            firstStallions.insert(firstStallion);
        }

        if (secondStallion == "all") {
            tool.getDefaultStallionsSet(secondStallions);
        } else {
            secondStallions.insert(secondStallion);
        }

        if (broodmare == "all") {
            tool.getDefaultBroodmaresSet(broodmares);
        } else {
            broodmares.insert(broodmare);
        }

        std::cout << "父,母父,母母,凝った,面白,見事,危険,短距離,速力,長距離,底力,安定,気性難,早熟,晩成,丈夫,ダート,パワー,SP,ST,PW" << std::endl;
        for (auto itS2 = secondStallions.begin(); itS2 != secondStallions.end(); ++itS2) {
            for (auto itS1 = firstStallions.begin(); itS1 != firstStallions.end(); ++itS1) {
                for (auto itB = broodmares.begin(); itB != broodmares.end(); ++itB) {
                    pedsearch::base::DefaultBroodmare tmp = tool.makeDefaultBroodmare(*itS1, *itB);
                    pedsearch::search::PedigreeAnalysis result = tool.analyze(
                        *itS2, tmp, true, true, true, true, true
                    );
                    printPedigreeAnalysis(result, {*itS2, *itS1, *itB}, tool);
                }
            }
        }

    } catch (std::runtime_error e) {
        std::cerr << e.what() << std::endl;
    }
}

void threeGeneration(
    std::string thirdStallion, std::string secondStallion, std::string firstStallion,
    std::string broodmare
) {
    try {
        pedsearch::search::PedigreeTool tool(
            "database/default_stallions.json",
            "database/default_broodmares.json",
            "database/stallions.json",
            "database/elaborated.json"
        );

        std::set<std::string_view> firstStallions;
        std::set<std::string_view> secondStallions;
        std::set<std::string_view> thirdStallions;
        std::set<std::string_view> broodmares;
        if (firstStallion == "all") {
            tool.getDefaultStallionsSet(firstStallions);
        } else {
            firstStallions.insert(firstStallion);
        }

        if (secondStallion == "all") {
            tool.getDefaultStallionsSet(secondStallions);
        } else {
            secondStallions.insert(secondStallion);
        }

        if (thirdStallion == "all") {
            tool.getDefaultStallionsSet(thirdStallions);
        } else {
            thirdStallions.insert(thirdStallion);
        }

        if (broodmare == "all") {
            tool.getDefaultBroodmaresSet(broodmares);
        } else {
            broodmares.insert(broodmare);
        }

        std::cout << "父,母父,母母父,母母母,凝った,面白,見事,危険,短距離,速力,長距離,底力,安定,気性難,早熟,晩成,丈夫,ダート,パワー,SP,ST,PW" << std::endl;
        for (auto itS3 = thirdStallions.begin(); itS3 != thirdStallions.end(); ++itS3) {
            for (auto itS2 = secondStallions.begin(); itS2 != secondStallions.end(); ++itS2) {
                for (auto itS1 = firstStallions.begin(); itS1 != firstStallions.end(); ++itS1) {
                    for (auto itB = broodmares.begin(); itB != broodmares.end(); ++itB) {
                        pedsearch::base::DefaultBroodmare tmp1 = tool.makeDefaultBroodmare(*itS1, *itB);
                        pedsearch::base::DefaultBroodmare tmp2 = tool.makeDefaultBroodmare(*itS2, tmp1);
                        pedsearch::search::PedigreeAnalysis result = tool.analyze(
                            *itS3, tmp2, true, true, true, true, true
                        );
                        printPedigreeAnalysis(result, {*itS3, *itS2, *itS1, *itB}, tool);
                    }
                }
            }
        }
    } catch (std::runtime_error e) {
        std::cerr << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "Usage:" << std::endl;
        std::cout << "pedtool [stallion_name] [broodmare_name]" << std::endl;
        std::cout << "pedtool [stallion_name] [stallion_name] [broodmare_name]" << std::endl;
        std::cout << "pedtool [stallion_name] [stallion_name] [stallion_name] [broodmare_name]" << std::endl;
        std::cout << "you can set \"all\" to stallion_name and broodmare_name." << std::endl;
    } else if (argc == 3) {
        oneGeneration(argv[1], argv[2]);
    } else if (argc == 4) {
        twoGeneration(argv[1], argv[2], argv[3]);
    } else if (argc == 5) {
        threeGeneration(argv[1], argv[2], argv[3], argv[4]);
    } else {
        std::cerr << "Invalid arguments." << std::endl;
    }
}
