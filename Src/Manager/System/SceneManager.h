#pragma once
#include <memory>
#include <chrono>
#include <vector>
#include <list>
#include <map>
#include <functional>
#include "../../Common/Singleton.h"
#include "../../Common/Fader.h"

class SceneBase;
class Fader;
class Camera;

class SceneManager : public Singleton<SceneManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<SceneManager>;

public:

	// シーン管理用
	enum class SCENE_ID
	{
		NONE = -1,
		TITLE,		//タイトル
		SELECT,		//セレクト
		OPTION,		//オプション
		GAME,		//ゲーム
		RESULT,		//リザルト
		MAX
	};

	//シーン変更管理用
	enum class CHANGE_SCENE_STATE
	{
		NONE = -1,
		PUSH_BACK,		//末尾追加
		POP_BACK,		//末尾削除
		CHANGE_BACK,	//末尾変更
	};
	
	//初期化
	void Init(void)override;

	//3D関連の初期化
	void Init3D(void);

	//更新処理
	void Update(void);

	//描画処理
	void Draw(void);

	//リソースの破棄
	void Destroy(void)override;

	/// @brief 先頭の（Updateが呼ばれる）シーンを切り替える
	/// @param _sceneId 切り替え先のシーンID
	/// @param _isReset シーンをリセットするか(true:リセットする)
	/// @param _isFade フェードの有無(true:フェードあり)
	void ChangeScene(const SCENE_ID _sceneId, const bool _isReset, const bool _isFade = false);

	/// @brief すべてのシーンを切り替える
	/// @param _sceneId 切り替え先のシーンID
	void ChangeAllScene(const SCENE_ID _sceneId);

	/// <summary>
	/// シーンをプッシュする。スタックの数が増える
	/// 一番上のシーンのUpdateしか呼ばれません。
	/// </summary>
	/// <param name="_sceneId">積むシーンID</param>
	/// <param name="_isFade">フェードの有無(true:フェードあり)</param>

	/// @brief シーンをプッシュする。スタックの数が増える
	/// 一番上のシーンのUpdateしか呼ばれません。
	/// @param _sceneId 
	/// @param _isFade 
	void PushScene(const SCENE_ID _sceneId, const bool _isFade = false);

	//スタックの頭のシーンを削除する。
	//ただし、スタック上にシーンが一つしかない場合は、削除しない。
	void PopScene(void);

	//シーンをリセットする
	void ResetScene(void);

	/// <summary>
	/// シーンの状態遷移
	/// </summary>
	/// <param name="nextId">遷移先のシーン</param>
	//void ChangeScene(SCENE_ID nextId);
	
	//フェード開始
	void StartFadeIn(void);

	//シーンIDの取得
	const SCENE_ID GetSceneID(void) const { return sceneId_; }

	//デルタタイムの取得
	const float GetDeltaTime(void) const { return deltaTime_; }

	//経過時間の所得
	const float GetTotalTime(void) const { return totalTime_; }

	/// @brief カメラの取得
	/// @param _playerIndex プレイヤー番号
	/// @return 指定したプレイヤー番号のカメラ
	std::weak_ptr<Camera> GetCamera(const int _playerIndex) const;

private:

	//シーンID
	SCENE_ID sceneId_;		//現在シーンID
	SCENE_ID waitSceneId_;	//待機シーンID

	//シーン変更管理
	CHANGE_SCENE_STATE changeSceneState_;

	//ポインタ
	std::list<std::unique_ptr<SceneBase>> scene_;	//各種シーン	
	std::unique_ptr<Fader> fader_;					//フェード
	std::vector<std::shared_ptr<Camera>> cameras_;	//カメラ

	// シーン遷移中判定
	bool isSceneChanging_;

	//時間
	std::chrono::system_clock::time_point preTime_;		//前の時間
	float deltaTime_;									//デルタタイム
	float totalTime_;									//経過時間

	//関数ポインタ
	std::map<SCENE_ID, std::function<std::unique_ptr<SceneBase>(void)>> createScene_;	//シーン生成用
	std::map<CHANGE_SCENE_STATE, std::function<void(void)>> changeScene_;				//シーン変更用
	std::map<Fader::STATE, std::function<void(void)>> fadeState_;						//フェード用

	//シーン生成
	std::unique_ptr<SceneBase> CreateSceneTitle(void);
	std::unique_ptr<SceneBase> CreateSceneSelect(void);
	std::unique_ptr<SceneBase> CreateSceneOption(void);
	std::unique_ptr<SceneBase> CreateSceneGame(void);
	std::unique_ptr<SceneBase> CreateSceneResult(void);

	//シーン変更
	void ChangeScenePushBack(void);
	void ChangeScenePopBack(void);
	void ChangeSceneChangeBack(void);

	//フェード
	void FadeNone(void);
	void FadeOut(void);
	void FadeIn(void);

	//カメラと分割スクリーンの生成
	void CreateCameraAndSplitScreens(const int _playerNum);
	
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager(void);
	// デストラクタも同様
	~SceneManager(void)override;

	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	// シーン遷移
	void DoChangeScene(SCENE_ID sceneId);

	/// <summary>
	/// シーン変更時のリセット等
	/// </summary>
	/// <param name="_isFade">フェードの有無(true:フェードあり)</param>
	void ResetChangeScene(const bool _isFade);

	// フェード
	void Fade(void);
};
