#pragma once
#include <memory>
#include <chrono>
#include <vector>
#include <list>
#include <map>
#include <functional>
#include "../../Common/Fader.h"

class Grid;
class SceneBase;
class Fader;
class Camera;

class SceneManager
{

public:

	// シーン管理用
	enum class SCENE_ID
	{
		NONE = -1,
		TITLE,		//タイトル
		SELECT,		//セレクト
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
	
	//インスタンスを生成
	static void CreateInstance(void);

	//インスタンスを取得
	static SceneManager& GetInstance(void);

	//初期化
	void Init(void);

	//3D関連の初期化
	void Init3D(void);

	//更新処理
	void Update(void);

	//描画処理
	void Draw(void);

	/// <summary>
	/// 先頭の（Updateが呼ばれる）シーンを切り替える
	/// </summary>
	/// <param name="_sceneId">切り替え先のシーンID</param>
	/// <param name="_isFade">フェードの有無(true:フェードあり)</param>
	void ChangeScene(const SCENE_ID _sceneId, const bool _isFade = false);

	/// <summary>
	/// すべてのシーンを切り替える
	/// </summary>
	/// <param name="_sceneId">切り替え先のシーンID</param>
	void ChangeAllScene(const SCENE_ID _sceneId);

	/// <summary>
	/// シーンをプッシュする。スタックの数が増える
	/// 一番上のシーンのUpdateしか呼ばれません。
	/// </summary>
	/// <param name="_sceneId">積むシーンID</param>
	/// <param name="_isFade">フェードの有無(true:フェードあり)</param>
	void PushScene(const SCENE_ID _sceneId, const bool _isFade = false);

	//スタックの頭のシーンを削除する。
	//ただし、スタック上にシーンが一つしかない場合は、削除しない。
	void PopScene(void);

	//リソースの破棄
	void Destroy(void);

	/// <summary>
	/// シーンの状態遷移
	/// </summary>
	/// <param name="nextId">遷移先のシーン</param>
	void ChangeScene(SCENE_ID nextId);
	
	/// <summary>
	/// フェード開始
	/// </summary>
	/// <param name=""></param>
	void StartFadeIn(void);

	/// <summary>
	/// 分割描画の設定
	/// </summary>
	/// <param name="_isSplitMode">分割する場合true、しない場合false</param>
	void SetIsSplitMode(const bool _isSplitMode) { isSplitMode_ = _isSplitMode; }

	/// <summary>
	/// シーンIDを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>シーンID</returns>
	SCENE_ID GetSceneID(void) const { return sceneId_; }

	//メインスクリーンの取得
	int GetMainScreen() const { return mainScreen_; }
	
	//スクリーンの取得
	int GetScreen(int _screenNum) const { return splitScreens_[_screenNum]; }

	//デルタタイムの取得
	float GetDeltaTime(void) const { return deltaTime_; }

	//経過時間の所得
	float GetTotalTime(void) const { return totalTime_; }

	/// <summary>
	/// カメラの取得
	/// </summary>
	/// <param name="_playerIndex">プレイヤー番号</param>
	/// <returns>指定したプレイヤー番号のカメラ</returns>
	std::weak_ptr<Camera> GetCamera(const int _playerIndex) const;

	//スクリーンの番号の取得
	int GetScreenIndex(void)const { return screenIndex_; }

private:

	//条件人数
	static constexpr int CASE_VALUE = 3;

	// 静的インスタンス
	static SceneManager* instance_;

	//シーンID
	SCENE_ID sceneId_;		//現在シーンID
	SCENE_ID waitSceneId_;	//待機シーンID

	//シーン変更管理
	CHANGE_SCENE_STATE changeSceneState_;

	//ポインタ
	std::unique_ptr<Grid> grid_;					//デバッググリッド
	std::list<std::unique_ptr<SceneBase>> scene_;	//各種シーン	
	std::unique_ptr<Fader> fader_;					//フェード
	std::vector<std::shared_ptr<Camera>> cameras_;	//カメラ

	// シーン遷移中判定
	bool isSceneChanging_;

	//時間
	std::chrono::system_clock::time_point preTime_;		//前の時間
	float deltaTime_;									//デルタタイム
	float totalTime_;									//経過時間

	//スクリーン
	int mainScreen_;					//メインスクリーン
	bool isSplitMode_;					//画面分割を行うか
	int screenIndex_ ;					//分割スクリーンのインデックス
	std::vector<int> splitScreens_;		//分割用スクリーン

	//関数ポインタ
	std::map<SCENE_ID, std::function<std::unique_ptr<SceneBase>(void)>> createScene_;	//シーン作成用
	std::map<CHANGE_SCENE_STATE, std::function<void(void)>> changeScene_;				//シーン変更用
	std::map<Fader::STATE, std::function<void(void)>> fadeState_;						//フェード用

	//カメラ生成
	void CreateCameras(const int _playerNum);

	//分割スクリーンの生成
	void CreateSplitScreen(const int _playerNum);
	
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager(void);
	// コピーコンストラクタも同様
	SceneManager(const SceneManager& manager) = default;
	// デストラクタも同様
	~SceneManager(void);

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

	//マルチ画面の描画
	void DrawMultiScreen();
};
