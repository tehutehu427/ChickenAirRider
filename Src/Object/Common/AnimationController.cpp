#include"../pch.h"
#include "../../Manager/System/SceneManager.h"
#include "AnimationController.h"

AnimationController::AnimationController(int modelId)
{
	modelId_ = modelId;

	playName_ = "";
	isLoop_ = false;

	isStop_ = false;
	switchLoopReverse_ = 0.0f;
	endLoopSpeed_ = 0.0f;
	stepEndLoopStart_ = 0.0f;
	stepEndLoopEnd_ = 0.0f;
}

AnimationController::~AnimationController(void)
{
	for (const auto& anim : animations_)
	{
		MV1DeleteModel(anim.second.model);
	}
}

void AnimationController::Add(const std::string _name, const int _animNum, const float _speed, const int _modelId)
{
	Animation anim;
	if (_modelId != -1)
	{
		//リソースマネージャでロードしたものを使う
		anim.model = _modelId;
	}
	else
	{
		//持ち主のモデル
		anim.model = modelId_;
	}
	anim.animIndex = _animNum;
	anim.speed = _speed;

	//指定番号の配列にアニメーションが存在しない場合
	if (animations_.count(_name) == 0)
	{
		//追加
		animations_.emplace(_name, anim);
	}
	//存在する場合
	else
	{
		//上書き
		animations_[_name].model = anim.model;
		animations_[_name].animIndex = anim.animIndex;
		animations_[_name].attachNo = anim.attachNo;
		animations_[_name].totalTime = anim.totalTime;
	}
}


void AnimationController::Play(const std::string _name, const bool _isLoop,
const float _startStep, const float _endStep, const bool _isStop, const bool _isForce)
{
	//アニメーションが存在するか
	if (!animations_.contains(_name))
	{
		std::cout << "アニメーション" + _name + "が存在しません" << std::endl;
		return;
	}

	if (playName_ != _name || _isForce) {

		//再生中のアニメーションがあるなら
		if (playName_ != "")
		{
			// モデルからアニメーションを外す
			playAnim_.attachNo = MV1DetachAnim(modelId_, playAnim_.attachNo);
		}

		// アニメーション種別を変更
		playName_ = _name;
		playAnim_ = animations_[_name];

		// 初期化
		playAnim_.step = _startStep;

		// モデルにアニメーションを付ける
		int animIdx = animations_[_name].animIndex;

		//アニメーションを割り当てる
		playAnim_.attachNo = MV1AttachAnim(modelId_, animIdx, playAnim_.model);

		// アニメーション総時間の取得
		if (_endStep > 0.0f)
		{
			playAnim_.totalTime = _endStep;
		}
		else
		{
			playAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);
		}

		// アニメーションループ
		isLoop_ = _isLoop;

		// アニメーションしない
		isStop_ = _isStop;

		stepEndLoopStart_ = -1.0f;
		stepEndLoopEnd_ = -1.0f;
		switchLoopReverse_ = 1.0f;
	}

}

void AnimationController::Update(void)
{

	// 経過時間の取得
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	if (!isStop_)
	{
		// 再生
		playAnim_.step += (deltaTime * playAnim_.speed * switchLoopReverse_);

		// アニメーション終了判定
		bool isEnd = false;
		if (switchLoopReverse_ > 0.0f)
		{
			// 通常再生の場合
			if (playAnim_.step > playAnim_.totalTime)
			{
				isEnd = true;
			}
		}
		else
		{
			// 逆再生の場合
			if (playAnim_.step < playAnim_.totalTime)
			{
				isEnd = true;
			}
		}

		if (isEnd)
		{
			// アニメーションが終了したら
			if (isLoop_)
			{
				// ループ再生
				if (stepEndLoopStart_ > 0.0f)
				{
					// アニメーション終了後の指定フレーム再生
					switchLoopReverse_ *= -1.0f;
					if (switchLoopReverse_ > 0.0f)
					{
						playAnim_.step = stepEndLoopStart_;
						playAnim_.totalTime = stepEndLoopEnd_;
					}
					else
					{
						playAnim_.step = stepEndLoopEnd_;
						playAnim_.totalTime = stepEndLoopStart_;
					}
					playAnim_.speed = endLoopSpeed_;
					
				}
				else
				{
					// 通常のループ再生
					playAnim_.step = 0.0f;
				}
			}
			else
			{
				// ループしない
				playAnim_.step = playAnim_.totalTime;
			}

		}

	}

	// アニメーション設定
	MV1SetAttachAnimTime(modelId_, playAnim_.attachNo, playAnim_.step);

}

void AnimationController::SetEndLoop(float startStep, float endStep, float speed)
{
	stepEndLoopStart_ = startStep;
	stepEndLoopEnd_ = endStep;
	endLoopSpeed_ = speed;
}

const std::string AnimationController::GetPlayName(void) const
{
	return playName_;
}

const float AnimationController::GetAnimStep(void) const
{
	return playAnim_.step;
}

void AnimationController::SetAnimSpeed(const float _spd)
{
	playAnim_.speed = _spd;
}

bool AnimationController::IsEnd(void) const
{

	bool ret = false;

	if (isLoop_)
	{
		// ループ設定されているなら、
		// 無条件で終了しないを返す
		return ret;
	}

	if (playAnim_.step >= playAnim_.totalTime)
	{
		// 再生時間を過ぎたらtrue
		return true;
	}

	return ret;

}
