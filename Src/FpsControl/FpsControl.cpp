#include"../pch.h"
#include "FpsControl.h"
#include "../Application.h"

FpsControl::FpsControl(void)
	: currentTime_(0)
	, prevFrameTime_(0)
	, frameCnt_(0)
	, updateFrameRateTime_(0)
	, frameRate_(0.0f)
{
}

FpsControl::~FpsControl(void)
{
}

void FpsControl::Init(void)
{
	currentTime_ = 0;
	prevFrameTime_ = 0;
	frameCnt_ = 0;
	updateFrameRateTime_ = 0;
	frameRate_ = 0.0f;
}

bool FpsControl::UpdateFrameRate(void)
{
	Sleep(1);	//システムに処理を返す

	//現在の時刻を取得
	currentTime_ = GetNowCount();

	//現在の時刻が、前回のフレーム実行時より
	//1/60秒経過していたら処理を実行する
	if (currentTime_ - prevFrameTime_ >= FRAME_RATE)
	{
		//フレーム実行時の時間を計算
		prevFrameTime_ = currentTime_;

		//フレーム数をカウント
		frameCnt_++;

		//1/60経過した
		return true;
	}

	return false;
}

void FpsControl::CalcFrameRate(void)
{
	static const int FRAME_RATE_UPDATE_TIME = 1000;	//フレームレート更新時間(ミリ秒)

	//前回のフレームレート更新からの経過時間を求める
	int difTime = currentTime_ - updateFrameRateTime_;

	//前回のフレームレートを更新から
	//1秒以上経過していたらフレームレートを更新する
	if (difTime > FRAME_RATE_UPDATE_TIME)
	{
		//フレーム回数をミリ秒に合わせる
		//少数まで出したのでfloatにキャスト
		float castFrameCnt = (float)(frameCnt_ * FRAME_RATE_UPDATE_TIME);

		//フレームレートを求める
		//理想通りなら 60000 / 1000 で60となる
		frameRate_ = castFrameCnt / difTime;

		//フレームレート更新時間を更新
		updateFrameRateTime_ = currentTime_;
	}
}

void FpsControl::DrawFrameRate(void)const
{
	//デバッグ用表示

	//フレームレート表示位置
	static const int FRAME_RATE_DRAW_POS_X = Application::SCREEN_SIZE_X - 90;	
	static const int FRAME_RATE_DRAW_POS_Y = 0;
	static const unsigned int FRAME_RATE_DRAW_COLOR = GetColor(255, 0, 0);

	//スクリーンの右端に出るように表示
	DrawFormatString(
		FRAME_RATE_DRAW_POS_X,
		FRAME_RATE_DRAW_POS_Y,
		FRAME_RATE_DRAW_COLOR,
		L"FPS[%.2f]",
		frameRate_);
}
