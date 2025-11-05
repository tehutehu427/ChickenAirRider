#pragma once
#include "../Common/Singleton.h"
#include "../Object/Player/Character/CharaImportData.h"

class CharacterManager : public Singleton<CharacterManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<CharacterManager>;

public:

	//読み込み
	void Init(void);

private:

	//キャラクターの読み込み情報
	std::vector<CharaImportData> importData_;

	//コンストラクタ
	CharacterManager(void);

	//デストラクタ
	~CharacterManager(void)override;
};

