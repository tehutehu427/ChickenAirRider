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

	//カウントの有効の有無
	void SetCountValid(const bool _cntValid) { cntStop_ = _cntValid; }
			
private:

	//時間計測
	float timer_;

	//カウンタ
	float cnt_;

	//カウントを止めるか
	bool cntStop_;
};

