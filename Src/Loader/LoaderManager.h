#pragma once
#include<unordered_map>
#include"ILoader.h"
#include"JsonLoader.h"
#include"CsvLoader.h"

template<typename T>
class LoaderManager
{
public:

    /// @brief ファイルの全データを取得
    /// @param _fileName ファイル名 
    /// @return データ
    const T& GetfileData(const std::string& _fileName)const
    {
        //ロード済みか
        if (!isFileDataLoaded_[_fileName])
        {
            //ロード
            FileLoad(_fileName);
        }

        return fileDataList_[_fileName];
    }

    /// @brief ファイル内のnameでデータを取得
    /// @param _filename ファイル名
    /// @param _dataName データ名
    /// @return データ
    const T& GetNameToData(const std::string& _filename, const std::string& _dataName)
    {
        //ロード済みか
        if (!isNameToDataLoaded_[_dataName])
        {
            //ロード
            NameToDataLoad(_fileName);
        }

        //名前に対応したデータが存在するか
        auto it = nameToDataList_.find(_dataName);
        if (it == nameToDataList_.end()) {
            throw std::runtime_error("データ内に次の名前は見つかりませんでした: " + _dataName);
        }
        return it->second;
    }

private:

    //ファイルごとのデータ
    std::unordered_map<std::string, T> fileDataList_;
    std::unordered_map<std::string, bool> isFileDataLoaded_;

	//ファイル内データ名対応データ
	std::unordered_map<std::string, T> nameToDataList_;
	std::unordered_map<std::string, bool> isNameToDataLoaded_;

    /// @brief ロード
    /// @param filename ファイル名
    void FileLoad(const std::string& _filename)
    {
        // 拡張子を取得
        auto pos = _filename.find_last_of('.');
        std::string ext = (pos == std::string::npos) ? "" : _filename.substr(pos + 1);

        //ローダー
        std::unique_ptr<ILoader<T>> loader;

        //Json
        if (ext == "json") {
            loader = std::make_unique<JsonLoader<T>>();
        }
        //csv
        else if (ext == "csv") {
            loader = std::make_unique<CsvLoader<T>>();
        }
        //対応なし
        else {
            throw std::runtime_error("以下の拡張子は対応していません->" + ext);
        }

        //各ローダーに命令
        fileDataList_ = loader->Load(_filename);
    }

    //オブジェクトの名前に対応したロード
    void NameToDataLoad(const std::string& _filename)
    {
        // 拡張子を取得
        auto pos = _filename.find_last_of('.');
        std::string ext = (pos == std::string::npos) ? "" : _filename.substr(pos + 1);

        std::unique_ptr<ILoader<T>> loader;

        //Json
        if (ext == "json") {
            loader = std::make_unique<JsonLoader<T>>();
        }
        //csv
        else if (ext == "csv") {
            loader = std::make_unique<CsvLoader<T>>();
        }
        //対応なし
        else {
            throw std::runtime_error("次の拡張子は対応していません:" + ext);
        }

        //各ローダーに命令
        auto dataList = loader->Load(_filename);

        //取得データを保存
        for (const auto& data : dataList)
        {
            nameToDataList_[dataList.name] = dataList;
        }

        //ロードした
        isNameToDataLoaded_[dataList.name] = true;
    }
};

