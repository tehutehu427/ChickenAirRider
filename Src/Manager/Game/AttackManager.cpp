#include"../../pch.h"
#include "AttackManager.h"

void AttackManager::LoadOutSide(void)
{
}

void AttackManager::SetAttackCollider(const Collider* _col, const AttackData& _data)
{
	//登録コライダのポインタ
	if (!_col)return;

	//攻撃情報を設定
	attackDatas_[_col] = _data;
}

void AttackManager::DeleteAttackCollider(const Collider* _col)
{
	//削除コライダ
	if (!_col)return;

	//一致したものを消す
	attackColliderHitList_.erase(_col);
	attackDatas_.erase(_col);
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
	attackColliderHitList_[_col].clear();
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
	bool isMultiHit = attackDatas_[_atkCol].isMultiHit;
	bool isRegist = attackColliderHitList_[_atkCol].contains(_hitCol);
	if (!isMultiHit && isRegist)
	{
		//当たらない
		return false;
	}

	//当たる
	return true;
}

const AttackData& AttackManager::GetAttackData(const Collider* _atkCol, const Collider* _hitCol)
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
	attackColliderHitList_[_atkCol].insert(_hitCol);

	//攻撃情報を返す
	return attackDatas_[_atkCol];
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
	attackColliderHitList_.clear();
	attackDatas_.clear();
}

const bool AttackManager::IsRegisterCollider(const Collider* _col)
{
	//含まれているかを探す
	auto colPtr = _col;
	if (!attackColliderHitList_.contains(colPtr))
	{
		//見つからなかった
		return false;
	}

	//見つかった
	return true;
}