#include"../pch.h"
#include"../Application.h"
#include"../Utility/Utility.h"
#include "../Loader/LoaderManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Object/Stage/StageObject.h"
#include "StageManager.h"

//静的インスタンスの初期化
StageManager* StageManager::instance_ = nullptr;

void StageManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new StageManager();
	}
}

void StageManager::Load(void)
{
	//ステージ情報
	importData_.emplace(MODE::MAIN, LoaderManager<StageImportData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_OUTSIDE + L"MainStage.json")));
	importData_.emplace(MODE::BATTLE, LoaderManager<StageImportData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_OUTSIDE + L"BattleStage.json")));
	importData_.emplace(MODE::AIR_GLIDER, LoaderManager<StageImportData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_OUTSIDE + L"AirGliderStage.json")));
	gliderStartStageData_ = (LoaderManager<StageImportData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_OUTSIDE + L"AirGliderStartStage.json")));
}

void StageManager::Init(const MODE _mode, int _createNum)
{
	//初期化
	stages_.clear();

	//モード設定
	mode_ = _mode;

	//エアグライダー系のみスタート台を作る
	if (mode_ == MODE::AIR_GLIDER)
	{
		//ステージの作成
		for (const auto& data : gliderStartStageData_)
		{
			//モデルの作成
			int modelId = modelId_[data.name]();

			//ステージの生成
			stages_.emplace_back(std::make_unique<StageObject>(data, modelId, tag_[data.tag], data.position));
		}
	}

	//生成数分作成
	for (int i = 0; i < _createNum; i++)
	{
		//ステージの作成
		for (const auto& data : importData_[mode_])
		{
			//モデルの作成
			int modelId = modelId_[data.name]();

			//座標
			VECTOR pos = data.position;
			pos.z *= (i + 1);

			//ステージの生成
			stages_.emplace_back(std::make_unique<StageObject>(data, modelId, tag_[data.tag], pos));
		}
	}

	//初期化
	for (const auto& stage : stages_)
	{
		stage->Init();
	}
}

void StageManager::Update(void)
{
	//モードごとの更新
	update_[mode_]();
}

void StageManager::Draw(const Camera& _camera)
{
	//ステージの描画
	for (const auto& stage : stages_)
	{
		//カメラ制限
		if (Utility::MagnitudeF(VSub(stage->GetTrans().pos, _camera.GetPos())) > Camera::VIEW_RANGE)continue;

		stage->Draw();
	}
}

void StageManager::Destroy(void)
{
	//ステージを明示的に解放
	stages_.clear();
}

StageManager::StageManager(void)
{
	//リソース
	modelId_["Stage"] = [this](void)
	{
		//インスタンス
		auto& res = ResourceManager::GetInstance();
		return res.LoadModelDuplicate(ResourceManager::SRC::MAIN_STAGE);
	};
	modelId_["Glass"] = [this](void)
	{
		//インスタンス
		auto& res = ResourceManager::GetInstance();
		return res.LoadModelDuplicate(ResourceManager::SRC::GLASS);
	};
	modelId_["Building"] = [this](void)
	{
		//インスタンス
		auto& res = ResourceManager::GetInstance();
		return res.LoadModelDuplicate(ResourceManager::SRC::BUILDING);
	};
	modelId_["Tree"] = [this](void)
	{
		//インスタンス
		auto& res = ResourceManager::GetInstance();
		return res.LoadModelDuplicate(ResourceManager::SRC::TREE);
	};
	modelId_["GlideStage"] = [this](void)
	{
		//インスタンス
		auto& res = ResourceManager::GetInstance();
		return res.LoadModelDuplicate(ResourceManager::SRC::GLIDE_STAGE);
	};
	modelId_["GlideStartStage"] = [this](void)
	{
		//インスタンス
		auto& res = ResourceManager::GetInstance();
		return res.LoadModelDuplicate(ResourceManager::SRC::GLIDE_START_STAGE);
	};
	modelId_["WorldBorder"] = [this](void)
	{
		//ワールドボーダーなのでモデルなし
		return -1;
	};

	//タグ
	tag_["normalObject"] = Collider::TAG::NORMAL_OBJECT;
	tag_["building"] = Collider::TAG::NORMAL_OBJECT;
	tag_["worldBorder"] = Collider::TAG::WORLD_BORDER;
	tag_["gliderBorder"] = Collider::TAG::GLIDER_BORDER;
	tag_["glideStage"] = Collider::TAG::GLIDE_STAGE;
	tag_["tree"] = Collider::TAG::TREE;
	tag_["glass"] = Collider::TAG::GROUND;
	tag_["ground"] = Collider::TAG::GROUND;

	update_[MODE::MAIN] = [this](void){UpdateNormal(); };
	update_[MODE::BATTLE] = [this](void){UpdateNormal(); };
	update_[MODE::AIR_GLIDER] = [this](void){UpdateLoop(); };

	mode_ = MODE::MAIN;
}

StageManager::~StageManager(void)
{
}

void StageManager::UpdateNormal(void)
{
	//ステージの更新
	for (const auto& stage : stages_)
	{
		stage->Update();
	}
}

void StageManager::UpdateLoop(void)
{
	//通常更新
	UpdateNormal();
}
