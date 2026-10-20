#include"../pch.h"
#include "../../Manager/System/SceneManager.h"
#include "EffectController.h"

EffectController::EffectController(void)
	: effects_()
	, nextId_(0)
{
}

EffectController::~EffectController(void)
{
	//全停止
	AllDelete();
}

void EffectController::Update(void)
{
	//エフェクトの更新
	for (auto& effectPair : effects_)
	{
		//エフェクトの種類
		auto& effect = effectPair.second;

		//再生中のエフェクトの更新
		for (auto itr = effect.playData.begin(); itr != effect.playData.end();)
		{
			//エフェクトのID
			const int id = itr->first;

			//エフェクトの再生データ
			PlayData& data = itr->second;

			//ループしていないエフェクトで再生が終わっているか
			if (IsEffekseer3DEffectPlaying(data.playId) == -1)
			{
				//ループの有無
				if (data.isLoop)
				{
					//再再生
					RePlay(effectPair.first, id);

					//次の要素に
					itr++;
				}
				else
				{
					//エフェクトを終了
					StopEffekseer3DEffect(data.playId);

					//要素の削除
					itr = effect.playData.erase(itr);
				}

			}
			else
			{
				//次の要素に
				itr++;
			}
		}
	}
}

void EffectController::Add(const int _effHandle, const EFF_TYPE _effType)
{
	//エフェクトの要素が存在するか
	if (effects_.count(_effType))
	{
		//既に存在したので何もしない
		return;
	}

	//エフェクト
	EffectData effect;

	//リソースID
	effect.resId = _effHandle;

	//追加
	effects_.emplace(_effType, effect);
}

const int EffectController::Play(const EFF_TYPE _effType, const VECTOR _pos, const Quaternion _quaRot, const VECTOR _scl, const bool _isLoop, const float _speedMultiplier)
{
	//エフェクトの要素を取得
	auto effectItr = effects_.find(_effType);

	//エフェクトの要素が存在するか
	if (effectItr == effects_.end())
	{
		//その要素がなかったので失敗
		return -1;
	}

	//エフェクトのデータを取得
	EffectData& effectData = effectItr->second;

	//管理用のIDを発行
	int id = nextId_;

	//次のIDを更新
	nextId_++;

	//プレイハンドルを設定
	PlayData playData;
	playData.playId = PlayEffekseer3DEffect(effectData.resId);

	//以降、再生するエフェクトの制御は必ずプレイハンドルIDを使用すること
	//エフェクトの大きさ設定(XYZ)
	SetScalePlayingEffekseer3DEffect(playData.playId, _scl.x, _scl.y, _scl.z);
	playData.scl = _scl;

	//エフェクトの角度を設定
	const VECTOR rot = _quaRot.ToEuler();
	SetRotationPlayingEffekseer3DEffect(playData.playId, rot.x, rot.y, rot.z);
	playData.quaRot = _quaRot;

	//エフェクトの位置を設定
	SetPosPlayingEffekseer3DEffect(playData.playId, _pos.x, _pos.y, _pos.z);
	playData.pos = _pos;

	//エフェクトの速度を設定
	SetSpeedPlayingEffekseer3DEffect(playData.playId, _speedMultiplier);
	playData.speedMulti = _speedMultiplier;

	//ループの有無
	playData.isLoop = _isLoop;

	//プレイエフェクトの保存
	effects_[_effType].playData.emplace(id, playData);

	//配列番号を返す
	return id;
}


void EffectController::SetPos(const EFF_TYPE _effType, const int _id, const VECTOR _pos)
{
	//エフェクトの要素を取得
	auto effectItr = effects_.find(_effType);

	//エフェクトの要素が存在するか
	if (effectItr == effects_.end())
	{
		//その要素がなかった
		return;
	}

	//エフェクトのデータを取得
	EffectData& effectData = effectItr->second;

	//エフェクトの再生要素を取得
	auto playItr = effectData.playData.find(_id);

	//エフェクトの再生要素が存在するか
	if (playItr == effectData.playData.end())
	{
		//その再生要素がなかった
		return;
	}

	//エフェクトの再生データを取得
	PlayData& playData = playItr->second;

	//座標の再設定
	SetPosPlayingEffekseer3DEffect(playData.playId, _pos.x, _pos.y, _pos.z);
	playData.pos = _pos;
}

void EffectController::SetQuaRot(const EFF_TYPE _effType, const int _id, const Quaternion _quaRot)
{
	//エフェクトの要素を取得
	auto effectItr = effects_.find(_effType);

	//エフェクトの要素が存在するか
	if (effectItr == effects_.end())
	{
		//その要素がなかった
		return;
	}

	//エフェクトのデータを取得
	EffectData& effectData = effectItr->second;

	//エフェクトの再生要素を取得
	auto playItr = effectData.playData.find(_id);

	//エフェクトの再生要素が存在するか
	if (playItr == effectData.playData.end())
	{
		//その再生要素がなかった
		return;
	}

	//エフェクトの再生データを取得
	PlayData& playData = playItr->second;

	//回転の再設定
	SetRotationPlayingEffekseer3DEffect(playData.playId, _quaRot.ToEuler().x, _quaRot.ToEuler().y, _quaRot.ToEuler().z);
	playData.quaRot = _quaRot;
}

void EffectController::SetScale(const EFF_TYPE _effType, const int _id, const VECTOR _scl)
{
	//エフェクトの要素を取得
	auto effectItr = effects_.find(_effType);

	//エフェクトの要素が存在するか
	if (effectItr == effects_.end())
	{
		//その要素がなかった
		return;
	}

	//エフェクトのデータを取得
	EffectData& effectData = effectItr->second;

	//エフェクトの再生要素を取得
	auto playItr = effectData.playData.find(_id);

	//エフェクトの再生要素が存在するか
	if (playItr == effectData.playData.end())
	{
		//その再生要素がなかった
		return;
	}

	//エフェクトの再生データを取得
	PlayData& playData = playItr->second;

	//大きさの再設定
	SetScalePlayingEffekseer3DEffect(playData.playId, _scl.x, _scl.y, _scl.z);
	playData.scl = _scl;
}

void EffectController::SetSpeed(const EFF_TYPE _effType, const int _id, const float _speedMultiplier)
{
	//エフェクトの要素を取得
	auto effectItr = effects_.find(_effType);

	//エフェクトの要素が存在するか
	if (effectItr == effects_.end())
	{
		//その要素がなかった
		return;
	}

	//エフェクトのデータを取得
	EffectData& effectData = effectItr->second;

	//エフェクトの再生要素を取得
	auto playItr = effectData.playData.find(_id);

	//エフェクトの再生要素が存在するか
	if (playItr == effectData.playData.end())
	{
		//その再生要素がなかった
		return;
	}

	//エフェクトの再生データを取得
	PlayData& playData = playItr->second;

	//速度の再設定
	SetSpeedPlayingEffekseer3DEffect(playData.playId, _speedMultiplier);
	playData.speedMulti = _speedMultiplier;
}

void EffectController::Delete(const EFF_TYPE _effType, const int _id)
{
	//エフェクトの要素を取得
	auto effectItr = effects_.find(_effType);

	//エフェクトの要素が存在するか
	if (effectItr == effects_.end())
	{
		//その要素がなかった
		return;
	}

	//エフェクトのデータを取得
	EffectData& effectData = effectItr->second;

	//エフェクトの再生要素を取得
	auto playItr = effectData.playData.find(_id);

	//エフェクトの再生要素が存在するか
	if (playItr == effectData.playData.end())
	{
		//その再生要素がなかった
		return;
	}

	//エフェクトストップ
	StopEffekseer3DEffect(playItr->second.playId);

	//指定されたエフェクトを削除
	effectData.playData.erase(_id);
}

void EffectController::AllStop(void)
{
	//全停止
	for (auto& effectPair : effects_)
	{
		//エフェクトデータ
		auto& effect = effectPair.second;

		//再生要素
		for (auto& playPair : effect.playData)
		{
			//再生データ
			PlayData& data = playPair.second;

			//停止する
			StopEffekseer3DEffect(data.playId);
		}
	}
}

void EffectController::AllDelete(void)
{
	//全停止
	AllStop();
	
	//エフェクトのクリア
	effects_.clear();

	//次に渡すIDを初期化
	nextId_ = 0;
}

void EffectController::RePlay(const EFF_TYPE _effType, const int _id)
{
	//エフェクトの要素を取得
	auto effectItr = effects_.find(_effType);

	//エフェクトの要素が存在するか
	if (effectItr == effects_.end())
	{
		//その要素がなかった
		return;
	}

	//エフェクトのデータを取得
	EffectData& effectData = effectItr->second;

	//エフェクトの再生要素を取得
	auto playItr = effectData.playData.find(_id);

	//エフェクトの再生要素が存在するか
	if (playItr == effectData.playData.end())
	{
		//その再生要素がなかった
		return;
	}

	//エフェクトの再生データを取得
	PlayData& playData = playItr->second;

	//エフェクトの停止
	StopEffekseer3DEffect(playData.playId);

	//新しく再生
	VECTOR scl = playData.scl;
	VECTOR rot = playData.quaRot.ToEuler();
	VECTOR pos = playData.pos;
	float speed = playData.speedMulti;

	//プレイハンドルを設定
	playData.playId = PlayEffekseer3DEffect(effectData.resId);

	//以降、再生するエフェクトの制御は必ずプレイハンドルIDを使用すること
	//エフェクトの大きさ設定(XYZ)
	SetScalePlayingEffekseer3DEffect(playData.playId, scl.x, scl.y, scl.z);

	//エフェクトの角度を設定
	SetRotationPlayingEffekseer3DEffect(playData.playId, rot.x, rot.y, rot.z);

	//エフェクトの位置を設定
	SetPosPlayingEffekseer3DEffect(playData.playId, pos.x, pos.y, pos.z);

	//エフェクトの速度を設定
	SetSpeedPlayingEffekseer3DEffect(playData.playId, speed);
}