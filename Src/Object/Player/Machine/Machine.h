#pragma once
#include<memory>
#include"../Object/ObjectBase.h"
#include"../Parameter/UnitParameter.h"
#include"../Object/Common/AnimationController.h"
#include"MachineImportData.h"

class Player;

class Machine : public ObjectBase
{
public:

	//パラメーターの基本倍率
	static constexpr float PARAM_NORMAL = 1.5f;

	//コンストラクタ
	Machine(const MachineImportData& _importData, const int _modelId, const VECTOR _localPos = {}, const VECTOR _pos = {});

	//デストラクタ
	~Machine(void)override;

	//読み込み
	void Load(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

	//当たり判定
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

	//当たり判定の生成
	void CreateCol(void);

	//当たり判定の削除
	void DeleteCol(void);

	//座標の設定
	void SetPos(const VECTOR& _pos) { trans_.pos = _pos; }

	//大きさ設定
	void SetScale(const VECTOR& _scale);

	//回転の設定
	void SetQuaRot(const Quaternion& _quaRot) { trans_.quaRot = _quaRot; }

	//半径の取得
	const float GetHitRadius(void)const { return radius_; }

	//乗車者の相対座標の取得
	const VECTOR& GetRiderLocalPos(void)const { return riderLocalPos_; }

	//パラメーターの取得
	const UnitParameter& GetUnitParam(void)const { return unitParam_; }

	//生存判定
	const bool IsDead(void)const;

private:

	//乗車判定半径
	static constexpr float RIDE_COL_RADIUS = 250.0f;
	
	//ムテキ時間
	static constexpr float INVINCIBLE_SPIN = 0.2f;

	//アニメーションの有無
	bool isAnim_;

	//アニメーション
	std::unique_ptr<AnimationController> anim_;

	//パラメーター
	UnitParameter unitParam_;

	//半径
	float radius_;

	//乗車者の相対座標
	VECTOR riderLocalPos_;

	//モデルサイズ
	VECTOR modelScl_;

	//ダメージ
	float damage_;

	//無敵時間
	float invincible_;

	//アニメーション
	void InitAnimation(void);
	void Animation(void);
};

