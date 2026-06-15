#include "../../pch.h"
#include "../../Application.h"
#include "../../Utility/Utility.h"
#include "../../Common/Vector2.h"
#include "../System/SceneManager.h"
#include "../System/ResourceManager.h"
#include "../System/SplitScreenManager.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "HUDManager.h"

void HUDManager::LoadOutSide(void)
{
	//インスタンス
	ResourceManager& res = ResourceManager::GetInstance();
	const SceneManager& scnMng = SceneManager::GetInstance();

	//描画
	for (auto& playerHud : playerHUD_)
	{
		playerHud.hudData[static_cast<size_t>(HUD_TYPE::CHARGE_GAUGE)] = { false,&HUDManager::DrawChargeGauge };
		playerHud.hudData[static_cast<size_t>(HUD_TYPE::HEALTH)] = { false,&HUDManager::DrawHealth};
		playerHud.hudData[static_cast<size_t>(HUD_TYPE::PARAMETER)] = { false,&HUDManager::DrawParam};
	}

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

void HUDManager::Draw(void)
{
	//分割スクリーン
	auto& split = SplitScreenManager::GetInstance();
	int activeViewCnt = split.GetActiveViewCount();

	for (int i = 0; i < activeViewCnt; i++)
	{
		DrawPlayerHUD(i);
	}
}

void HUDManager::SetPlayer(const Player* _player, const int _index)
{
	//ゲーム最大人数より大きかったら無視
	if (_index > GameSetting::PLAYER_MAX_NUM)return;

	//プレイヤー設定
	playerHUD_[_index].player = _player;
}

void HUDManager::SetVisible(const int _index, const HUD_TYPE _type, const bool _visible)
{
	playerHUD_[_index].hudData[static_cast<size_t>(_type)].visible = _visible;
}

HUDManager::HUDManager(void)
{
}

HUDManager::~HUDManager(void)
{
}

void HUDManager::DrawPlayerHUD(const int _playerIndex)
{
	//プレイヤー参照
	const Player* player = playerHUD_[_playerIndex].player;

	//存在しないなら何もしない
	if (!player)return;

	//プレイヤーのHUD情報
	auto& huds = playerHUD_[_playerIndex].hudData;

	for (const auto& hud : huds)
	{
		//非表示判定
		if (!hud.visible)continue;

		//描画
		(this->*hud.func)(_playerIndex);
	}
}

void HUDManager::DrawChargeGauge(const int _playerIndex)
{
}

void HUDManager::DrawHealth(const int _playerIndex)
{
}

void HUDManager::DrawParam(const int _playerIndex)
{
}
