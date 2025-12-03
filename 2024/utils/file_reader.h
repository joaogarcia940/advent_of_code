//
// Created by CTW00728 on 12/3/2024.
//

#ifndef ADVENT_FILE_READER_H
#define ADVENT_FILE_READER_H

#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <string_view>
#include <optional>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <iostream>

using Text = std::vector<std::string>;

class FileReader {
public:
    explicit FileReader(const std::string_view &file_path) : file_path_{file_path} {
        ReadFile();
    }

    Text GetText() const {
        return text_;
    }

private:
    void ReadFile() {
        std::cout << "reading file " << file_path_ << std::endl;
        std::ifstream input_file(std::string{file_path_});
        if (input_file.is_open()) {
            std::string line;

            while (getline(input_file, line)) {
                text_.push_back(line);
            }
            input_file.close();
        } else {
            std::cerr << "Unable to open file: " << file_path_ << std::endl;
        }
    }

    const std::string_view file_path_;
    Text text_;
};


#endif //ADVENT_FILE_READER_H
