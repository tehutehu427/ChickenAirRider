#pragma once
#include<memory>
#include"../Object/Common/Transform.h"
#include"../Object/Common/AnimationController.h"
#include"../Parameter/UnitParameter.h"

class Character
{
public:

	//パラメーターの基本倍率
	static constexpr float PARAM_NORMAL = 2.5f;

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

	//大きさ設定
	void SetScale(const VECTOR& _scale) { trans_.scl = _scale; }

	//回転の設定
	void SetQuaRot(const Quaternion& _quaRot) { trans_.quaRot = _quaRot; }

	//相対座標の設定
	void SetLocalPos(const VECTOR& _localPos) { trans_.localPos = _localPos; }

	//アニメーションの取得
	AnimationController& GetAnim(void)const { return *anim_; }

	//パラメーターの取得
	const UnitParameter& GetUnitParam(void)const { return unitParam_; }

private:

	// モデル制御の基本情報
	Transform trans_;

	//アニメーション
	std::unique_ptr<AnimationController> anim_;

	//パラメーター
	UnitParameter unitParam_;
};

