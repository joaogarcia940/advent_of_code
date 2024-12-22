#include <iostream>
#include <vector>
#include <optional>
#include "day1/day1.h"
#include "day2/day2.h"
#include "day3/day3.h"
#include "day4/day4.h"
#include "day5/day5.h"
#include "day6/day6.h"

using Arguments = std::vector<std::string>;

namespace {
    Arguments ParseArguments(int argc, char *argv[]) {
        // Check if arguments are provided
        if (argc < 2) {
            std::cout << "Please provide arguments in the format day1=<number>." << std::endl;
            return {};
        }
        // Store arguments in a vector of strings for convenience
        std::vector<std::string> arguments;
        for (int i = 1; i < argc; ++i) {  // Start from 1 to skip the program name
            arguments.emplace_back(argv[i]);
        }
        return arguments;
    }

    void PrintArguments(const Arguments &arguments) {
        // Print the provided strings
        std::cout << "You provided the following strings:" << std::endl;
        for (const auto &arg: arguments) {
            std::cout << "- " << arg << std::endl;
        }
    }

    std::optional<int> GetDay(const Arguments &arguments) {
        std::optional<int> day_value{};
        std::string prefix = "day=";
        for (auto &arg: arguments) {
            // Check if the argument starts with "day="
            if (arg.rfind(prefix, 0) == 0) {  // rfind with position 0 checks for prefix
                try {
                    // Extract the integer value after "day="
                    day_value = std::stoi(arg.substr(prefix.size()));
                    break;  // Stop after finding the first valid "day=" argument
                } catch (const std::exception &e) {
                    std::cout << "Invalid format for day1 argument: " << arg << std::endl;
                    return std::nullopt;
                }
            } else {
                printf("ups\n");
            }
        }
        return day_value;
    }
}


int main(int argc, char *argv[]) {
    const auto arguments = ParseArguments(argc, argv);
    if (arguments.empty()) {
        return 1;
    }
    PrintArguments(arguments);
    const auto day = GetDay(arguments);
    if (!day.has_value()) {
        std::cout << "No day found" << std::endl;
        return 1;
    }
    switch (day.value()) {
        case 1:
            day1::Run();
            break;
        case 2:
            day2::Run();
            break;
        case 3:
            day3::Run();
            break;
        case 4:
            day4::Run();
            break;
        case 5:
            day5::Run();
            break;
        case 6:
            day6::Run();
            break;
        default:
            std::cout << "No day found" << std::endl;
            return 1;
    }


    return 0;
}
