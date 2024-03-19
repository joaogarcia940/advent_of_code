#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <tuple>

std::pair<std::string, std::string> GetNumbersInLine(std::string line, int pos)
{
    std::string r_number{};
    std::string number{};
    for (auto r_iter = std::make_reverse_iterator(line.begin() + pos); r_iter != line.rend(); ++r_iter) // make all this safe
    {
        if (!std::isdigit(*r_iter))
        {
            break;
        }
        else
        {
            r_number.push_back(*r_iter);
        }
    }
    std::reverse(r_number.begin(), r_number.end());

    for (auto iter = line.begin() + pos + 1; iter != line.end(); ++iter)
    {
        if (!std::isdigit(*iter))
        {
            break;
        }
        else
        {
            number.push_back(*iter);
        }
    }
    return std::make_pair(r_number, number);
}

void FillNumbersVector(char middle, const std::string &r_number, const std::string &number, std::vector<int> &numbers)
{
    if (r_number.empty() && number.empty())
    {
        return;
    }
    // if the middle is a '.' they are joint numbers, if not they are separate numbers.
    if (std::isdigit(middle))
    {
        if (r_number.empty())
        {
            numbers.push_back(std::stoi(middle + number));
        }
        else if (number.empty())
        {
            numbers.push_back(std::stoi(r_number + middle));
        }
        else
        {
            numbers.push_back(std::stoi(r_number + middle + number));
        }
    }
    else
    {
        if (not r_number.empty())
        {
            numbers.push_back(std::stoi(r_number));
        }
        if (not number.empty())
        {
            numbers.push_back(std::stoi(number));
        }
    }
}

int main()
{
    std::ifstream file("/home/joao/workspace/advent/day3/example_input.txt");

    if (!file.is_open())
    {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::string line_below;
    int lineNumber = 0;
    std::string::const_iterator it;
    std::vector<int> numbers;

    while (std::getline(file, line_below))
    {
        lineNumber++;

        if (lineNumber > 2)
        {
            std::string line_above;
            file.seekg(-3 * line_below.length() - 3, std::ios_base::cur); // Move the file pointer back two lines
            std::getline(file, line_above);

            std::string line_middle;
            std::getline(file, line_middle);

            file.seekg(line_below.length() + 1, std::ios_base::cur);

            it = line_middle.begin();
            // check pos of special character
            while ((it != line_middle.end()))
            {
                if (!std::isdigit(*it) && !(*it == '.'))
                {
                    std::string r_number_above;
                    std::string number_above;
                    std::string r_number_middle;
                    std::string number_middle;
                    std::string r_number_below;
                    std::string number_below;
                    auto pos = it - line_middle.begin();

                    std::tie(r_number_above, number_above) = GetNumbersInLine(line_above, pos);
                    std::tie(r_number_middle, number_middle) = GetNumbersInLine(line_middle, pos);
                    std::tie(r_number_below, number_below) = GetNumbersInLine(line_below, pos);

                    FillNumbersVector(*(line_above.begin() + pos), r_number_above, number_above, numbers);
                    FillNumbersVector(*(line_middle.begin() + pos), r_number_middle, number_middle, numbers);
                    FillNumbersVector(*(line_below.begin() + pos), r_number_below, number_below, numbers);
                }
                ++it;
            }
        }
    }

    file.close();

    return 0;
}
