#pragma once
#include"../Lib/nlohmann/json.hpp"
#include"ILoader.h"

template<typename T>
class JsonLoader : public ILoader<T>
{
public:

    /// <summary>
    ///  jsonファイルをロードしてデータを取得する
    /// </summary>
    /// <param name="_filename">jsonファイル名</param>
    /// <returns>対応データ</returns>
    std::vector<T> Load(const std::string& _filename) override
	{
        //ファイル名
        std::ifstream ifs(_filename);
        if (!ifs.is_open()) {
            throw std::runtime_error("ファイルを開けません: " + _filename);
        }
        
        //Jsonファイルから出力
        nlohmann::json j;
        ifs >> j;

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
};