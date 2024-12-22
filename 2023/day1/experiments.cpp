#include <iostream>
#include <regex>

int main()
{
    std::string input = "psrfcqqbslfhthree4";
    std::regex pattern(R"((?:zero|one|two|three|four|five|six|seven|eight|nine|(\d)))");

    std::smatch match;
    std::sregex_iterator it(input.begin(), input.end(), pattern);
    std::sregex_iterator end;

    if (it != end)
    {
        std::string extractedValue = it->str();
        size_t position = it->position();
        std::cout << "Found: " << extractedValue << " at position " << position << std::endl;
    }
    else
    {
        std::cout << "Not found." << std::endl;
    }

    return 0;
}
