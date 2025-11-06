#pragma once
#include "../Common/Singleton.h"
#include "../Object/Player/Character/CharaImportData.h"

class CharacterManager
{
public:

	//インスタンス生成
	static void CreateInstance(void);

	//インスタンス取得
	static CharacterManager& GetInstance(void) { return *instance_; }

	//読み込み
	void Init(void);

	//解放
	void Destroy(void);

	/// @brief アニメーションの番号を取得
	/// @param _name キャラクター名
	/// @return キャラクターのアニメーション番号
	const std::unordered_map<std::string, int>& GetAnimationData(const std::string _name);

private:

	//静的インスタンス
	static CharacterManager* instance_;

	//アニメーション名に
	std::unordered_map<std::string, std::unordered_map<std::string, int>> anim_;

	//コンストラクタ
	CharacterManager(void);

	//デストラクタ
	~CharacterManager(void);
};

