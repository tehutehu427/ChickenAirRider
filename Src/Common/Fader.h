#pragma once
#include <array>

class Fader
{

public:

	// 状態
	enum class STATE
	{
		NONE, 
		FADE_OUT,	// 徐々に暗転
		FADE_IN,	// 徐々に明転
		MAX
	};

	//コンストラクタ
	Fader(void);

	//デストラクタ
	~Fader(void);

	//初期化
	void Init(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);
	
	//指定フェードを開始する
	void SetFade(const STATE _state);
	
	//フェード状態の取得
	const STATE GetState(void) const { return state_; }

	//終了判定を返す
	const bool IsEnd(void) const { return isEnd_; }

private:

	//フェード時間
	static constexpr float TOTAL_TIME = 1.0f;

	//最大拡大率
	static constexpr float RATE_MAX = 3.0f;

	// 状態
	STATE state_;

	//時間計測用
	float time_;

	// 状態(STATE)を保ったまま終了判定を行うため、
	// Update->Draw->Updateの1フレーム判定用
	bool isPreEnd_;

	// フェード処理の終了判定
	bool isEnd_;

	//マスク画像領域
	int tmpScreen_;

	//マスク用画像
	int imgMask_;

	//画像の拡大率
	float rate_;
	
	//関数ポインタ
	using Func = void(Fader::*)(void);

	//状態別更新処理の管理
	std::array<Func, static_cast<int>(STATE::MAX)> stateUpdateMap_;

	//状態別更新処理
	void UpdateFadeIn(void);
	void UpdateFadeOut(void);
	void UpdateNone(void);

	//円が他のくりぬき処理
	void SpriteMask(void)const;
};
