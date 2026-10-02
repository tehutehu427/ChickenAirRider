#pragma once
#include "../Common/Singleton.h"
#include "../Object/Player/AnimationImportData.h"

class AnimationManager : public Singleton<AnimationManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<AnimationManager>;

public:

	//読み込み
	void Init(void)override;

	//解放
	void Destroy(void);

	/// <summary>
	/// アニメーションの番号を取得
	/// </summary>
	/// <param name="_name">キャラクター名</param>
	/// <returns>キャラクターのアニメーション番号</returns>
	const std::unordered_map<std::string, AnimationImportData::AnimationData>& GetAnimationData(const std::string _name);

private:

	//アニメーション名に
	std::unordered_map<std::string, std::unordered_map<std::string, AnimationImportData::AnimationData>> anim_;

	//コンストラクタ
	AnimationManager(void);

	//デストラクタ
	~AnimationManager(void)override;
};

