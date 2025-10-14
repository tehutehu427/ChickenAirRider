#pragma once

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
	const bool IsTimeOver(void) { return timer_ < cnt_; }
			
private:

	//時間計測
	float timer_;

	//カウンタ
	float cnt_;

	//カウントの状態
	bool cntValid_;
};

