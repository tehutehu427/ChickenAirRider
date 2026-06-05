#include "../../pch.h"
#include "../../Application.h"
#include "../../Utility/Utility.h"
#include "../../Common/Vector2.h"
#include "../System/SceneManager.h"
#include "../System/ResourceManager.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "HUDManager.h"

void HUDManager::LoadOutSide(void)
{
	//インスタンス
	ResourceManager& res = ResourceManager::GetInstance();
	const SceneManager& scnMng = SceneManager::GetInstance();

	//画像
	gaugeImg_ = res.Load(ResourceManager::SRC::CHARGE_GAUGE).handleId_;
	gaugeMaskImg_ = res.Load(ResourceManager::SRC::CHARGE_GAUGE_MASK).handleId_;
	numImgs_ = res.Load(ResourceManager::SRC::NUMBER).handleIds_;

	//パワーアップ画像
	maxSpeedImg_ = res.Load(ResourceManager::SRC::MAX_SPEED).handleId_;
	acceleImg_ = res.Load(ResourceManager::SRC::ACCELERATION).handleId_;
	turnImg_ = res.Load(ResourceManager::SRC::TURNING).handleId_;
	chargeImg_ = res.Load(ResourceManager::SRC::CHARGE).handleId_;
	flightImg_ = res.Load(ResourceManager::SRC::FLIGHT).handleId_;
	weightImg_ = res.Load(ResourceManager::SRC::WEIGHT).handleId_;
	attackImg_ = res.Load(ResourceManager::SRC::ATTACK).handleId_;
	defenceImg_ = res.Load(ResourceManager::SRC::DEFENCE).handleId_;
	maxHealthImg_ = res.Load(ResourceManager::SRC::MAX_HEALTH).handleId_;

	//マテリアル
	material_ = std::make_unique<PixelMaterial>(L"GaugeMask.cso", 1);
	material_->AddConstBuf({ 0.5f,0.5f,0.0f,gaugeCnt_ });
	material_->AddTextureBuf(gaugeMaskImg_);
	renderer_ = std::make_unique<PixelRenderer>(*material_);
}

void HUDManager::Init(void)
{
}

void HUDManager::Update(void)
{
	//デルタタイム
	const auto& delta = SceneManager::GetInstance().GetDeltaTime();

	//カウンタ
	gaugeCnt_ += delta;
}

void HUDManager::Draw(const int _playerIndex)
{
}
