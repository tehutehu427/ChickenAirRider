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
		BOOST,		//ブーストエフェクト
		CHARGE,		//チャージエフェクト
		SPIN,		//スピンエフェクト
		BLAST,		//爆発エフェクト
		MAX
	};

	//コンストラクタ
	EffectController(void);

	//デストラクタ
	~EffectController(void);

	//更新関係
	void Update(void);

	/// <summary>
	/// エフェクトの追加
	/// </summary>
	/// <param name="_effHandle">エフェクトハンドル</param>
	/// <param name="_effType">ハンドルを保存したいタイプ</param>
	void Add(const int _effHandle, const EFF_TYPE _effType);

	/// <summary>
	/// エフェクトの再生
	/// </summary>
	/// <param name="_effType">エフェクトの再生</param>
	/// <param name="_pos">座標情報</param>
	/// <param name="_quaRot">回転情報</param>
	/// <param name="_scl">大きさ情報</param>
	/// <param name="_isLoop">ループの有無</param>
	/// <param name="_speedMultiplier">速度の倍率</param>
	/// <returns>再生したエフェクトの配列番号(このエフェクトの設定をいじる時に使用), -1:失敗</returns>
	const int Play(const EFF_TYPE _effType,
		const VECTOR _pos,
		const Quaternion _quaRot,
		const VECTOR _scl,
		const bool _isLoop = false,
		const float _speedMultiplier = NORMAL_MULTI);

	/// <summary>
	/// エフェクトの削除
	/// </summary>
	/// <param name="_effType">削除するエフェクトの種類</param>
	/// <param name="_id">削除するエフェクトのID</param>
	void Delete(const EFF_TYPE _effType, const int _id);

	//全停止
	void AllStop(void);

	//全削除
	void AllDelete(void);

	/// <summary>
	/// 座標の再設定
	/// </summary>
	/// <param name="_effType">エフェクトの種類</param>
	/// <param name="_id">変更したいエフェクトのID</param>
	/// <param name="_pos">変更後の座標情報</param>
	void SetPos(const EFF_TYPE _effType, const int _id, const VECTOR _pos);

	/// <summary>
	/// 回転の再設定
	/// </summary>
	/// <param name="_effType">エフェクトの種類</param>
	/// <param name="_id">変更したいエフェクトのID</param>
	/// <param name="_quaRot">変更後の回転情報</param>
	void SetQuaRot(const EFF_TYPE _effType, const int _id, const Quaternion _quaRot);
	
	/// <summary>
	/// 大きさの再設定
	/// </summary>
	/// <param name="_effType">エフェクトの種類</param>
	/// <param name="_id">変更したいエフェクトのID</param>
	/// <param name="_scl">変更後の大きさ</param>
	void SetScale(const EFF_TYPE _effType, const int _id, const VECTOR _scl);

	/// <summary>
	/// 速度の再設定
	/// </summary>
	/// <param name="_effType">エフェクトの種類</param>
	/// <param name="_id">変更したいエフェクトのID</param>
	/// <param name="_speedMultiplier">変更後の速度倍率 </param>
	void SetSpeed(const EFF_TYPE _effType, const int _id, const float _speedMultiplier);

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
	};

	//通常倍率
	static constexpr float NORMAL_MULTI = 1.0f;	

	//エフェクトデータ
	std::map<EFF_TYPE, EffectData> effects_;	

	//次に渡すID
	int nextId_ = 0;

	/// <summary>
	/// もう一度再生
	/// </summary>
	/// <param name="_effType">もう一度再生させるエフェクトの種類</param>
	/// <param name="_arrayNum">もう一度再生させる配列</param>
	void RePlay(const EFF_TYPE _effType, const int _arrayNum);
};