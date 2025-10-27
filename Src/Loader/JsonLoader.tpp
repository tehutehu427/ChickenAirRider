#pragma once
#include <ofstream>
#include <nlohmann/json.hpp>

template<typename T>
std::vector<T> Load(const std::string& _filename) override;
{
        //ファイル名
        std::ifstream ifs(_filename);
        
        //Jsonファイルから出力
        nlohmann::json j; ifs >> j;

        //戻り値
        std::vector<T> result;

        //データを取得
        for (auto& elem : j) {
            T data{};
            FromJson(elem, data);
            result.push_back(data);
        }
        return result;
}