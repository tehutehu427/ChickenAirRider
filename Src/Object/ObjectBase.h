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
class ModelMaterial;
class ModelRenderer;

class ObjectBase
{
public:

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

	/// <summary>
	/// それぞれの当たり判定後の処理
	/// </summary>
	/// <param name="_hitCol">相手側の当たり判定タグ</param>
	virtual void OnHit(const Collider* _hitCol) = 0;

	//モデル情報の取得
	const Transform& GetTrans(void) const { return trans_; }

	//コライダの取得
	const std::vector<std::unique_ptr<Collider>>& GetColliders(void) { return collider_; };

	//モデルの色を変える
	virtual void ChangeModelColor(const COLOR_F _colorScale);

protected:

	// シングルトン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// モデル制御の基本情報
	Transform trans_;

	//モデルの頂点シェーダー
	std::unique_ptr<ModelRenderer> modelRenderer_;

	//当たり判定関係
	std::vector<std::unique_ptr<Collider>> collider_;	//全体の当たり判定情報

	//エフェクト
	std::unique_ptr<EffectController> effect_;

	/// <summary>
	/// 当たり判定作成(形状情報作成後)
	/// </summary>
	/// <param name="_tag">自身の当たり判定タグ</param>
	/// <param name="_geometry">自身の形状情報</param>
	/// <param name="_notHitTags">衝突させないタグ</param>
	void MakeCollider(const Collider::TAG& _tag, std::unique_ptr<Geometry> _geometry, const std::set<Collider::TAG> _notHitTags = {});

	//該当タグで当たり判定設定
	void SetIsEnabledByTag(const Collider::TAG& _tag, const bool _isEnabled);

	//全当たり判定の消去
	void SetIsEnabledByAll(const bool _isEnabled);
};

