#include"../../pch.h"
#include "AttackManager.h"

void AttackManager::LoadOutSide(void)
{
}

void AttackManager::AddAttackCollider(const ATTACK_TYPE& _name, const Collider* _col)
{
	//登録コライダのポインタ
	if (!_col)return;

	if (IsRegisterCollider(_col))
	{
		//エラー防止
		assert(!"すでに登録しているものを再登録しようとしています");
		return;
	}

	//登録
	colliderAttackTypeList_[_col].name = _name;
}

void AttackManager::DeleteAttackCollider(const Collider* _col)
{
	//削除コライダ
	if (!_col)return;

	//一致したものを消す
	colliderAttackTypeList_.erase(_col);
}

void AttackManager::ResetTargetColList(const Collider* _col)
{
	//攻撃コライダ
	if (!_col)return;

	//含まれているかを探す
	if (!IsRegisterCollider(_col))
	{
		//見つからなかった
		return;
	}

	//リセット
	colliderAttackTypeList_[_col].targetCol.clear();
}

const bool AttackManager::IsCanHit(const Collider* _atkCol, const Collider* _hitCol)
{
	//コライダのポインタ
	if (!_atkCol || !_hitCol)return false;

	//含まれているかを探す
	if (!IsRegisterCollider(_atkCol))
	{
		//見つからなかった
		assert(!"選択されたコライダは登録されていません");
		return false;
	}

	//単体ヒット　かつ　既に攻撃済みリストに当たったコライダが登録されているかを調べる
	bool isMultiHit = attackDatas_[static_cast<int>(colliderAttackTypeList_[_atkCol].name)].isMultiHit;
	bool isRegist = colliderAttackTypeList_[_atkCol].targetCol.contains(_hitCol);
	if (!isMultiHit && isRegist)
	{
		//当たらない
		return false;
	}

	//当たる
	return true;
}

const AttackManager::AttackData& AttackManager::GetAttackData(const Collider* _atkCol, const Collider* _hitCol)
{
	//コライダのポインタ
	if (!_atkCol || !_hitCol)return AttackData();

	//含まれているか
	if (!IsRegisterCollider(_atkCol))
	{
		assert(!"選択されたコライダは登録されていません");
		return {};
	}

	//見つかったので攻撃済みリストに当たった側を保存する
	colliderAttackTypeList_[_atkCol].targetCol.insert(_hitCol);

	//攻撃情報を返す
	return attackDatas_[static_cast<int>(colliderAttackTypeList_[_atkCol].name)];
}

void AttackManager::SetAttackData(const ATTACK_TYPE& _name, const AttackData& _data)
{
	//攻撃情報を上書き
	attackDatas_[static_cast<int>(_name)] = _data;
}

AttackManager::AttackManager(void)
{
}

AttackManager::~AttackManager(void)
{
}

void AttackManager::Destroy(void)
{
	//全削除
	colliderAttackTypeList_.clear();
	attackDatas_.fill(AttackData());
}

const bool AttackManager::IsRegisterCollider(const Collider* _col)
{
	//含まれているかを探す
	auto colPtr = _col;
	if (!colliderAttackTypeList_.contains(colPtr))
	{
		//見つからなかった
		return false;
	}

	//見つかった
	return true;
}