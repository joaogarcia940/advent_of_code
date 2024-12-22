//
// Created by CTW00728 on 12/21/2024.
//

#ifndef ADVENT_DAY6_H
#define ADVENT_DAY6_H

#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <string_view>
#include <optional>
#include <queue>
#include <sstream>
#include <algorithm>
#include <utility>
#include <numeric>
#include <unordered_map>
#include <set>
#include "../utils/file_reader.h"
#include "../utils/numbers_in_string.h"
#include "../utils/print_vector.h"

namespace day6 {

    constexpr std::string_view example = "day6/example.txt";
    constexpr std::string_view real = "day6/real.txt";
    constexpr std::string_view test = "day6/test.txt";

    using Num = int;

    struct Coordinates {
        Num x{};
        Num y{};
    };

    struct Guard {
        enum class Direction {
            None,
            Up,
            Down,
            Left,
            Right,
        };
        Coordinates position{};
        Direction direction{Direction::None};
    };

    struct Map {
        enum class ElementType {
            Unknown,
            Unvisited,
            Visited,
            Obstacle
        };

        // Custom hash function
        struct CoordinatesHash {
            std::size_t operator()(const Coordinates &c) const {
                return std::hash<Num>()(c.x) ^ (std::hash<Num>()(c.y) << 1);
            }
        };

        // Custom equality function
        struct CoordinatesEqual {
            bool operator()(const Coordinates &lhs, const Coordinates &rhs) const {
                return lhs.x == rhs.x && lhs.y == rhs.y;
            }
        };

        std::unordered_map<Coordinates, std::unordered_map<ElementType, std::set<Guard::Direction>>, CoordinatesHash, CoordinatesEqual> map;
    };

    bool operator==(const Coordinates c1, const Coordinates c2) {
        return c1.x == c2.x && c1.y == c2.y;
    }

    class FileParser {
    public:
        explicit FileParser(Text &&text) {
            Num x{0};
            Num y{0};
            for (auto &line: text) {
                x = 0;
                for (auto &letter: line) {
                    switch (letter) {
                        case '.':
                            map_.map[{x, y}][Map::ElementType::Unvisited] =
                                    {Guard::Direction::None};
                            break;
                        case '#':
                            map_.map[{x, y}][Map::ElementType::Obstacle] = {Guard::Direction::None};
                            break;
                        case '^':
                            map_.map[{x, y}][Map::ElementType::Visited] = {Guard::Direction::Up};
                            guard_ = Guard{{x, y}, Guard::Direction::Up};
                            break;
                        case 'v':
                            map_.map[{x, y}][Map::ElementType::Visited] = {Guard::Direction::Down};
                            guard_ = Guard{{x, y}, Guard::Direction::Down};
                            break;
                        case '<':
                            map_.map[{x, y}][Map::ElementType::Visited] = {Guard::Direction::Left};
                            guard_ = Guard{{x, y}, Guard::Direction::Left};
                            break;
                        case '>':
                            map_.map[{x, y}][Map::ElementType::Visited] = {Guard::Direction::Right};
                            guard_ = Guard{{x, y}, Guard::Direction::Right};
                            break;
                        case '\n':
                            break;
                        default:
                            std::cerr << "Weird char" << letter << std::endl;
                    }
                    ++x;
                }
                ++y;
            }
        }

        auto GetMap() {
            return map_;
        }

        auto GetGuard() {
            return guard_;
        }

    private:
        Map map_;
        Guard guard_;
    };

    // se ele voltar onde ja esteve e estiver a 90 graus a direita de outro ja visitado tenho um loop
    class Route {
    public:
        Route(Map &&map, Guard &&guard) : map_{map}, guard_{guard}, loop_count_{0} {
            auto current_map_element = map_.map.find(guard_.position);
            while (true) {
                current_map_element->second[Map::ElementType::Visited].insert(guard_.direction);

                auto next_map_element = map_.map.find(GetNextMapCoordinates());
                while (IsGuardInMap(next_map_element) &&
                       next_map_element->second.find(Map::ElementType::Obstacle) != next_map_element->second.cend()) {
                    TurnGuardRight90Degrees();
                    next_map_element = map_.map.find(GetNextMapCoordinates());
                }
                if (not IsGuardInMap(next_map_element)) {
                    break;
                }
                if (next_map_element->second.find(Map::ElementType::Visited) !=
                    next_map_element->second.end()) // if it was already visited
                {
                    auto guard_backup = guard_;

                    guard_backup.position = next_map_element->first;
                    guard_backup.direction = TurnGuardRight90Degrees(guard_backup.direction);

                    const auto next_next_map_element = map_.map.find(GetNextMapCoordinates(guard_backup));
                    if (next_next_map_element->second.find(Map::ElementType::Visited) !=
                        next_next_map_element->second.end() &&
                        next_next_map_element->second[Map::ElementType::Visited].find(
                                guard_backup.direction) !=
                        next_next_map_element->second[Map::ElementType::Visited].end()) // you found a loop
                    {
                        const Guard guard_two_steps_ahead_of_real{
                                GetNextMapCoordinates({GetNextMapCoordinates(guard_), guard_.direction}),
                                guard_.direction};
                        std::cout << "found loop by putting obstacle in (" << guard_two_steps_ahead_of_real.position.x
                                  << ","
                                  << guard_two_steps_ahead_of_real.position.y << ")" << std::endl;
                        ++loop_count_;
                    }
                }

                guard_.position = next_map_element->first;
                current_map_element = next_map_element;
            }
        }

        long GetDistinctPositions() const {
            return std::count_if(map_.map.cbegin(), map_.map.cend(), [](const auto &map_elem) {
                return map_elem.second.find(Map::ElementType::Visited) != map_elem.second.end();
            });
        }

        long GetLoopCount() const {
            return loop_count_;
        }

    private:
        void TurnGuardRight90Degrees() {
            guard_.direction = TurnGuardRight90Degrees(guard_.direction);
        }

        static Guard::Direction TurnGuardRight90Degrees(Guard::Direction direction) {
            switch (direction) {
                case Guard::Direction::Up:
                    return Guard::Direction::Right;
                    break;
                case Guard::Direction::Right:
                    return Guard::Direction::Down;
                    break;
                case Guard::Direction::Down:
                    return Guard::Direction::Left;
                    break;
                case Guard::Direction::Left:
                    return Guard::Direction::Up;
                    break;
                default:
                    std::cerr << "Invalid guard direction" << std::endl;
            }
            return Guard::Direction::None;
        }

        Coordinates GetNextMapCoordinates() const {
            return GetNextMapCoordinates(guard_);
        }

        static Coordinates GetNextMapCoordinates(const Guard &guard) {
            const auto [x, y] = guard.position;
            switch (guard.direction) {
                case Guard::Direction::Up:
                    return {x, y - 1};
                case Guard::Direction::Down:
                    return {x, y + 1};
                case Guard::Direction::Left:
                    return {x - 1, y};
                case Guard::Direction::Right:
                    return {x + 1, y};
                default:
                    std::cerr << "Invalid guard direction" << std::endl;
            }
            return {x, y};
        }

        bool IsGuardInMap(const auto map_element) {
            return map_element != map_.map.end();
        }

        Map map_;
        Guard guard_;
        long loop_count_;
    };

    void Run() {
        FileReader file_reader{example};
        FileParser file_parser{file_reader.GetText()};
        Route route{file_parser.GetMap(), file_parser.GetGuard()};
        std::cout << "Part 1: " << route.GetDistinctPositions() << std::endl;
        std::cout << "Part 2: " << route.GetLoopCount() << std::endl;
    }

}

#endif //ADVENT_DAY6_H