//
// Created by CTW00728 on 12/21/2024.
//

#ifndef ADVENT_DAY6_H
#define ADVENT_DAY6_H

#include "../utils/file_reader.h"
#include "../utils/numbers_in_string.h"
#include "../utils/print_vector.h"
#include <algorithm>
#include <fstream>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

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
  enum class ElementType { Unknown, Unvisited, Visited, Obstacle };

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

  std::unordered_map<
      Coordinates, std::unordered_map<ElementType, std::set<Guard::Direction>>,
      CoordinatesHash, CoordinatesEqual>
      map;
};

bool operator==(const Coordinates c1, const Coordinates c2) {
  return c1.x == c2.x && c1.y == c2.y;
}

class FileParser {
public:
  explicit FileParser(Text &&text) {
    Num x{0};
    Num y{0};
    for (auto &line : text) {
      x = 0;
      for (auto &letter : line) {
        switch (letter) {
        case '.':
          map_.map[{x, y}][Map::ElementType::Unvisited] = {
              Guard::Direction::None};
          break;
        case '#':
          map_.map[{x, y}][Map::ElementType::Obstacle] = {
              Guard::Direction::None};
          break;
        case '^':
          map_.map[{x, y}][Map::ElementType::Visited] = {Guard::Direction::Up};
          guard_ = Guard{{x, y}, Guard::Direction::Up};
          break;
        case 'v':
          map_.map[{x, y}][Map::ElementType::Visited] = {
              Guard::Direction::Down};
          guard_ = Guard{{x, y}, Guard::Direction::Down};
          break;
        case '<':
          map_.map[{x, y}][Map::ElementType::Visited] = {
              Guard::Direction::Left};
          guard_ = Guard{{x, y}, Guard::Direction::Left};
          break;
        case '>':
          map_.map[{x, y}][Map::ElementType::Visited] = {
              Guard::Direction::Right};
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

  auto GetMap() { return map_; }

  auto GetGuard() { return guard_; }

private:
  Map map_;
  Guard guard_;
};

class Route {
public:
  Route(Map &&map, Guard &&guard)
      : map_{map}, guard_{guard}, initial_guard_{guard}, loop_count_{0} {
    // First, walk the normal path and record all positions
    std::vector<Coordinates> path_positions;
    auto current_map_element = map_.map.find(guard_.position);

    while (true) {
      current_map_element->second[Map::ElementType::Visited].insert(
          guard_.direction);
      path_positions.push_back(guard_.position);

      auto next_map_element = map_.map.find(GetNextMapCoordinates());
      while (IsGuardInMap(next_map_element) &&
             next_map_element->second.find(Map::ElementType::Obstacle) !=
                 next_map_element->second.cend()) {
        TurnGuardRight90Degrees();
        next_map_element = map_.map.find(GetNextMapCoordinates());
      }
      if (not IsGuardInMap(next_map_element)) {
        break;
      }

      guard_.position = next_map_element->first;
      current_map_element = next_map_element;
    }

    // Now test placing obstacles at each position on the path
    std::set<Coordinates, CoordinatesCompare> tested_positions;
    for (const auto &test_pos : path_positions) {
      // Skip if we already tested this position or it's the starting position
      if (tested_positions.count(test_pos) > 0 ||
          test_pos == initial_guard_.position) {
        continue;
      }
      tested_positions.insert(test_pos);

      // Test if placing an obstacle here creates a loop
      if (TestObstacleCreatesLoop(test_pos)) {
        ++loop_count_;
      }
    }
  }

  long GetDistinctPositions() const {
    return std::count_if(
        map_.map.cbegin(), map_.map.cend(), [](const auto &map_elem) {
          return map_elem.second.find(Map::ElementType::Visited) !=
                 map_elem.second.end();
        });
  }

  long GetLoopCount() const { return loop_count_; }

private:
  struct CoordinatesCompare {
    bool operator()(const Coordinates &lhs, const Coordinates &rhs) const {
      if (lhs.x != rhs.x)
        return lhs.x < rhs.x;
      return lhs.y < rhs.y;
    }
  };

  struct StateCompare {
    bool operator()(const std::pair<Coordinates, Guard::Direction> &lhs,
                    const std::pair<Coordinates, Guard::Direction> &rhs) const {
      if (lhs.first.x != rhs.first.x)
        return lhs.first.x < rhs.first.x;
      if (lhs.first.y != rhs.first.y)
        return lhs.first.y < rhs.first.y;
      return lhs.second < rhs.second;
    }
  };

  bool TestObstacleCreatesLoop(const Coordinates &obstacle_pos) {
    // Create a copy of the map with the obstacle
    Map test_map = map_;
    test_map.map[obstacle_pos][Map::ElementType::Obstacle] = {
        Guard::Direction::None};

    // Simulate from the beginning with the new obstacle
    Guard test_guard = initial_guard_;
    std::set<std::pair<Coordinates, Guard::Direction>, StateCompare>
        visited_states;

    auto current_element = test_map.map.find(test_guard.position);
    while (true) {
      // Check if we've been in this exact state before (position + direction)
      auto state = std::make_pair(test_guard.position, test_guard.direction);
      if (visited_states.count(state) > 0) {
        return true; // Found a loop!
      }
      visited_states.insert(state);

      // Find next position
      auto next_coords = GetNextMapCoordinates(test_guard);
      auto next_element = test_map.map.find(next_coords);

      // Turn if hitting obstacle
      while (IsGuardInMap(next_element, test_map) &&
             next_element->second.find(Map::ElementType::Obstacle) !=
                 next_element->second.cend()) {
        test_guard.direction = TurnGuardRight90Degrees(test_guard.direction);
        next_coords = GetNextMapCoordinates(test_guard);
        next_element = test_map.map.find(next_coords);
      }

      // Check if guard left the map
      if (!IsGuardInMap(next_element, test_map)) {
        return false; // No loop, guard left the map
      }

      test_guard.position = next_element->first;
      current_element = next_element;
    }
  }

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
    const Num x = guard.position.x;
    const Num y = guard.position.y;
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

  template <typename MapElement> bool IsGuardInMap(MapElement map_element) {
    return map_element != map_.map.end();
  }

  template <typename MapElement>
  bool IsGuardInMap(MapElement map_element, const Map &test_map) {
    return map_element != test_map.map.end();
  }

  Map map_;
  Guard guard_;
  Guard initial_guard_;
  long loop_count_;
};

void Run() {
  FileReader file_reader{real};
  FileParser file_parser{file_reader.GetText()};
  Route route{file_parser.GetMap(), file_parser.GetGuard()};
  std::cout << "Part 1: " << route.GetDistinctPositions() << std::endl;
  std::cout << "Part 2: " << route.GetLoopCount() << std::endl;
}

} // namespace day6

#endif // ADVENT_DAY6_H
