#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <regex>
#include <unordered_map>

using Text = std::vector<std::string>;

class FileToText
{
public:
    explicit FileToText(const std::string &file_path) : file_path_{file_path}
    {
        ReadFile();
    }

    void ReadFile()
    {
        std::ifstream input_file(file_path_);
        if (input_file.is_open())
        {
            std::string line;

            while (getline(input_file, line))
            {
                text_.push_back(line);
            }
            input_file.close();
        }
        else
        {
            std::cerr << "Unable to open file: " << file_path_ << std::endl;
        }
    }

    Text GetText()
    {
        return text_;
    }

protected:
    std::string file_path_;
    Text text_;
};

template <typename T>
class FileParser
{
public:
    virtual T &Filter() = 0;

    Text Get()
    {
        return text_;
    }

    T &Print(const std::string &additional_log = "")
    {
        std::for_each(text_.cbegin(), text_.cend(), [&additional_log](const std::string &line)
                      { std::cout << line << additional_log << std::endl; });
        return *static_cast<T *>(this);
    }

    T &WithPattern(const std::regex &pattern)
    {
        pattern_ = pattern;
        return *static_cast<T *>(this);
    }

    T &WithInputPath(const std::string &path)
    {
        file_path_ = path;
        return *static_cast<T *>(this);
    }

    T &WithTextFromFile(const std::string &path)
    {
        file_path_ = path;
        FileToText file_reader{path};
        text_ = file_reader.GetText();
        return *static_cast<T *>(this);
    }

    T &WithAddedLine(const std::string &line)
    {
        text_.push_back(line);
        return *static_cast<T *>(this);
    }

protected:
    Text text_;
    std::regex pattern_;
    std::string file_path_;
};

void PrintText(const Text &text, std::ostream &ostream = std::cout)
{
    std::for_each(text.begin(), text.end(), [&ostream](const std::string &line)
                  { ostream << line << std::endl; });
}

void PrintText(const Text &text, std::ostream &&ostream)
{
    PrintText(text, ostream);
}
