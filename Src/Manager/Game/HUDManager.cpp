#include "../../pch.h"
#include "../../Application.h"
#include "../../Utility/Utility.h"
#include "../../Common/Vector2.h"
#include "../System/SceneManager.h"
#include "../System/ResourceManager.h"
#include "../System/KeyConfig.h"
#include "../System/SplitScreenManager.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "../../Object/Player/Player.h"
#include "../../Object/Player/Action/MachineAction.h"
#include "../../Object/Player/Logic/UserLogic.h"
#include "HUDManager.h"

void HUDManager::LoadOutSide(void)
{
	//インスタンス
	ResourceManager& res = ResourceManager::GetInstance();
	const SceneManager& scnMng = SceneManager::GetInstance();

	//描画
	for (auto& playerHud : playerHUD_)
	{
		playerHud.hudData[static_cast<int>(HUD_TYPE::PUSH_BUTTON)] = { &HUDManager::DrawPushButton,false };
		playerHud.hudData[static_cast<int>(HUD_TYPE::CHARGE_GAUGE)] = { &HUDManager::DrawChargeGauge,false };
		playerHud.hudData[static_cast<int>(HUD_TYPE::HEALTH)] = { &HUDManager::DrawHealth,false };
		playerHud.hudData[static_cast<int>(HUD_TYPE::PARAMETER)] = { &HUDManager::DrawParam,false };
		playerHud.hudData[static_cast<int>(HUD_TYPE::GET_OFF)] = { &HUDManager::DrawGetOff,false };
	}

	//入力画像
	pushImg_ = res.Load(ResourceManager::SRC::PUSH).handleId_;
	mouseImg_ = res.Load(ResourceManager::SRC::MOUSE).handleId_;
	mouseLeftImg_ = res.Load(ResourceManager::SRC::MOUSE_LEFT).handleId_;
	mouseRightImg_ = res.Load(ResourceManager::SRC::MOUSE_RIGHT).handleId_;
	mouseMoveImg_ = res.Load(ResourceManager::SRC::MOUSE_MOVE).handleId_;
	aButtonImg_ = res.Load(ResourceManager::SRC::A_BUTTON).handleId_;
	bButtonImg_ = res.Load(ResourceManager::SRC::B_BUTTON).handleId_;
	xButtonImg_ = res.Load(ResourceManager::SRC::X_BUTTON).handleId_;
	yButtonImg_ = res.Load(ResourceManager::SRC::Y_BUTTON).handleId_;

	//ゲージ画像
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

	//チャージゲージカウント
	cnt_ = 0.0f;

	//マテリアル
	pushButtonMaterial_ = std::make_unique<PixelMaterial>(L"Blinking.cso", 1);
	pushButtonMaterial_->AddConstBuf({ cnt_,BLINKING_SPEED ,0.0f,0.0f });
	pushButtonMaterial_->AddTextureBuf(pushImg_);

	chargeGaugeMaterial_ = std::make_unique<PixelMaterial>(L"GaugeMask.cso", 1);
	chargeGaugeMaterial_->AddConstBuf({ 0.5f,0.5f,0.0f,cnt_ });
	chargeGaugeMaterial_->AddTextureBuf(gaugeMaskImg_);

	getOffMaterial_ = std::make_unique<PixelMaterial>(L"CircleGauge.cso", 2);
	getOffMaterial_->AddConstBuf({ 0.5f,0.5f,0.0f,0.0f });
	getOffMaterial_->AddConstBuf({ GET_OFF_RADIUS,0.0f,0.0f,0.0f });
}

void HUDManager::Init(void)
{
	//分割スクリーン
	auto& split = SplitScreenManager::GetInstance();
	int activeViewCnt = split.GetActiveViewCount();
	int plNum = static_cast<int>(GameSetting::PLAYER_MAX_NUM) - 1;

	for (int i = 0; i < activeViewCnt; i++)
	{
		for (int j = 0; j < plNum; j++)
		{
			//全描画を非表示
			SetVisible(i, static_cast<HUD_TYPE>(j), false);
		}
	}
}

void HUDManager::Update(void)
{
	//デルタタイム
	const auto& delta = SceneManager::GetInstance().GetDeltaTime();

	//カウンタ
	cnt_ += delta;
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

void HUDManager::DrawPushButton(const int _playerIndex)
{
	//プレイヤー
	const auto* player = playerHUD_[_playerIndex].player;

	//存在しないなら何もしない
	if (!player)return;

	//分割
	const auto& view = SplitScreenManager::GetInstance().GetViewport(_playerIndex);

	//サイズ比率(XYほぼ同一なので統一)
	float scale = static_cast<float>(view.height) / Application::SCREEN_SIZE_Y;

	//座標
	Vector2 pos = { view.x + static_cast<int>(Application::SCREEN_HALF_X * scale),view.y + static_cast<int>(Application::SCREEN_HALF_Y * scale) };
	int localPosX = static_cast<int>(PUSH_LOCAL_POS_X * scale);
	Vector2 pushSize = { static_cast<int>(PUSH_SIZE_X * scale),static_cast<int>(PUSH_SIZE_Y * scale) };

	//Push画像
	pushButtonMaterial_->SetConstBuf(0, { cnt_,BLINKING_SPEED,0.0f,0.0f });
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	pushButtonMaterial_->SetTextureBuf(0, pushImg_);
	renderer_.Draw(*pushButtonMaterial_, Vector2(pos.x - localPosX - pushSize.x / 2, pos.y - pushSize.y / 2), pushSize);

	//ボタン
	Vector2 buttonSize = { static_cast<int>(BUTTON_SIZE * scale),static_cast<int>(BUTTON_SIZE * scale) };
	pushButtonMaterial_->SetTextureBuf(0, aButtonImg_);
	renderer_.Draw(*pushButtonMaterial_, Vector2(pos.x + localPosX - buttonSize.x / 2, pos.y - buttonSize.y / 2), buttonSize);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void HUDManager::DrawChargeGauge(const int _playerIndex)
{
	//プレイヤー
	const auto* player = playerHUD_[_playerIndex].player;

	//存在しないなら何もしない
	if (!player)return;

	//アクションのポインタ(キャスト用)
	const auto actionPtr = &player->GetAction();

	//ポインタをキャストし、MachineActionではなかった場合、処理しないように
	const auto* mAction = dynamic_cast<const MachineAction*>(actionPtr);
	if (!mAction)return;

	//分割
	const auto& view = SplitScreenManager::GetInstance().GetViewport(_playerIndex);

	//各要素
	const float chargeCnt = mAction->GetChargeCnt();
	const float speed = mAction->GetSpeed();

	//サイズ比率(XYほぼ同一なので統一)
	float scale = static_cast<float>(view.height) / Application::SCREEN_SIZE_Y;

	//中央位置
	Vector2 gaugeCenter =
	{
		view.x + static_cast<int>(CHARGE_POS.x * view.width) - static_cast<int>(GAUGE_LOCAL_POS * scale),
		view.y + static_cast<int>(CHARGE_POS.y * view.height) - static_cast<int>(GAUGE_LOCAL_POS * scale)
	};

	//合計サイズ
	float totalScale = scale * GAUGE_SIZE_MULTI;
	Vector2 gaugeSize = { static_cast<int>(GAUGE_SIZE * totalScale), static_cast<int>(GAUGE_SIZE * totalScale) };

	//外枠の描画
	DrawRotaGraph(gaugeCenter.x, gaugeCenter.y, totalScale, 0.0, gaugeImg_, true);

	//メーターの描画
	chargeGaugeMaterial_->SetConstBuf(0, { 0.5f,0.5f,chargeCnt,cnt_ });
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	renderer_.Draw(*chargeGaugeMaterial_, gaugeCenter - gaugeSize / 2, gaugeSize);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//速度計の描画(3桁までの描画)
	int speedInt = static_cast<int>(speed);
	std::array<int, 3>digits = { (speedInt / 100),(speedInt / 10) % 10, speedInt % 10 };
	float offset = NUMBER_LOCAL_POS * scale;

	DrawRotaGraph(gaugeCenter.x - static_cast<int>(offset), gaugeCenter.y, totalScale, 0.0, numImgs_[digits[0]], true);
	DrawRotaGraph(gaugeCenter.x, gaugeCenter.y, totalScale, 0.0, numImgs_[digits[1]], true);
	DrawRotaGraph(gaugeCenter.x + static_cast<int>(offset), gaugeCenter.y, totalScale, 0.0, numImgs_[digits[2]], true);
}

void HUDManager::DrawHealth(const int _playerIndex)
{
	//プレイヤー
	const auto* player = playerHUD_[_playerIndex].player;

	//存在しないなら何もしない
	if (!player)return;

	//分割
	const auto& view = SplitScreenManager::GetInstance().GetViewport(_playerIndex);

	//サイズ比率(XYほぼ同一なので統一)
	float scale = static_cast<float>(view.height) / Application::SCREEN_SIZE_Y;

	//HP
	float health = player->GetNowHealth();
	float maxHealth = player->GetMaxHealth();

	//割合
	float rate = health / maxHealth;
	float barWidth = HEALTH_BAR_WIDTH * scale;
	float barHeight = HEALTH_BAR_HEIGHT * scale;

	//位置
	Vector2 pos = {};
	pos.x = view.x + static_cast<int>(view.width * HEALTH_POS.x);
	pos.y = view.y + static_cast<int>(view.height * HEALTH_POS.y - barHeight);
	float hpHeight = static_cast<int>(barHeight * rate);
	float top = pos.y + (barHeight - hpHeight);
	int frame = static_cast<int>(FRAME_SIZE * scale);

	//枠
	DrawBox(pos.x - frame,
		pos.y - frame,
		pos.x + static_cast<int>(barWidth) + frame,
		pos.y + static_cast<int>(barHeight) + frame,
		Utility::GRAY, true);
	//HPバー
	if (health > 0.0f)
	{
		DrawBox(pos.x,
			static_cast<int>(top),
			pos.x + static_cast<int>(barWidth),
			pos.y + static_cast<int>(barHeight),
			Utility::RED, true);
	}
}

void HUDManager::DrawParam(const int _playerIndex)
{
	//プレイヤー
	const auto* player = playerHUD_[_playerIndex].player;

	//存在しないなら何もしない
	if (!player)return;

	//分割
	const auto& view = SplitScreenManager::GetInstance().GetViewport(_playerIndex);
	
	//サイズ比率(XYほぼ同一なので統一)
	float scale = static_cast<float>(view.height) / Application::SCREEN_SIZE_Y;

	//パラメーター
	const auto& param = player->GetParam();

	//パラメーター情報まとめ
	ParamInfo params[] =
	{
		{static_cast<int>(param.maxSpeed), maxSpeedImg_, Utility::CYAN},
		{static_cast<int>(param.acceleration), acceleImg_, Utility::PURPLE},
		{static_cast<int>(param.turning), turnImg_, Utility::GREEN},
		{static_cast<int>(param.charge), chargeImg_, Utility::YELLOW},
		{static_cast<int>(param.flight), flightImg_, Utility::WHITE},
		{static_cast<int>(param.weight), weightImg_, Utility::BROWN},
		{static_cast<int>(param.attack), attackImg_, Utility::ORANGE},
		{static_cast<int>(param.defence), defenceImg_, Utility::BLUE},
		{static_cast<int>(param.maxHealth), maxHealthImg_, Utility::RED}
	};

	//開始座標
	Vector2 start = {};
	start.x = view.x + static_cast<int>(view.width * PARAM_BOX_START_POS.x);
	start.y = view.y + static_cast<int>(view.height * PARAM_BOX_START_POS.y);

	//座標
	int x;
	int y;

	//数値関係
	int numberLocal = static_cast<int>((PARAM_NUMBER_LOCAL_POS - NUMBER_INTERVAL / 2) * scale);
	int powerUpInterval = static_cast<int>(POWER_UP_INTERVAL * scale);
	int boxSize = static_cast<int>(PARAM_BOX_SIZE * scale);
	int numberInterval = static_cast<int>(NUMBER_INTERVAL * scale);
	int paramBoxInterval = static_cast<int>(PARAM_BOX_INTERVAL * scale);
	int tens;
	int one;
	
	//パラメーターの個数
	int size = static_cast<int>(std::size(params));
	for (int i = 0; i < size; i++)
	{
		//座標の調整
		x = start.x;
		y = start.y + i * powerUpInterval;

		//アイコン
		DrawRotaGraph(x, y, scale, 0.0, params[i].icon, true);

		//数値
		tens = params[i].value / 10;
		one = params[i].value % 10;
		x += numberLocal;
		DrawRotaGraph(x, y, scale, 0.0, numImgs_[tens], true);
		DrawRotaGraph(x + numberInterval, y, scale, 0.0, numImgs_[one], true);

		//バー
		x += paramBoxInterval;
		DrawBox(x,
			y - boxSize / 2,
			x + boxSize * params[i].value,
			y + boxSize / 2,
			params[i].color,
			true);
	}
}

void HUDManager::DrawGetOff(const int _playerIndex)
{
	//プレイヤー参照
	const Player* player = playerHUD_[_playerIndex].player;

	//降車(スペシャル)ボタンを押した時間
	const float getOffCnt = KeyConfig::GetInstance().GetKeyTrgHoldCnt(KeyConfig::CONTROL_TYPE::SPECIAL_BUTTON, static_cast<KeyConfig::JOYPAD_NO>(_playerIndex + 1));

	//0なら表示しない
	if (getOffCnt <= 0.0f)return;

	//降りるまでの時間
	float getOffTime = UserLogic::GETOFF_PUSH_TIME;

	//割合(0.0～1.0)
	float gaugePercent = std::clamp(getOffCnt / getOffTime, 0.0f, 1.0f);

	//分割
	const auto& view = SplitScreenManager::GetInstance().GetViewport(_playerIndex);

	//サイズ比率(XYほぼ同一なので統一)
	float scale = static_cast<float>(view.height) / Application::SCREEN_SIZE_Y;

	//中央位置
	Vector2 gaugeCenter =
	{
		view.x + static_cast<int>(GET_OFF_POS.x * view.width),
		view.y + static_cast<int>(GET_OFF_POS.y * view.height)
	};

	//サイズ比率反映
	float size = GET_OFF_SIZE * scale;
	float frame = GET_OFF_FRAME_SIZE * scale;
	Vector2 sizeVec = Vector2(static_cast<int>(size), static_cast<int>(size));

	//枠の円
	DrawCircle(gaugeCenter.x + static_cast<int>(size / 2), gaugeCenter.y + static_cast<int>(size / 2), static_cast<int>(size * GET_OFF_RADIUS + frame), 0x0);

	getOffMaterial_->SetConstBuf(0, { 0.5f,0.5f,gaugePercent,cnt_ });
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	renderer_.Draw(*getOffMaterial_, gaugeCenter, sizeVec);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}