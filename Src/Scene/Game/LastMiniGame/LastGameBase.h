#pragma once
#include "../GameBase.h"

class SkyDome;

class LastGameBase : public GameBase
{
public:

	//コンストラクタ
	LastGameBase(SceneGame& _parent);

	//デストラクタ
	virtual ~LastGameBase(void)override;

	//初期化
	virtual void Init(void)override;

	//更新
	virtual void Update(void)override;

	//描画
	virtual void Draw(void)override;

	//解放
	virtual void Release(void)override;

	//プレイヤーの順位取得
	const int GetRank(const int _playerIndex) { return ranks_[_playerIndex]; }

	//順位決定
	void ConfirmRank(const int _playerIndex);

	//順位が決まったかどうか
	const bool IsComfirmRanks(void) { return nowRank_ < 2; };

protected:

	//プレイヤーの順位
	std::unordered_map<int, int> ranks_;

	//現在順位
	int nowRank_;

	//デバッグ描画
	virtual void DebugDraw(void)override;

};

