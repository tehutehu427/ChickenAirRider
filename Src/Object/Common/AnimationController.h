#pragma once
#include <string>
#include <map>
class SceneManager;

class AnimationController
{
	
public :

	// アニメーションデータ
	struct Animation
	{
		int model = -1;
		int attachNo = -1;
		int animIndex = 0;
		float speed = 0.0f;
		float totalTime = 0.0f;
		float step = 0.0f;
	};


	// コンストラクタ
	AnimationController(int modelId);
	// デストラクタ
	~AnimationController(void);

	/// @brief 追加
	/// @param type アニメーション番号
	/// @param speed 速度
	/// @param modelId モデルID
	void Add(int type, const float speed,int modelId = -1);

	/// @brief アニメーションプレイ
	/// @param type アニメーション番号
	/// @param isLoop ループするか
	/// @param startStep 再生開始フレーム
	/// @param endStep 再生終了フレーム
	/// @param isStop アニメーションを止める
	/// @param isForce 同じアニメーションを再生したい場合はtrue
	void Play(int type, bool isLoop = true, 
		float startStep = 0.0f, float endStep = -1.0f, bool isStop = false, bool isForce = false);

	//更新
	void Update(void);

	// アニメーション終了後に繰り返すループステップ
	void SetEndLoop(float startStep, float endStep, float speed);

	// 再生中のアニメーション
	int GetPlayType(void) const;

	//アニメーションステップゲッタ
	const float GetAnimStep(void)const;

	//アニメーションスピードセッタ
	void SetAnimSpeed(const float _spd);

	// 再生終了
	bool IsEnd(void) const;

private :

	// モデルのハンドルID
	int modelId_;

	// 種類別のアニメーションデータ
	std::map<int, Animation> animations_;

	int playType_;
	Animation playAnim_;

	// アニメーションをループするかしないか
	bool isLoop_;

	// アニメーションを止めたままにする
	bool isStop_;

	// アニメーション終了後に繰り返すループステップ
	float stepEndLoopStart_;
	float stepEndLoopEnd_;
	float endLoopSpeed_;

	// 逆再生
	float switchLoopReverse_;

};

