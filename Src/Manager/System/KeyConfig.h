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
		KEY_PAD1,			// キー入力とパッド１入力
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

	/// @brief キーが押されているか
	/// @param cType 操作名
	/// @param no ゲームコントローラーの番号
	/// @param type 入力タイプ
	/// @return true:押されている
	bool IsNew(CONTROL_TYPE cType, KeyConfig::JOYPAD_NO no ,TYPE type = TYPE::ALL);

	/// @brief キーが押されたか(押された瞬間のみ)
	/// @param cType 操作名
	/// @param no ゲームコントローラーの番号
	/// @param type 入力タイプ
	/// @return true:押された
	bool IsTrgDown(CONTROL_TYPE cType, KeyConfig::JOYPAD_NO no,TYPE type = TYPE::ALL);

	/// @brief キーが離されたか(離された瞬間のみ)
	/// @param cType 操作名
	/// @param no ゲームコントローラーの番号
	/// @param type 入力タイプ
	/// @return true:押されている
	bool IsTrgUp(CONTROL_TYPE cType, KeyConfig::JOYPAD_NO no, TYPE type = TYPE::ALL);

	/// @brief キーが保持されているか(離された瞬間のみ)
	/// @param cType 操作名
	/// @param no ゲームコントローラーの番号
	/// @param type 入力タイプ
	/// @return true:押されている
	bool IsTrgHold(CONTROL_TYPE cType, KeyConfig::JOYPAD_NO no, float _holdTime, TYPE type = TYPE::ALL);

	/// @brief 対応キーを追加
	/// @param type キーの種類
	/// @param key 追加したい入力(キーボード)
	void Add(CONTROL_TYPE type, int key);

	/// @brief 対応キーを追加
	/// @param type キーの種類
	/// @param key 追加したい入力(ゲームコントローラーボタン)
	void Add(CONTROL_TYPE type, JOYPAD_BTN key);

	/// @brief 対応キーを追加
	/// @param type キーの種類
	/// @param key 追加したい入力(ゲームコントローラースティック)
	void Add(CONTROL_TYPE type, JOYPAD_STICK key);

	/// @brief 対応キーを追加
	/// @param type キーの種類
	/// @param key 追加したい入力(マウス)
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
	
	/// @brief ゲームコントローラーを振動する
	/// @param _no 振動させるゲームコントローラーの番号
	/// @param _time ミリ秒　,-1で無限に続けることができる(STOP 必須)
	/// @param _pow 振動の強さ(1～1000)
	void PadVibration(KeyConfig::JOYPAD_NO _no, int _time, int _pow);

	/// @brief 振動を止める
	/// @param _no ゲームコントローラーの番号
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

