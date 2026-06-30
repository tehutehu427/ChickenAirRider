#pragma once
#include"../Common/Vector2.h"

class Timer
{
public:

	/// @brief コンストラクタ
	/// @param _timeLimit 時間制限(0.0だと無制限)
	Timer(void);
	
	//デストラクタ
	~Timer(void);

	//初期化
	void Init(const float _timeLimit = 0.0f);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//時間制限の有無
	const bool IsTimeLimit(void)const { return timeLimit_ > 0.0f; }

	//時間制限の設定
	void SetTimeLimit(const float _timeLimit) { timeLimit_ = _timeLimit; }

	//カウントの有無の設定
	void SetCountValid(const bool _cntValid) { cntValid_ = _cntValid; }

	//カウントの表示の有無
	void SetCountView(const bool _isView) { isView_ = _isView; }

	//時間制限が終わったか
	const bool IsTimeOver(void)const;

	//残り時間の取得
	const float GetRemainingTime(void)const;

	/// @brief 指定秒数以下になったか
	/// @param _time 指定秒
	/// @return 指定秒数以下だった
	const bool IsUnderSeconds(const float _time) { return GetRemainingTime() < _time; }

	//位置の設定
	void SetPos(const Vector2 _pos) { pos_ = _pos; }

	//カウンタの数字が変わったか
	const bool IsChanged(void)const { return isChanged_; }
			
private:

	//秒数の分換算
	static constexpr int SECOND_TO_MINUTE = 60;

	//画像幅
	static constexpr int IMAGE_WIDTH = 20;
	static constexpr float IMAGE_EXRATE = 0.6f;
	static constexpr float FRAME_EXRATE = 0.4f;

	//時間計測
	float timeLimit_;

	//カウンタ
	float cnt_;

	//カウンタの前フレーム
	float preCnt_;

	//カウンタの数字が変わったか
	bool isChanged_;

	//カウントの状態
	bool cntValid_;

	//カウントの表示
	bool isView_;

	//表示場所
	Vector2 pos_;

	//画像
	int frameImg_;
	int colonImg_;
	int* numImgs_;

	//分変換
	const int Minute(const float _value)const;

	//秒変換
	const int Second(const float _value)const;
};

