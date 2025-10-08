#pragma once
#include<memory>
#include<set>
#include<vector>
#include "Common/Transform.h"
#include "../Common/IntVector3.h"
#include "Common/Collider.h"

class ResourceManager;
class SceneManager;
class Geometry;
class EffectController;

class ObjectBase
{
public:

	//当たり判定情報
	struct ColParam
	{
		std::unique_ptr<Geometry> geometry_;	//形状情報
		std::shared_ptr<Collider> collider_;	//全体の当たり判定情報
	};

	// コンストラクタ
	ObjectBase(void);

	// デストラクタ
	virtual ~ObjectBase(void);

	//読み込み
	virtual void Load(void) = 0;
	//初期化
	virtual void Init(void) = 0;
	//更新
	virtual void Update(void) = 0;
	//描画
	virtual void Draw(void) = 0;

	//当たり判定の削除
	void Sweep(void);

	/// <summary>
	/// それぞれの当たり判定後の処理
	/// </summary>
	/// <param name="_hitColTag">相手側の当たり判定タグ</param>
	virtual void OnHit(const std::weak_ptr<Collider> _hitCol) = 0;

	//モデル情報の取得
	inline const Transform& GetTrans(void) const { return trans_; }

	//モデルの色を変える
	virtual void ChangeModelColor(const COLOR_F _colorScale);

protected:

	// シングルトン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// モデル制御の基本情報
	Transform trans_;

	//当たり判定関係
	std::vector<ColParam> colParam_;

	//エフェクト
	std::unique_ptr<EffectController> effect_;

	/// <summary>
	/// 当たり判定作成(形状情報作成後)
	/// </summary>
	/// <param name="_tag">自身の当たり判定タグ</param>
	/// <param name="_Geometry">自身の形状情報</param>
	/// <param name="_notHitTags">衝突させないタグ</param>
	void MakeCollider(const std::set<Collider::TAG> _tag, std::unique_ptr<Geometry> _geometry, const std::set<Collider::TAG> _notHitTags = {});

	//全当たり判定の消去
	void DeleteAllCollider(void);
};

