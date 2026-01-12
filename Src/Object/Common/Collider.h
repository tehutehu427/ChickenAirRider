#pragma once

#include<set>
#include"Geometry/Geometry.h"

class ObjectBase;

class Collider
{

public :

	// 衝突用タグ
	enum class TAG
	{
		//-----------------------------------------------------------------
		// 追加後は必ずマネージャーにて当たり判定範囲を設定してください
		//-----------------------------------------------------------------

		PLAYER1 = 0,	//プレイヤー1
		PLAYER2 = 1,	//プレイヤー2
		PLAYER3 = 2,	//プレイヤー3
		PLAYER4 = 3,	//プレイヤー4

		FOOT,			//足元

		DAMAGE,			//ダメージ

		MACHINE,		//機体
		MACHINE_RIDE,	//機体の乗り判定

		NORMAL_OBJECT,	//通常オブジェ
		GROUND,			//地面
		TREE,			//木

		ITEM_BOX,		//アイテムボックス
		POWER_UP,		//パワーアップアイテム
		BATTLE_ITEM,	//バトルアイテム
	};

	/// @brief コンストラクタ
	/// @param _parent 親情報
	/// @param _tags 自身の衝突用タグ
	/// @param _geometry 当たり判定の形状
	/// @param _notHitTags 衝突させないタグ
	Collider(ObjectBase& _parent, const TAG _tag, std::unique_ptr<Geometry> _geometry, const std::set<TAG> _notHitTags);

	// デストラクタ
	~Collider(void);

	//衝突用タグの取得
	inline const TAG GetTag(void)const { return myTag_; }

	//当たり判定の形状を取得
	inline Geometry& GetGeometry(void)const { return *geometry_; }

	//衝突させないタグの取得
	inline const std::set<TAG>& GetNotHitTags(void)const { return notHitTags_; }

	//親を取得
	inline const ObjectBase& GetParent(void)const { return parent_; }

	//当たったかの判定の取得
	inline const bool IsHit(void)const { return isHit_; }

	//終了判定の取得
	inline const bool IsDead(void)const { return isDead_; }

	//終了処理(所持者の解放時に置く)
	inline void Kill(void) { isDead_ = true; }

	/// @brief 当たった時の処理
	/// @param _collider 相手のコライダ
	void OnHit(const std::weak_ptr<Collider> _collider);

private:

	//親
	ObjectBase& parent_;

	// 自身の衝突用タグ
	TAG myTag_;

	// 衝突しないタグ
	std::set<TAG> notHitTags_;

	//当たり判定の形状
	std::unique_ptr<Geometry> geometry_;

	//当たったかの判定
	bool isHit_;

	//終了判定
	bool isDead_;
};
