#include"../pch.h"
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SceneManager.h"
#include "Fader.h"

Fader::Fader(void)
{
	state_ = STATE::NONE;
	isPreEnd_ = false;
	isEnd_ = false;
	imgMask_ = -1;
	tmpScreen_ = -1;

	//処理の登録
	stateUpdateMap_[static_cast<int>(STATE::FADE_IN)] = &Fader::UpdateFadeIn;
	stateUpdateMap_[static_cast<int>(STATE::FADE_OUT)] = &Fader::UpdateFadeOut;
	stateUpdateMap_[static_cast<int>(STATE::NONE)] = &Fader::UpdateNone;
}

Fader::~Fader(void)
{
	//リソースの破棄
	DeleteGraph(imgMask_);
	DeleteGraph(tmpScreen_);
}

void Fader::Init(void)
{
	//初期化
	state_ = STATE::NONE;
	isPreEnd_ = true;
	isEnd_ = true;
	rate_ = 0.0f;
	time_ = 0.0f;

	//リソースを読み込み
	imgMask_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::FADE).handleId_;

	//描画領域を作成
	tmpScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		true
	);
}

void Fader::Update(void)
{
	//終了してるときは何もしない
	if (isEnd_)
	{
		return;
	}

	//状態ごとの更新処理
	(this->*stateUpdateMap_[static_cast<int>(state_)])();
}

void Fader::Draw(void)
{
	//状態がないときは何もしない
	if (state_ == STATE::NONE)
	{
		return;
	}

	//画像マスク処理
	SpriteMask();
}

void Fader::SetFade(const STATE _state)
{
	//状態を設定
	state_ = _state;
	if (state_ != STATE::NONE)
	{
		//フェード処理を開始するので、終了判定をリセット
		isPreEnd_ = false;
		isEnd_ = false;
	}
}

void Fader::UpdateFadeIn(void)
{
	//カウンタ
	time_ += SceneManager::GetInstance().GetDeltaTime();

	//拡大率を時間で変える
	rate_ = Utility::EaseInQuad(
		time_,
		TOTAL_TIME,
		0.0f,        // 開始値
		RATE_MAX     // 終了値
	);

	//終了判定
	if (time_ >= TOTAL_TIME || rate_ >= RATE_MAX)
	{
		//最大値を超えないように
		rate_ = RATE_MAX;

		//完全終了
		if (isPreEnd_)
		{
			isEnd_ = true;
			time_ = 0.0f;
		}

		//次のフレームで終了判定を行う
		isPreEnd_ = true;
	}
}

void Fader::UpdateFadeOut(void)
{
	//カウンタ
	time_ += SceneManager::GetInstance().GetDeltaTime();

	//拡大率を時間で変える
	rate_ = Utility::EaseOutQuad(
		time_,          // 経過時間
		TOTAL_TIME,     // 総時間
		RATE_MAX,       // 開始値
		0.0f            // 終了値
	);

	//終了判定
	if (time_ >= TOTAL_TIME || rate_ <= 0.0f)
	{
		//最小値を超えないように
		rate_ = 0.0f;

		//完全終了
		if (isPreEnd_)
		{
			isEnd_ = true;
			time_ = 0.0f;
		}

		//次のフレームで終了判定を行う
		isPreEnd_ = true;
	}
}

void Fader::UpdateNone(void)
{
	//何もしない
}

void Fader::SpriteMask(void)const
{
	// 描画領域をマスク画像領域に切り替える
	// 元々は、背面スクリーンになっている
	SetDrawScreen(tmpScreen_);

	//画面全体を黒に塗る
	DrawBox(
		0, 0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		Utility::BLACK,
		true);

	//白色の画像を描画
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y,
		rate_,
		0.0f,
		imgMask_,
		true);

	//描画領域を元に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	//描画を色の乗算モードにする
	SetDrawBlendMode(DX_BLENDMODE_MUL, 0);

	//元々のゲーム画面にマスク画像を描画する
	DrawGraph(0, 0, tmpScreen_, false);

	//描画モードを元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}