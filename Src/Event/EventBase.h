#pragma once

class EventBase
{
public:

	//コンストラクタ
	EventBase(void);

	//デストラクタ
	virtual~EventBase(void);

	//開始
	virtual void Start(void) = 0;
	
	//更新
	virtual void Update(void) = 0;
	
	//終了
	virtual void End(void) = 0;

	//終了判定
	const bool IsEnd(void)const { return isEnd_; }

protected:

	//制限時間
	static constexpr float LIMIT_TIME = 30.0f;

	//カウンタ
	float cnt_;

	//終了判定
	bool isEnd_;
};