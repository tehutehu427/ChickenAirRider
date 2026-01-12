#include "../pch.h"
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Common/Vector2.h"
#include "../System/SceneManager.h"
#include "../System/ResourceManager.h"
#include "PlayerManager.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "../Object/Player/Player.h"
#include "../Object/Player/Action/MachineAction.h"
#include "UIManager.h"

void UIManager::Init(void)
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

void UIManager::Update(void)
{
	//デルタタイム
	const auto& delta = SceneManager::GetInstance().GetDeltaTime();

	//カウンタ
	gaugeCnt_ += delta;
}

void UIManager::Draw(const int _playerIndex)
{
	//描画
	for (const auto draw : drawView_)
	{
		//中身がないならスキップ
		if (draw.second == nullptr)continue;

		draw.second(_playerIndex);
	}
}

void UIManager::AddDraw(const DRAW_TYPE _type)
{
	//描画するUIに追加
	drawView_.emplace(_type, drawList_[_type]);
}

void UIManager::SubDraw(const DRAW_TYPE _type)
{
	//描画するUIから削除
	drawView_.erase(_type);
}

void UIManager::CreateViewports(const int _playerCnt, const int _screenW, const int _screenH)
{
	//中身があるなら
	if (!viewPort_.empty())
	{
		//初期化
		viewPort_.clear();
	}

	//プレイヤーの人数
	if (_playerCnt == 1)
	{
		//全画面描画
		viewPort_.push_back({ 0,0,_screenW,_screenH });
	}
	else if (_playerCnt == 2)
	{
		//上下分割
		int h = _screenH / 2;
		viewPort_.push_back({ 0,0,_screenW,h });
		viewPort_.push_back({ 0,0,_screenW,h });
	}
	else if (_playerCnt >= 3)
	{
		//4分割
		int w = _screenW / 2;
		int h = _screenH / 2;
		viewPort_.push_back({ 0,0,w,h });
		viewPort_.push_back({ 0,0,w,h });
		viewPort_.push_back({ 0,0,w,h });
		viewPort_.push_back({ 0,0,w,h });
	}
}

const UIManager::Viewport& UIManager::GetViewport(const int _playerIndex)
{
	return viewPort_[_playerIndex];
}

UIManager::UIManager(void)
{
	gaugeImg_ = -1;
	gaugeMaskImg_ = -1;
	gaugeCnt_ = 0.0f;
	numImgs_ = nullptr;

	//描画リスト
	drawList_.emplace(DRAW_TYPE::CHARGE_GAUGE, [this](const int _playerIndex) {DrawChargeGauge(_playerIndex); });
	drawList_.emplace(DRAW_TYPE::HEALTH, [this](const int _playerIndex) {DrawHealth(_playerIndex); });
	drawList_.emplace(DRAW_TYPE::CHECK_PARAM, [this](const int _playerIndex) {DrawParam(_playerIndex); });
}

UIManager::~UIManager(void)
{
	numImgs_ = nullptr;
}

void UIManager::DrawChargeGauge(const int _playerIndex)
{
	//プレイヤー
	const auto& player = PlayerManager::GetInstance().GetPlayer(_playerIndex);

	//アクションがないならスキップ
	if (player.GetAction().lock() == nullptr)return;

	//プレイヤーのアクション
	const MachineAction& mAction = dynamic_cast<const MachineAction&>(*player.GetAction().lock());

	//チャージカウンタ
	const float chargeCnt = mAction.GetChargeCnt();

	//速度
	const float speed = mAction.GetSpeed();

	//座標
	int posX = viewPort_[_playerIndex].x;
	int posY = viewPort_[_playerIndex].y;
	Vector2 chargeGaugePos = {};
	chargeGaugePos.x = posX + static_cast<int>(CHARGE_POS.x * viewPort_[_playerIndex].w) - GAUGE_LOCAL_POS;
	chargeGaugePos.y = posY + static_cast<int>(CHARGE_POS.y * viewPort_[_playerIndex].h) - GAUGE_LOCAL_POS;
	Vector2 gaugeSize = Vector2(GAUGE_SIZE / (Application::SCREEN_SIZE_Y / viewPort_[_playerIndex].h) * GAUGE_SIZE_MULTI
		, GAUGE_SIZE / (Application::SCREEN_SIZE_Y / viewPort_[_playerIndex].h) * GAUGE_SIZE_MULTI);

	//表示場所
	float size = 1.0f / (Application::SCREEN_SIZE_Y / viewPort_[_playerIndex].h) * GAUGE_SIZE_MULTI;

	//外枠
	DrawRotaGraph(chargeGaugePos.x, chargeGaugePos.y, size, 0.0, gaugeImg_, true);

	//定数バッファ
	material_->SetConstBuf(0, { 0.5f,0.5f,chargeCnt,gaugeCnt_ });

	//描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	renderer_->Draw(chargeGaugePos - gaugeSize / 2, gaugeSize);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	int speed100 = static_cast<int>(speed) / 100;
	int speed10 = static_cast<int>(speed) / 10;
	int speed1 = static_cast<int>(speed) % 10;
	//速度
	DrawRotaGraph(chargeGaugePos.x - (NUMBER_LOCAL_POS * size), chargeGaugePos.y, size, 0.0, numImgs_[speed100], true);
	DrawRotaGraph(chargeGaugePos.x, chargeGaugePos.y, size, 0.0, numImgs_[speed10], true);
	DrawRotaGraph(chargeGaugePos.x + (NUMBER_LOCAL_POS * size), chargeGaugePos.y, size, 0.0, numImgs_[speed1], true);
}

void UIManager::DrawHealth(const int _playerIndex)
{
	//プレイヤー
	const auto& player = PlayerManager::GetInstance().GetPlayer(_playerIndex);

	const float nowHealth = player.GetNowHealth();
	const Parameter& param = player.GetAllParam();

	int posX = viewPort_[_playerIndex].x;
	int posY = viewPort_[_playerIndex].y;

	int healthPosX = posX + static_cast<int>(HEALTH_POS.x * viewPort_[_playerIndex].w);
	int healthPosY = posY + static_cast<int>(HEALTH_POS.y * viewPort_[_playerIndex].h);

	//最大体力
	DrawBox(healthPosX - HEALTH_BOX_LOCAL_POS_X,
		healthPosY + HEALTH_BOX_LOCAL_POS_Y,
		healthPosX + HEALTH_BOX_LOCAL_POS_X,
		healthPosY + HEALTH_BOX_LOCAL_POS_Y - param.maxHealth_ * HEALTH_BOX,
		Utility::BLACK, true);

	if (nowHealth > 0.0f)
	{
		//体力
		DrawBox(healthPosX - HEALTH_BOX_LOCAL_POS_X + HEALTH_LOCAL,
			healthPosY + HEALTH_BOX_LOCAL_POS_Y - HEALTH_LOCAL,
			healthPosX + HEALTH_BOX_LOCAL_POS_X - HEALTH_LOCAL,
			healthPosY + HEALTH_BOX_LOCAL_POS_Y - (param.maxHealth_ * HEALTH_BOX) * (nowHealth / param.maxHealth_) + HEALTH_LOCAL,
			Utility::RED, true);
	}
}

void UIManager::DrawParam(const int _playerIndex)
{
	//プレイヤー
	const auto& player = PlayerManager::GetInstance().GetPlayer(_playerIndex);

	//パラメーター
	Parameter param = player.GetParam();

	//ビューポート
	Viewport view = viewPort_[_playerIndex];

	//倍率
	int magnification = Application::SCREEN_SIZE_Y / view.h;

	//間隔
	int interval = PARAM_BOX_INTERVAL / magnification;
	int powerUpInterval = POWER_UP_INTERVAL / magnification;

	//サイズ
	int size = PARAM_BOX_SIZE / magnification;

	//パワーアップ画像
	int powerUpSize = size / 2;

	//座標
	Vector2 pos = {};
	pos.x = PARAM_BOX_START_POS.x * view.w;
	pos.y = PARAM_BOX_START_POS.y * view.h;

	//最高速
	DrawExtendGraph(pos.x - powerUpInterval, pos.y, pos.x + size - powerUpInterval, pos.y + size, maxSpeedImg_, true);
	DrawBox(pos.x, pos.y, pos.x + (size * static_cast<int>(param.maxSpeed_)), pos.y + size, Utility::CYAN, true);
	pos.y += interval + size;

	//加速
	DrawExtendGraph(pos.x - powerUpInterval, pos.y, pos.x + size - powerUpInterval, pos.y + size, acceleImg_, true);
	DrawBox(pos.x, pos.y, pos.x + (size * static_cast<int>(param.acceleration_)), pos.y + size, Utility::PURPLE, true);
	pos.y += interval + size;

	//旋回
	DrawExtendGraph(pos.x - powerUpInterval, pos.y, pos.x + size - powerUpInterval, pos.y + size, turnImg_, true);
	DrawBox(pos.x, pos.y, pos.x + (size * static_cast<int>(param.turning_)), pos.y + size, Utility::GREEN, true);
	pos.y += interval + size;

	//チャージ
	DrawExtendGraph(pos.x - powerUpInterval, pos.y, pos.x + size - powerUpInterval, pos.y + size, chargeImg_, true);
	DrawBox(pos.x, pos.y, pos.x + (size * static_cast<int>(param.charge_)), pos.y + size, Utility::YELLOW, true);
	pos.y += interval + size;

	//飛行
	DrawExtendGraph(pos.x - powerUpInterval, pos.y, pos.x + size - powerUpInterval, pos.y + size, flightImg_, true);
	DrawBox(pos.x, pos.y, pos.x + (size * static_cast<int>(param.flight_)), pos.y + size, Utility::WHITE, true);
	pos.y += interval + size;

	//重さ
	DrawExtendGraph(pos.x - powerUpInterval, pos.y, pos.x + size - powerUpInterval, pos.y + size, weightImg_, true);
	DrawBox(pos.x, pos.y, pos.x + (size * static_cast<int>(param.weight_)), pos.y + size, Utility::BROWN, true);
	pos.y += interval + size;

	//攻撃
	DrawExtendGraph(pos.x - powerUpInterval, pos.y, pos.x + size - powerUpInterval, pos.y + size, attackImg_, true);
	DrawBox(pos.x, pos.y, pos.x + (size * static_cast<int>(param.attack_)), pos.y + size, Utility::ORANGE, true);
	pos.y += interval + size;

	//防御
	DrawExtendGraph(pos.x - powerUpInterval, pos.y, pos.x + size - powerUpInterval, pos.y + size, defenceImg_, true);
	DrawBox(pos.x, pos.y, pos.x + (size * static_cast<int>(param.defence_)), pos.y + size, Utility::BLUE, true);
	pos.y += interval + size;

	//最大体力
	DrawExtendGraph(pos.x - powerUpInterval, pos.y, pos.x + size - powerUpInterval, pos.y + size, maxHealthImg_, true);
	DrawBox(pos.x, pos.y, pos.x + (size * static_cast<int>(param.maxHealth_)), pos.y + size, Utility::PINK, true);
	pos.y += interval + size;
}
