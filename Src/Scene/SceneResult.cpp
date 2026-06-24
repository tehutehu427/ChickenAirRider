#include "../pch.h"
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/SplitScreenManager.h"
#include "../Manager/System/Camera.h"
#include "../Manager/System/SoundManager.h"
#include "../Manager/Game/AnimationManager.h"
#include "../Manager/Game/PlayerManager.h"
#include "../Manager/Game/GameSetting.h"
#include "../Object/Common/AnimationController.h"
#include "SceneResult.h"

SceneResult::SceneResult(void)
{
}

SceneResult::~SceneResult(void)
{
	SoundManager::GetInstance().StopAll();
}

void SceneResult::Load(void)
{
	//インスタンス
	auto& res = ResourceManager::GetInstance();

	//順位画像
	rankImg_.emplace(1, res.Load(ResourceManager::SRC::RANK_ONE).handleId_);
	rankImg_.emplace(2, res.Load(ResourceManager::SRC::RANK_TWO).handleId_);
	rankImg_.emplace(3, res.Load(ResourceManager::SRC::RANK_THREE).handleId_);
	rankImg_.emplace(4, res.Load(ResourceManager::SRC::RANK_FOUR).handleId_);

	//背景
	backImg_ = res.Load(ResourceManager::SRC::RESULT_BACK).handleId_;
	backCurtainImg_ = res.Load(ResourceManager::SRC::CURTAIN).handleId_;
}

void SceneResult::Init(void)
{
	//インスタンス
	auto& scnMng = SceneManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();
	auto& res = ResourceManager::GetInstance();
	auto& split = SplitScreenManager::GetInstance();
	auto& snd = SoundManager::GetInstance();
	const auto& mainCamera = scnMng.GetCamera(0).lock();

	//プレイヤー人数
	int plNum = GameSetting::GetInstance().GetPlayerNum();

	//アニメーション番号
	const auto& animNums = AnimationManager::GetInstance().GetAnimationData("chicken");

	//リザルトは画面一つ
	split.CreateSplitViews(1, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);

	//モデルの生成
	for (int i = 0; i < plNum; i++)
	{
		//モデル情報
		Transform trans;
		trans.pos = { PLAYER_POS.x + (PLAYER_LOCAL_POS * i),PLAYER_POS.y,PLAYER_POS.z };
		trans.scl = Utility::VECTOR_ONE;
		trans.quaRot = Quaternion();
		trans.modelId = res.LoadModelDuplicate(ResourceManager::SRC::CHICKEN);
		trans.Update();

		//アニメーション
		std::unique_ptr<AnimationController>anim;
		anim = std::make_unique<AnimationController>(trans.modelId);

		//モデル情報の保存
		trans_.push_back(trans);

		//アニメーション割り当て
		for (const auto& animNum : animNums)
		{
			anim->Add(animNum.first, animNum.second, 60.0f);
		}

		//立ち状態
		anim->Play("idle");

		//アニメーションの保存
		anim_.push_back(std::move(anim));
	}

	//カメラの固定化
	mainCamera->ChangeMode(Camera::MODE::FIXED_POINT);
	mainCamera->SetPos(CAMERA_POS);

	//BGM読み込み
	int id = res.Load(ResourceManager::SRC::RESULT_BGM).handleId_;
	snd.Add(SoundManager::SOUND_NAME::RESULT_BGM, id, SoundManager::TYPE::BGM);

	id = res.Load(ResourceManager::SRC::ENTER_SE).handleId_;
	snd.Add(SoundManager::SOUND_NAME::ENTER, id, SoundManager::TYPE::SE);

	//BGM再生
	snd.Play(SoundManager::SOUND_NAME::RESULT_BGM, SoundManager::PLAYTYPE::LOOP);
}

void SceneResult::Update(void)
{
	//インスタンス
	auto& key = KeyConfig::GetInstance();
	auto& snd = SoundManager::GetInstance();

	//決定
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		//タイトルへ
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true, true);

		//決定音
		snd.Play(SoundManager::SOUND_NAME::ENTER, SoundManager::PLAYTYPE::BACK);
	}

	//モデル更新
	for (auto& trans : trans_)
	{
		trans.Update();
	}

	//アニメーション更新
	for (auto& anim : anim_)
	{
		anim->Update();
	}
}

void SceneResult::Draw(const Camera& _camera)
{
	//プレイヤー人数
	auto& plMng = PlayerManager::GetInstance();
	auto ranks = plMng.GetRanks();

	//背景
	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, backImg_,true);
	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, backCurtainImg_,true);

	for (int i = 0 ; i < trans_.size() ; i++)
	{
		//モデル描画
		MV1DrawModel(trans_[i].modelId);
	
		//順位
		DrawRotaGraph(RANK_POS.x + RANK_LOCAL_POS * i, RANK_POS.y, 0.15, 0.0, rankImg_[ranks[i]], true);
	}
}

void SceneResult::Release(void)
{
}
