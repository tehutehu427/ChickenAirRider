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
	/// @param _name アニメーション名
	/// @param _type アニメーション番号
	/// @param _speed 速度
	/// @param _modelId モデルID
	void Add(const std::string _name, const int _animNum, const float _speed, const int _modelId = -1);

	/// @brief アニメーションプレイ
	/// @param _name アニメーション名
	/// @param isLoop ループするか
	/// @param startStep 再生開始フレーム
	/// @param endStep 再生終了フレーム
	/// @param isStop アニメーションを止める
	/// @param isForce 同じアニメーションを再生したい場合はtrue
	void Play(const std::string _name, const bool _isLoop = true, 
		const float _startStep = 0.0f, const float _endStep = -1.0f, const bool _isStop = false, const bool _isForce = false);

	//更新
	void Update(void);

	// アニメーション終了後に繰り返すループステップ
	void SetEndLoop(float startStep, float endStep, float speed);

	// 再生中のアニメーション
	const std::string GetPlayName(void) const;

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
	std::map<std::string, Animation> animations_;
	
	//再生中アニメーション名
	std::string playName_;

	//アニメーション情報
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

