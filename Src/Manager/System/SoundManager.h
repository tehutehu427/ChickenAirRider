#pragma once
#include <unordered_map>
#include <string>

class SoundManager
{
public:

	/// <summary>
	/// リソース種類
	/// </summary>
	enum class SRC
	{
		NONE,					//なし

		//BGM
		TITLE_BGM,				//タイトルBGM
		SELECT_BGM,				//セレクトBGM		
		EDIT_BGM,				//エディットBGM
		PLAY_BGM_1,				//プレイBGM1
		PLAY_BGM_2,				//プレイBGM2
		MULTI_BGM_1,			//マルチBGM1
		MULTI_BGM_2,			//マルチBGM2
		MULTI_CLEAR_BGM,		//マルチクリアBGM

		//システム効果音
		TITLE_SCENE_CHANGE,		//タイトルシーン遷移
		SELECT_SCENE_CHANGE,	//セレクトシーン遷移
		DECISION,				//決定
		CANCEL,					//キャンセル
		OK,						//OK

		//マルチモード効果音
		ROUND_JINGLE,			//ラウンドジングル
		DRUM_ROLL,				//ドラムロール
		DRUM_ROLL_END,			//ドラムロール終了
		MULTI_CLEAR_JINGLE,		//マルチクリアジングル

		//プレイヤー効果音
		PLAYER_PUNCH_HIT,		//プレイヤーパンチヒット
		PLAYER_PUNCH_MOTION,	//プレイヤーパンチモーション
		PLAYER_DASH_START,		//プレイヤーダッシュスタート
		PLAYER_JUMP,			//プレイヤージャンプ
		DELON,					//デロンSE
		SPRING_SE,				//スプリングSE
		SLIME_SE,				//スライムSE

		//エディット効果音
		CLICK_OBJECT_SE,		//クリックオブジェクトSE
		CREATE_OBJECT_SE,		//オブジェクト生成SE
		ERROR_SE,				//エラーSE
		PALETTE_CLICK,			//パレットクリックSE
		PALETTE_OPEN,			//パレット開く
		PALETTE_CLOSE,			//パレット閉じる
		EDIT_SYSTEM_ICON_CLICK,	//エディットシステムアイコンクリック
		CHANGE_PHASE_ICON_CLICK,	//フェーズアイコンクリック
		BOMB_SE,	//爆発音
		COUNTDOWN_SE,	//笛の音

		//プレイモード効果音
		GAME_OVER,			//ゲームオーバー
		CLEAR,				//クリア

		//チキン鳴き声
		CHICKEN_SE,				//チキンSE
		CHICKEN_SE_2,			//チキンSE2
		CHICKEN_SE_3,			//チキンSE3
		CHICKEN_SE_4,			//チキンSE4
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

	//デストラクタ
	~SoundManager();

	//明示的にインスタンス生成する
	static void CreateInstance();

	//静的インスタンスの取得
	static SoundManager& GetInstance();

	//解放
	void Destroy();

	//リソースの解放
	void Release();

	//初期化
	void Init();

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
	
	//静的インスタンス
	static SoundManager* instance_;
	
	//ボリューム
	int volume_[TYPE_MAX];

	//管理対象
	std::unordered_map<SRC, SoundResource> resourcesMap_;

	//読み込み済み
	std::unordered_map<SRC, SoundResource> loadedMap_;

	// コンストラクタ
	SoundManager();

	// デフォルトコンストラクタは使用不可
	SoundManager(const SoundManager&) = delete;

	// コピー代入演算子は使用不可
	SoundManager& operator=(const SoundManager&) = delete;

	//内部読み込み処理
	bool _Load(const SRC _src);

	//再生種類を取得
	int GetPlayType(const PLAYTYPE _playType);

};