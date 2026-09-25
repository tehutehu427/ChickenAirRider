#pragma once
#include "../../Common/Singleton.h"
#include "../../Object/Player/Character/CharacterImportData.h"

class Character;

class CharacterManager : public Singleton<CharacterManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<CharacterManager>;

public:

	//キャラクターの種類
	enum class CHARA_TYPE
	{
		NONE = -1,

		CHICKEN,	//チキン

		MAX
	};

	//キャラクターの取得
	std::unique_ptr<Character> CreateCharacter(const std::string& _name)const;

private:

	//インポートデータ
	std::vector<CharacterImportData> importData_;
	
	//名前管理
	using Func = const int(CharacterManager::*)(void);
	std::unordered_map<std::string, Func> getModelId_;

	//コンストラクタ
	CharacterManager(void);

	//デストラクタ
	~CharacterManager(void)override;

	//読み込み
	void LoadOutSide(void)override;

	//モデルIDの取得
	const int GetChickenModelId(void);
};

