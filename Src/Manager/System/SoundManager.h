#pragma once
#include <unordered_map>
#include <string>
#include "../Common/Singleton.h"

class SoundManager : public Singleton<SoundManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<SoundManager>;

public:

	//サウンドの名前
	enum class SOUND_NAME
	{
		NONE = 1,		//なし

		//BGM
		TITLE_BGM,		//タイトルBGM	
		SELECT_BGM,		//セレクトシーン
		MAIN_GAME_BGM,	//メインゲーム
		LAST_GAME_BGM,	//最終ミニゲーム
		RESULT_BGM,		//リザルトシーン

		//SE
		ENTER,		//決定音
		SELECT_SE,	//選択音
		CANCEL,		//キャンセル音

		ENGINE,		//エンジン音
		CHARGE,		//チャージ
		CHARGE_MAX,	//チャージ完了
		BOOST,		//ブースト
		BEAM,		//ビーム
		CANNON,		//大砲
		DAMAGE,		//ダメージ
		GET_ITEM,	//アイテムゲット
		HEAL,		//回復
		JUMP,		//ジャンプ
		SPIN,		//スピン

		COUNT_DOWN_SE,	//カウントダウン
		TIME_UP_SE,		//タイムアップ

		MAX
	};

	//再生種類
	enum class TYPE
	{
		BGM,				//BGM
		SE,					//効果音
		MAX
	};

	//再生種類
	enum class PLAYTYPE
	{
		NORMAL,	//ノーマル再生
		LOOP,	//ループ再生
		BACK	//バックグラウンド再生
	};

	//デフォルトの音量
	static constexpr int DEFAULT_VOLUME = 70;

	//音量関係
	static constexpr int TITLE_BGM_VOL = 80;
	static constexpr int OPTION_SE_VOL = 80;
	static constexpr int ENGINE_SE_VOL = 140;
	static constexpr int CHARGE_SE_VOL = 80;
	static constexpr int CHARGE_MAX_SE_VOL = 80;
	static constexpr int SPIN_SE_VOL = 130;
	static constexpr int BOOST_SE_VOL = 180;

	//システムの最大音量
	static constexpr int VOLUME_MAX = 255;  

	//音源の最大音量
	static constexpr int PERCENT_MAX = 100;

	//解放
	void Destroy(void)override;

	/// <summary>
	/// サウンドの追加
	/// </summary>
	/// <param name="_name">サウンド名</param>
	/// <param name="_id">サウンドのID</param>
	/// <param name="_soundType">サウンドの種類</param>
	/// <param name="_volumePercent">音量(%表記)</param>
	/// <returns>true：追加完了</returns>
	const bool Add(const SOUND_NAME _name, const int _id, const TYPE _soundType, const int _volumePercent = PERCENT_MAX);

	/// <summary>
	/// 音源の再生
	/// </summary>
	/// <param name="_name">サウンド名</param>
	/// <param name="_playType">再生タイプ</param>
	void Play(const SOUND_NAME _name, const PLAYTYPE _playType);

	/// <summary>
	/// 音源の停止
	/// </summary>
	/// <param name="_name">サウンド名</param>
	void Stop(const SOUND_NAME _name);

	//全音源の停止
	void StopAll(void);

	/// <summary>
	/// 再生中かを返す
	/// </summary>
	/// <param name="_name">サウンド名</param>
	/// <returns>true:再生中</returns>
	bool IsPlay(const SOUND_NAME _name) const;
	
	/// <summary>
	/// 音源単体ごとの音量の再設定
	/// </summary>
	/// <param name="_name">サウンド名</param>
	/// <param name="_volumePercent">音量(%表記)</param>
	void SetVolume(const SOUND_NAME _name, const int _volumePercent);

	//読み込んだ音量を設定する
	void SetLoadedSoundsVolume(void) { for (int i = 0; i < static_cast<int>(TYPE::MAX); i++) { SetSystemVolume(volume_[i], i); } };

	/// <summary>
	/// 種類ごとの音量の設定
	/// </summary>
	/// <param name="_volumePercent">音量(%表記)</param>
	/// <param name="_type">サウンド種類</param>
	void SetSystemVolume(const int _volumePercent, const int _type);

	/// <summary>
	/// 音量を返す
	/// </summary>
	/// <param name="_type">サウンド種類</param>
	/// <returns>指定したサウンド種類の音量を返す</returns>
	const int GetSoundTypeVolume(const int _type) const { return volume_[_type]; }

private:

	//サウンドリソース
	struct SoundInfo
	{
		int handleId = -1;			//音源ハンドルID
		TYPE type = TYPE::MAX;		//音源の種類
		int volumePercent = 100;	//音源の音量(%表記)
	};		
		
	//ボリューム
	int volume_[static_cast<int>(TYPE::MAX)];

	//管理対象
	std::unordered_map<SOUND_NAME, SoundInfo> info_;

	// コンストラクタ
	SoundManager(void);

	//デストラクタ
	~SoundManager(void)override;

	//読み込み
	void LoadOutSide(void)override;

	//再生種類を取得
	int GetPlayType(const PLAYTYPE _playType);
};