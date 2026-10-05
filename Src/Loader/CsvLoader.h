#pragma once
#include<iostream>
#include "ILoader.h"

template<typename T>
class CsvLoader : public ILoader<T>
{
public:

    /// <summary>
	/// csvファイルをロードしてデータを取得する
    /// </summary>
    /// <param name="_filename">csvファイル名</param>
    /// <returns>データ</returns>
    std::vector<T> Load(const std::string& _filename) override {
        std::ifstream ifs(_filename);
        std::vector<T> result;
        std::string line;

        while (std::getline(ifs, line)) {
            std::stringstream ss(line);
            T data{};
            FromCsv(ss, data);
            result.push_back(data);
        }
        return result;
    }
};

