#pragma once
#include<memory>
#include"../Object/Common/Transform.h"
#include"../Parameter/UnitParameter.h"

class Character
{
public:

	//パラメーターの基本倍率
	static constexpr float PARAM_NORMAL = 1.7f;

	//キャラクターの種類
	enum class CHARA_TYPE
	{
		NONE = -1,

		CHICKEN,
		
		MAX,
	};

	//コンストラクタ
	Character(void);

	//デストラクタ
	~Character(void);

	//読み込み
	void Load(void);

	//初期化
	void Init(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//座標の設定
	void SetPos(const VECTOR& _pos) { trans_.pos = _pos; }

	//回転の設定
	void SetQuaRot(const Quaternion& _quaRot) { trans_.quaRot = _quaRot; }

	//パラメーターの取得
	const UnitParameter& GetUnitParam(void) { return unitParam_; }

private:

	// モデル制御の基本情報
	Transform trans_;

	//パラメーター
	UnitParameter unitParam_;
};

