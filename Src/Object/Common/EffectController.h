#pragma once

#include<vector>
#include<map>
#include"../Common/Quaternion.h"

class EffectController
{
public:

	//エフェクトの種類
	enum class EFF_TYPE
	{
		NONE,
		WIND,		//風
		BLAST,		//爆発
		DASH,		//ダッシュ
		JUMP,		//ジャンプ
		LANDING,	//着地
		PUNCH_HIT,	//パンチヒット
		COIN_GOAL,	//コインのゴール
	};

	//コンストラクタ
	EffectController(void);

	//デストラクタ
	~EffectController(void);

	//更新関係
	void Update(void);

	/// @brief エフェクトの追加
	/// @param _effHandle エフェクトハンドル
	/// @param _effType ハンドルを保存したいタイプ
	void Add(const int _effHandle, const EFF_TYPE _effType);

	/// @brief エフェクトの再生
	/// @param _effType エフェクトの種類
	/// @param _pos 座標情報
	/// @param _quaRot 回転情報
	/// @param _scl 大きさ情報
	/// @param _isLoop ループの有無
	/// @param _speedMultiplier 速度の倍率
	/// @return 再生したエフェクトの配列番号(このエフェクトの設定をいじる時に使用), -1:失敗
	const int Play(const EFF_TYPE _effType,
		const VECTOR _pos,
		const Quaternion _quaRot,
		const VECTOR _scl,
		const bool _isLoop = false,
		const float _speedMultiplier = NORMAL_MULTI);

	/// @brief 再生中エフェクトを止める
	/// @param _effType 止めたいエフェクトの種類
	/// @param _arrayNum 止めたいエフェクトの配列番号
	void Stop(const EFF_TYPE _effType, const int _arrayNum);

	/// @brief エフェクトの削除
	/// @param _effType 削除するエフェクトの種類
	/// @param _arrayNum 削除するエフェクトの配列番号
	void Delete(const EFF_TYPE _effType, const int _arrayNum);

	//全停止
	void AllStop(void);

	//全削除
	void AllDelete(void);

	/// @brief 再生が終わっているか
	/// @param _effType 確認したいエフェクトの種類
	/// @param _arrayNum 確認したいエフェクトの配列番号
	/// @return true:再生が終わった
	const bool IsEnd(const EFF_TYPE _effType, const int _arrayNum);
	
	/// @brief 指定したエフェクトがいくつ再生されているか
	/// @param _effType 確認したいエフェクト
	/// @return プレイされている数(-1:そもそもそのエフェクトが存在しない)
	const int GetPlayNum(const EFF_TYPE _effType);

	/// @brief 座標の再設定
	/// @param _effType エフェクトの種類
	/// @param _arrayNum 変更したいエフェクトの配列番号
	/// @param _pos 変更後の座標情報
	void SetPos(const EFF_TYPE _effType, const int _arrayNum, const VECTOR _pos);
	
	/// @brief 回転の再設定
	/// @param _effType エフェクトの種類
	/// @param _arrayNum 変更したいエフェクトの配列番号
	/// @param _quaRot 変更後の回転情報
	void SetQuaRot(const EFF_TYPE _effType, const int _arrayNum, const Quaternion _quaRot);
	
	/// @brief 大きさの再設定
	/// @param _effType エフェクトの種類
	/// @param _arrayNum 変更したいエフェクトの配列番号
	/// @param _scl 変更後の大きさ
	void SetScale(const EFF_TYPE _effType, const int _arrayNum, const VECTOR _scl);

	/// @brief 速度の再設定
	/// @param _effType エフェクトの種類
	/// @param _arrayNum 変更したいエフェクトの配列番号
	/// @param _speedMultiplier 変更後の速度倍率 
	void SetSpeed(const EFF_TYPE _effType, const int _arrayNum, const float _speedMultiplier);

private:

	//プレイデータ
	struct PlayData
	{
		int playId;			//再生データ
		bool isLoop;		//ループの有無
		VECTOR pos;			//座標
		Quaternion quaRot;	//回転
		VECTOR scl;			//大きさ
		float speedMulti;	//速度倍率
	};

	//エフェクトデータ
	struct EffectData
	{
		int resId;							//リソースデータ
		std::map<int, PlayData> playData;	//プレイ中のデータ
		int playNum;						//プレイ中のエフェクトの数
	};

	static constexpr float NORMAL_MULTI = 1.0f;	//通常倍率

	std::map<EFF_TYPE, EffectData> effects_;	//エフェクト関連

	/// @brief もう一度再生
	/// @param _effType もう一度再生させるエフェクトの種類
	/// @param _arrayNum もう一度再生させる配列
	void RePlay(const EFF_TYPE _effType, const int _arrayNum);
};

