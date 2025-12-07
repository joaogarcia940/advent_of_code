#include "../common/read_file.h"
#include <iostream>
#include <numeric>
#include <unordered_map>

using Number = std::size_t;

template <typename K, typename V>
void PrintMapTable(const std::unordered_map<K, V>& m)
{
    std::cout << "╔═════════╦═════════╗\n";
    std::cout << "║   Key   ║  Value  ║\n";
    std::cout << "╠═════════╬═════════╣\n";

    for (const auto& [k, v] : m)
    {
        std::cout << "║ " << std::setw(7) << k << " ║ " << std::setw(7) << v << " ║\n";
    }

    std::cout << "╚═════════╩═════════╝\n";
}

class BeamSimulator
{
  public:
    BeamSimulator(Text text)
    {
        const auto width = text.front().size();
        const auto height = text.size();

        std::unordered_map<Number, Number> track_row_beams{};
        const auto start_col = text.front().find_first_of('S');
        track_row_beams[start_col] = 1;
        text[1][start_col] = '|';

        for (Number row = 2; row < height; ++row)
        {
            std::unordered_map<Number, Number> next_row{};
            for (auto& [col, timelines] : track_row_beams)
            {
                switch (text[row][col])
                {
                    case '^':
                        text[row][col - 1] = '|';
                        text[row][col + 1] = '|';

                        ++count_splits_;

                        next_row[col - 1] += timelines;
                        next_row[col + 1] += timelines;
                        break;
                    default:
                        text[row][col] = '|';
                        next_row[col] += timelines;
                        break;
                }
            }

            track_row_beams = std::move(next_row);
        }

        for (auto& [_, timelines] : track_row_beams)
            count_timelines_ += timelines;
    }

    Number GetCountSplits() const { return count_splits_; }

    Number GetCountTimelines() const { return count_timelines_; }

  private:
    Number count_splits_{0};
    Number count_timelines_{0};
};

inline void Run()
{
    const auto beam_simulator = BeamSimulator{ReadFile("2025/day7/real.txt")};

    std::cout << "Part 1 - " << beam_simulator.GetCountSplits() << std::endl;
    std::cout << "Part 2 - " << beam_simulator.GetCountTimelines() << std::endl;
}