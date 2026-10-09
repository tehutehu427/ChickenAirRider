#pragma once
#include<string>
#include<unordered_set>
#include<memory>
#include<vector>
#include<array>
#include<DxLib.h>
#include<unordered_map>
#include"../../Common/Singleton.h"
#include"../../Common/Quaternion.h"
#include"../../Object/Common/Collider.h"
#include"AttackData.h"

class AttackManager : public Singleton<AttackManager>
{
	//シングルトン化のため、Singletonクラスをフレンドクラスに指定
	friend class Singleton<AttackManager>;

public:

	//外部読み込み
	void LoadOutSide(void)override;

	/// <summary>
	/// 攻撃用コライダーの登録
	/// </summary>
	/// <param name="_col">攻撃用コライダ</param>
	/// <param name="_data">攻撃データ</param>
	void SetAttackCollider(const Collider* _col, const AttackData& _data);

	/// <summary>
	/// コライダリストから削除
	/// </summary>
	/// <param name="_col">削除するコライダ</param>
	void DeleteAttackCollider(const Collider* _col);

	/// <summary>
	/// 攻撃コライダの当たったリストを削除する
	/// </summary>
	/// <param name="_col">攻撃コライダ</param>
	void ResetTargetColList(const Collider* _col);

	/// <summary>
	/// 攻撃が当たるか
	/// </summary>
	/// <param name="_atkCol">当たった攻撃のコライダ</param>
	/// <param name="_hitCol">当たった本体のコライダ</param>
	/// <returns>true:当たる</returns>
	const bool IsCanHit(const Collider* _atkCol, const Collider* _hitCol);

	/// <summary>
	/// 攻撃情報を取得
	/// </summary>
	/// <param name="_atkCol">当たった攻撃のコライダ</param>
	/// <param name="_hitCol">当たった本体のコライダ</param>
	/// <returns>攻撃情報</returns>
	const AttackData& GetAttackData(const Collider* _atkCol, const Collider* _hitCol);

private:

	//コンストラクタ
	AttackManager(void);

	//デストラクタ
	~AttackManager(void)override;

	//削除
	void Destroy(void)override;

	//攻撃コライダが登録されているか
	const bool IsRegisterCollider(const Collider* _col);

	std::unordered_map<const Collider*, std::unordered_set<const Collider*>> attackColliderHitList_;	//攻撃判定用コライダーリスト
	std::unordered_map<const Collider*, AttackData> attackDatas_;										//各攻撃の情報リスト
};