#pragma once
#include"../Common/Vector2.h"

class Timer
{
public:

	/// @brief コンストラクタ
	/// @param _timeLimit 時間制限(0.0だと無制限)
	Timer(const float _timeLimit = 0.0f);
	
	//デストラクタ
	~Timer(void);

	//初期化
	void Init(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//時間制限の有無
	const bool IsTimeLimit(void)const { return timer_ > 0.0f; }

	//カウントの有無の設定
	void SetCountValid(const bool _cntValid) { cntValid_ = _cntValid; }

	//時間制限が終わったか
	const bool IsTimeOver(void);

	//残り時間の取得
	const float RemainingTime(void);

	//位置の設定
	void SetPos(const Vector2 _pos) { pos_ = _pos; }
			
private:

	//秒数の分換算
	static constexpr float SECOND_TO_MINUTE = 60.0f;

	//時間計測
	float timer_;

	//カウンタ
	float cnt_;

	//カウントの状態
	bool cntValid_;

	//表示場所
	Vector2 pos_;

	//画像
	int colonImg_;
	int* numImgs_;

	//分変換
	const int Minute(const float _value)const;

	//秒変換
	const int Second(const float _value)const;
};

