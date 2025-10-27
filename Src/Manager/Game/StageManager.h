#pragma once
#include<vector>
#include<memory>
#include<string>
#include"../Object/Stage/StageObject.h"

class StageManager
{
public:

	//オブジェクトの種類
	enum class OBJECT_TYPE
	{
		GLASS,		//草地
		BUILDING,	//ビル

		MAX,
	};

	//外部インポート用
	struct ImportData
	{
		std::string name;
		VECTOR position;
		VECTOR scale;
		Quaternion quaternion;
	};

	//インスタンス生成
	static void CreateInstance(void);

	//インスタンス取得
	static StageManager& GetInstance(void) { return *instance_; }

	//読み込み
	void Load(void);

	//初期化
	void Init(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//解放
	void Destroy(void);

private:

	//インポートデータ
	std::vector<ImportData> importData_;

	//名前管理
	std::unordered_map<std::string, int> modelId_;

	//静的インスタンス
	static StageManager* instance_;

	//ステージ
    std::vector<std::unique_ptr<StageObject>> stages_;

	//コンストラクタ
	StageManager(void);

	//コピー禁止
	StageManager(const StageManager& _copy) = delete;
	StageManager& operator= (const StageManager& _copy) = delete;

	//デストラクタ
	~StageManager(void);

	//外部管理用
	friend void FromJson(const nlohmann::json&, ImportData&);
	friend void FromCsv(std::stringstream& _ss, ImportData&);
};

//Json呼び出し
inline void FromJson(const nlohmann::json& _j, StageManager::ImportData& _data)
{
	//名前
	if (_j.contains("name"))
	{
		_data.name = _j.at("name").get<std::string>();
	}
	else
	{
		std::cerr << "StageObjectのJsonファイルにnameが存在しません" << "\n";
		return;
	}

	//座標
	if (_j[_data.name].contains("position"))
	{
		const auto& pos = _j.at("position");
		_data.position.x = pos.value("x", 0.0f);
		_data.position.y = pos.value("y", 0.0f);
		_data.position.z = pos.value("z", 0.0f);
	}

	//大きさ
	if (_j[_data.name].contains("scale"))
	{
		const auto& scl = _j.at("scale");
		_data.scale.x = scl.value("x", 1.0f);
		_data.scale.y = scl.value("y", 1.0f);
		_data.scale.z = scl.value("z", 1.0f);
	}

	//回転
	if (_j[_data.name].contains("quaternion"))
	{
		const auto& q = _j.at("quaternion");
		_data.quaternion.w = q.value("w", 1.0f);
		_data.quaternion.x = q.value("x", 0.0f);
		_data.quaternion.y = q.value("y", 0.0f);
		_data.quaternion.z = q.value("z", 0.0f);
	}
}

inline void FromCsv(std::stringstream& _ss, StageManager::ImportData& _data)
{
	//格納内容
	std::string token;

	//インポート

	//名前
	std::getline(_ss, _data.name, ',');

	//座標
	std::getline(_ss, token, ',');	_data.position.x = std::stof(token);
	std::getline(_ss, token, ',');	_data.position.y = std::stof(token);
	std::getline(_ss, token, ',');	_data.position.z = std::stof(token);

	//大きさ
	std::getline(_ss, token, ',');	_data.scale.x = std::stof(token);
	std::getline(_ss, token, ',');	_data.scale.y = std::stof(token);
	std::getline(_ss, token, ',');	_data.scale.z = std::stof(token);

	//回転
	std::getline(_ss, token, ',');	_data.quaternion.w = std::stof(token);
	std::getline(_ss, token, ',');	_data.quaternion.x = std::stof(token);
	std::getline(_ss, token, ',');	_data.quaternion.y = std::stof(token);
	std::getline(_ss, token, ',');	_data.quaternion.z = std::stof(token);
}
