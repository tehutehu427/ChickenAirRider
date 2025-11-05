#pragma once
#include <unordered_map>
#include <string>
#include "../Common/Singleton.h"

class SoundManager : public Singleton<SoundManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<SoundManager>;

public:

	/// <summary>
	/// リソース種類
	/// </summary>
	enum class SRC
	{
		NONE,					//なし

		//BGM

		//SE
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

	//解放
	void Destroy(void)override;

	//リソースの解放
	void Release(void);

	//初期化
	void Init(void)override;

	/// @brief リソースの読み込み
	/// @param _src リソース種類
	/// @return trueの場合読み込み成功,falseの場合失敗
	const bool LoadResource(const SRC _src);

	/// @brief 音源の再生
	/// @param _src リソース種類
	/// @param _playType 再生種類
	void Play(const SRC _src, const PLAYTYPE _playType);

	/// @brief 音源の停止
	/// @param _src リソース種類
	void Stop(const SRC _src);

	/// @brief 再生中かを返す
	/// @param _src リソース種類
	/// @return trueの場合再生中,falseの場合再生していない
	bool IsPlay(const SRC _src) const;

	//読み込んだ音量を設定する
	const void SetLoadedSoundsVolume() { for (int i = 0; i < TYPE_MAX; i++) { SetSystemVolume(volume_[i], i); } };

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
	struct SoundResource
	{
		int handleId = -1;			//音源ハンドルID
		TYPE type = TYPE::MAX;		//音源の種類
		std::wstring path = L"";	//音源のパス
	};		
		
	//ボリューム
	int volume_[TYPE_MAX];

	//管理対象
	std::unordered_map<SRC, SoundResource> resourcesMap_;

	//読み込み済み
	std::unordered_map<SRC, SoundResource> loadedMap_;

	// コンストラクタ
	SoundManager(void);

	//デストラクタ
	~SoundManager(void)override;

	//内部読み込み処理
	bool _Load(const SRC _src);

	//再生種類を取得
	int GetPlayType(const PLAYTYPE _playType);
};