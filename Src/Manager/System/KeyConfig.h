#pragma once
#include <vector>
#include <map>
#include <memory>
#include "../Common/Vector2.h"
#include"../Common/Singleton.h"

class InputManager;

class KeyConfig : public Singleton<KeyConfig>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<KeyConfig>;

public:

	// ゲームコントローラーの認識番号
	// DxLib定数、DX_INPUT_PAD1等に対応
	enum class JOYPAD_NO
	{
		KEY_PAD1 = 0,		// キー入力とパッド１入力
		PAD1,				// パッド１入力
		PAD2,				// パッド２入力
		PAD3,				// パッド３入力
		PAD4,				// パッド４入力
		INPUT_KEY = 4096	// キー入力
	};

	//入力タイプ
	enum class TYPE
	{
		KEYBOARD_MOUSE,
		PAD,
		ALL,
	};


	// ゲームコントローラーボタン
	enum class JOYPAD_BTN
	{
		RIGHTBUTTON_LEFT = 0,	// X
		RIGHTBUTTON_RIGHT,		// B
		RIGHTBUTTON_TOP,		// Y
		RIGHTBUTTON_DOWN,		// A
		R_TRIGGER,				// R_TRIGGER
		L_TRIGGER,				// L_TRIGGER
		R_BUTTON,				// R_BUTTON
		L_BUTTON,				// L_BUTTON
		START_BUTTON,			// START_BUTTON
		SELECT_BUTTON,			// SELECT_BUTTON
		LEFTBUTTON_TOP,			// 上
		LEFTBUTTON_DOWN,		// 下
		LEFTBUTTON_LEFT,		// 左
		LEFTBUTTON_RIGHT,		// 右
		LEFT_STICK,				// 左スティック押し込み
		RIGHT_STICK,			// 右スティック押し込み
		MAX
	};

	//ゲームコントローラースティック
	enum class JOYPAD_STICK
	{
		L_STICK_UP,		//左スティック上
		L_STICK_DOWN,	//左スティック下
		L_STICK_LEFT,	//左スティック左
		L_STICK_RIGHT,	//左スティック右
		R_STICK_UP,		//右スティック上
		R_STICK_DOWN,	//右スティック下
		R_STICK_LEFT,	//右スティック左
		R_STICK_RIGHT,	//右スティック右
		MAX
	};

	//マウス
	enum class MOUSE
	{
		CLICK_RIGHT,		//右クリック
		CLICK_LEFT,			//左クリック
		MOVE_LEFT,			//左移動
		MOVE_RIGHT,			//右移動
		MOVE_UP,			//上移動
		MOVE_DOWN,			//下移動
		WHEEL_FRONT,		//ホイール前(奥)回転
		WHEEL_BACK,			//ホイール後ろ(手前)回転
		MAX
	};

	//操作の種類
	enum class CONTROL_TYPE 	
	{
		ENTER,					//決定
		CANCEL,					//キャンセル
		SELECT_UP,				//選択肢の上入力
		SELECT_DOWN,			//選択肢の下入力
		SELECT_RIGHT,			//選択肢の右入力
		SELECT_LEFT,			//選択肢の左入力

		CAMERA_ZOOM_IN,			//カメラのズームイン
		CAMERA_ZOOM_OUT,		//カメラのズームアウト

		MACHINE_TURN_RIGHT,		//機体の右回転
		MACHINE_TURN_LEFT,		//機体の左回転
		MACHINE_TURN_FRONT,		//機体の前回転
		MACHINE_TURN_BACK,		//機体の後回転

		CHARACTER_MOVE_RIGHT,	//キャラクターの右移動
		CHARACTER_MOVE_LEFT,	//キャラクターの左移動
		CHARACTER_MOVE_FRONT,	//キャラクターの前移動
		CHARACTER_MOVE_BACK,	//キャラクターの後移動

		CAMERA_TURN_UP,			//カメラの前回転
		CAMERA_TURN_DOWN,		//カメラの後回転

		CAMERA_TURN_RIGHT,		//カメラの右回転(キャラクター用)
		CAMERA_TURN_LEFT,		//カメラの左回転(キャラクター用)

		PUSH_BUTTON,			//プッシュボタン
		SPECIAL_BUTTON,			//スペシャルボタン
		SPIN_ATTACK_R,			//スピンアタック
		SPIN_ATTACK_L,			//スピンアタック

		DEBUG_CHANGE_TITLE,		//デバッグ用タイトル画面に戻る
		DEBUG_CHANGE_CLEAR,		//デバッグ用クリア画面に戻る
		DEBUG_CHANGE_INPUT,		//デバッグ用入力デバイス切り替え
		DATA_INPUT,				//データを入力
		DATA_EXPORT,			//データを出力
		SELECT_SKIP,
		MAX,
	};

	//初期化
	void Init(void)override;
	
	//更新
	void Update(void);

	// リソースの破棄
	void Destroy(void)override;

	/// <summary>
	/// キーが押されているか
	/// </summary>
	/// <param name="cType">操作名</param>
	/// <param name="no">ゲームコントローラーの番号</param>
	/// <param name="type">入力タイプ</param>
	/// <returns>true:押されている</returns>
	bool IsNew(CONTROL_TYPE cType, KeyConfig::JOYPAD_NO no ,TYPE type = TYPE::ALL);

	//どれかキーが押されているか
	bool IsTrgDownAny(void);

	/// <summary>
	/// キーが押されたか(押された瞬間のみ)
	/// </summary>
	/// <param name="cType">操作名</param>
	/// <param name="no">ゲームコントローラーの番号</param>
	/// <param name="type">入力タイプ</param>
	/// <returns>true:押された</returns>
	bool IsTrgDown(CONTROL_TYPE cType, KeyConfig::JOYPAD_NO no,TYPE type = TYPE::ALL);

	/// <summary>
	/// キーが離されたか(離された瞬間のみ)
	/// </summary>
	/// <param name="cType">操作名</param>
	/// <param name="no">ゲームコントローラーの番号</param>
	/// <param name="type">入力タイプ</param>
	/// <returns>true:離された</returns>
	bool IsTrgUp(CONTROL_TYPE cType, KeyConfig::JOYPAD_NO no, TYPE type = TYPE::ALL);

	/// <summary>
	/// キーが保持されているか(離された瞬間のみ)
	/// </summary>
	/// <param name="cType">操作名</param>
	/// <param name="no">ゲームコントローラーの番号</param>
	/// <param name="_holdTime">保持時間</param>
	/// <param name="_isReset">判定後のリセットの有無</param>
	/// <param name="type">入力タイプ</param>
	/// <returns>true:押されている</returns>
	bool IsTrgHold(CONTROL_TYPE cType, KeyConfig::JOYPAD_NO no, float _holdTime, bool _isReset = false, TYPE type = TYPE::ALL);

	/// <summary>
	/// ボタンのホールド時間取得
	/// </summary>
	/// <param name="cType">操作名</param>
	/// <param name="no">コントローラー番号</param>
	/// @param type 入力タイプ
	/// <returns>入力時間</returns>
	float GetKeyTrgHoldCnt(CONTROL_TYPE cType, KeyConfig::JOYPAD_NO no, TYPE type = TYPE::ALL);
	
	/// <summary>
	/// 対応キーを追加
	/// </summary>
	/// <param name="type">キーの種類</param>
	/// <param name="key">追加したい入力(キーボード)</param>
	void Add(CONTROL_TYPE type, int key);

	/// <summary>
	/// 対応キーを追加
	/// </summary>
	/// <param name="type">キーの種類</param>
	/// <param name="key">追加したい入力(ゲームコントローラーボタン)</param>
	void Add(CONTROL_TYPE type, JOYPAD_BTN key);

	/// <summary>
	/// 対応キーを追加
	/// </summary>
	/// <param name="type">キーの種類</param>
	/// <param name="key">追加したい入力(ゲームコントローラースティック)</param>
	void Add(CONTROL_TYPE type, JOYPAD_STICK key);

	/// <summary>
	/// 対応キーを追加
	/// </summary>
	/// <param name="type">キーの種類</param>
	/// <param name="key">追加したい入力(マウス)</param>
	void Add(CONTROL_TYPE type, MOUSE key);

	// マウス座標の取得
	Vector2 GetMousePos(void) const;

	//マウスの移動量を取得
	Vector2 GetMouseMove(void) const;

	//マウスの座標を設定
	void SetMousePosScreen(void);

	//マウスの座標を設定
	void SetMousePos(const Vector2& pos);

	//ゲームコントローラーのLスティックの回転
	float GetLStickDeg(KeyConfig::JOYPAD_NO no) const;

	//ゲームコントローラーのRスティックの回転
	float GetRStickDeg(KeyConfig::JOYPAD_NO no) const;
	
	//ゲームコントローラーのLスティックの上を0.0度として角度を渡す
	Vector2 GetKnockLStickSize(KeyConfig::JOYPAD_NO no) const;

	//ゲームコントローラーのRスティックの上を0.0度として角度を渡す
	Vector2 GetKnockRStickSize(KeyConfig::JOYPAD_NO no) const;

	//指定の方向に倒れた度合い0から1000
	int PadStickOverSize(KeyConfig::JOYPAD_NO no, KeyConfig::JOYPAD_STICK stick)const;
	
	/// <summary>
	/// ゲームコントローラーを振動する
	/// </summary>
	/// <param name="_no">振動させるゲームコントローラーの番号</param>
	/// <param name="_time">ミリ秒　,-1で無限に続けることができる(STOP 必須)</param>
	/// <param name="_pow">振動の強さ(1～1000)</param>
	void PadVibration(KeyConfig::JOYPAD_NO _no, int _time, int _pow);

	/// <summary>
	/// 振動を止める
	/// </summary>
	/// <param name="_no">ゲームコントローラーの番号</param>
	void StopPadVibration(KeyConfig::JOYPAD_NO _no);

protected:

	KeyConfig(void);
	~KeyConfig(void)override = default;

private:

	std::unique_ptr<InputManager> inputManager_;	//入力管理クラスのインスタンス

	std::map<CONTROL_TYPE, std::vector<int>>keyInput_;					//操作の種類とキーの種類でキーボードの状態を格納
	std::map<CONTROL_TYPE, std::vector<JOYPAD_BTN>>conInput_;			//操作の種類とボタンの種類でコントローラーの状態を格納
	std::map<CONTROL_TYPE, std::vector<JOYPAD_STICK>>stickInput_;		//操作の種類とスティックの種類でコントローラーの状態を格納
	std::map < CONTROL_TYPE, std::vector<MOUSE>>mouseInput_;			//操作の種類とマウスの種類でマウスの状態を格納
};