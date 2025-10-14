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

private:

	//時間計測
	float timer_;

	//カウンタ
	float cnt_;
};

