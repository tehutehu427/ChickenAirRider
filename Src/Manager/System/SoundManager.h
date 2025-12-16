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
		NONE = 1,					//なし

		//BGM
		SELECT_BGM,		//セレクトシーン
		MAIN_GAME_BGM,	//メインゲーム
		LAST_GAME_BGM,	//最終ミニゲーム
		RESULT_BGM,		//リザルトシーン

		//SE
		TITLE_SE,	//タイトル
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

	//音源種類最大数
	static constexpr int TYPE_MAX = static_cast<int>(TYPE::MAX);

	//音源の最大音量
	static constexpr int PERCENT_MAX = 100;

	//解放
	void Destroy(void)override;

	//初期化
	void Init(void)override;

	/// @brief サウンドの追加
	/// @param _name サウンド名
	/// @param _id サウンドのID
	/// @param _soundType サウンドの種類
	/// @return true：追加完了
	const bool Add(const SOUND_NAME _name, const int _id, const TYPE _soundType, const int _volumePercent = PERCENT_MAX);

	/// @brief 音源の再生
	/// @param _src リソース種類
	/// @param _playType 再生種類
	void Play(const SOUND_NAME _name, const PLAYTYPE _playType);

	/// @brief 音源の停止
	/// @param _src リソース種類
	void Stop(const SOUND_NAME _name);

	//全音源の停止
	void StopAll(void);

	/// @brief 再生中かを返す
	/// @param _src リソース種類
	/// @return trueの場合再生中,falseの場合再生していない
	bool IsPlay(const SOUND_NAME _name) const;

	/// @brief 音源単体ごとの音量
	/// @param _name 音源の名前
	/// @param _volumePercent 音量(%表記)
	void SetVolume(const SOUND_NAME _name, const int _volumePercent) { info_[_name].volumePercent = _volumePercent; }

	//読み込んだ音量を設定する
	void SetLoadedSoundsVolume(void) { for (int i = 0; i < TYPE_MAX; i++) { SetSystemVolume(volume_[i], i); } };

	/// @brief 音量の設定
	/// @param _volumePercent 音量パーセント
	/// @param _type サウンド種類
	void SetSystemVolume(const int _volumePercent, const int _type);

	/// @brief 音量を返す
	/// @param _type サウンド種類
	/// @return 指定したサウンド種類の音量を返す
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
	int volume_[TYPE_MAX];

	//管理対象
	std::unordered_map<SOUND_NAME, SoundInfo> info_;

	// コンストラクタ
	SoundManager(void);

	//デストラクタ
	~SoundManager(void)override;

	//再生種類を取得
	int GetPlayType(const PLAYTYPE _playType);
};