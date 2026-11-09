#include "../../pch.h"
#include"../../Application.h"
#include "../Common/Vector2.h"
#include "../Utility/Utility.h"
#include "../System/ResourceManager.h"
#include "../System/SplitScreenManager.h"
#include "../System/Camera.h"
#include "ItemManager.h"
#include "MachineManager.h"
#include "../../Object/Player/Player.h"
#include "WorldUIManager.h"

void WorldUIManager::Init(void)
{
	//分割スクリーン
	auto& split = SplitScreenManager::GetInstance();
	int activeViewCnt = split.GetActiveViewCount();

	for (int i = 0; i < activeViewCnt; i++)
	{
		for (int j = 0; j < static_cast<int>(WORLD_UI_TYPE::MAX); j++)
		{
			//全描画を非表示
			SetVisible(i, static_cast<WORLD_UI_TYPE>(j), false);
		}
	}
}

void WorldUIManager::Update(void)
{
}

void WorldUIManager::Draw(const int _playerIndex)
{
	//ワールドUIの描画
	DrawWorldUI(_playerIndex);
}

void WorldUIManager::SetPlayer(const Player* _player, const int _index)
{
	//プレイヤーが存在しない　または　ゲーム最大人数より大きかったら無視
	if (!_player || _index > GameSetting::PLAYER_MAX_NUM)return;

	//プレイヤー設定
	playerWorldUi_[_index].player = _player;
}

void WorldUIManager::SetVisible(const int _index, const WORLD_UI_TYPE _type, const bool _visible)
{
	playerWorldUi_[_index].hudData[static_cast<size_t>(_type)].visible = _visible;
}

WorldUIManager::WorldUIManager(void)
{
}

WorldUIManager::~WorldUIManager(void)
{
}

void WorldUIManager::LoadOutSide(void)
{
	//インスタンス
	ResourceManager& res = ResourceManager::GetInstance();

	//描画
	for (auto& playerHud : playerWorldUi_)
	{
		playerHud.hudData[static_cast<int>(WORLD_UI_TYPE::ITEM_BOX)] = { &WorldUIManager::DrawItemBox,false };
		playerHud.hudData[static_cast<int>(WORLD_UI_TYPE::MACHINE)] = { &WorldUIManager::DrawMachine,false };
	}

	//注目用括弧画像
	attentionImg_ = res.Load(ResourceManager::SRC::ATTENTION).handleId_;
}

void WorldUIManager::DrawWorldUI(const int _playerIndex)
{
	//プレイヤー参照
	const Player* player = playerWorldUi_[_playerIndex].player;

	//存在しないなら何もしない
	if (!player)return;

	//プレイヤーのHUD情報
	auto& huds = playerWorldUi_[_playerIndex].hudData;

	for (const auto& hud : huds)
	{
		//非表示判定
		if (!hud.visible)continue;

		//描画
		(this->*hud.func)(_playerIndex);
	}
}

void WorldUIManager::DrawItemBox(const int _playerIndex)
{
	//プレイヤー
	const auto* player = playerWorldUi_[_playerIndex].player;

	//存在しないなら何もしない
	if (!player)return;

	//カメラ
	const auto& camera = player->GetCamera().lock();

	//分割
	const auto& view = SplitScreenManager::GetInstance().GetViewport(_playerIndex);

	//サイズ比率(XYほぼ同一なので統一)
	float scale = static_cast<float>(view.height) / Application::SCREEN_SIZE_Y;
	float size = ATTENTION_SIZE * scale;

	//カメラ座標
	VECTOR cameraPos = camera->GetPos();
	VECTOR cameraForward = camera->GetForward();
	VECTOR cameraUp = camera->GetUp();
	float fov = camera->GetFov();

	//アイテムボックスの座標
	const auto& boxesPos = ItemManager::GetInstance().GetItemBoxPos();

	//一番近いアイテムボックスの座標を取得
	float minDistance = FLT_MAX;
	VECTOR nearestItemBoxPos = {};
	for (const auto& boxPos : boxesPos)
	{
		//カメラの視界外なら無視
		if (CheckCameraViewClip(boxPos))continue;

		float distance = Utility::Distance(cameraPos, boxPos);
		if (distance < minDistance)
		{
			minDistance = distance;
			nearestItemBoxPos = boxPos;
		}
	}

	//距離が最大距離を超えていたら描画しない
	if (minDistance > ITEM_BOX_MAX_DISTANCE)return;

	//画面座標に変換
	Vector2 screenPos = Utility::WorldPosToScreenPos(nearestItemBoxPos, cameraPos, cameraForward, cameraUp, static_cast<float>(view.height), fov);

	//距離でサイズを変化させる
	float minSize = 0.5f;
	float maxSize = 1.0f;
	float sizeScale = std::clamp(1.0f - (minDistance / ITEM_BOX_MAX_DISTANCE), minSize, maxSize);

	//注目用括弧の描画
	DrawRotaGraph(screenPos.x, screenPos.y, sizeScale * size, 0.0, attentionImg_, true);
}

void WorldUIManager::DrawMachine(const int _playerIndex)
{
	//プレイヤー
	const auto* player = playerWorldUi_[_playerIndex].player;

	//存在しないなら何もしない
	if (!player)return;

	//カメラ
	const auto& camera = player->GetCamera().lock();

	//分割
	const auto& view = SplitScreenManager::GetInstance().GetViewport(_playerIndex);

	//サイズ比率(XYほぼ同一なので統一)
	float scale = static_cast<float>(view.height) / Application::SCREEN_SIZE_Y;
	float size = ATTENTION_SIZE * scale;

	//カメラ座標
	VECTOR cameraPos = camera->GetPos();
	VECTOR cameraForward = camera->GetForward();
	VECTOR cameraUp = camera->GetUp();
	float fov = camera->GetFov();

	//アイテムボックスの座標
	const auto& machinesPos = MachineManager::GetInstance().GetMachinePos();

	//一番近いアイテムボックスの座標を取得
	float minDistance = FLT_MAX;
	VECTOR nearestMachinePos = {};
	for (const auto& machinePos : machinesPos)
	{
		//カメラの視界外なら無視
		if (CheckCameraViewClip(machinePos))continue;

		float distance = Utility::Distance(cameraPos, machinePos);
		if (distance < minDistance)
		{
			minDistance = distance;
			nearestMachinePos = machinePos;
		}
	}

	//距離が最大距離を超えていたら描画しない
	if (minDistance > MACHINE_MAX_DISTANCE)return;

	//画面座標に変換
	Vector2 screenPos = Utility::WorldPosToScreenPos(nearestMachinePos, cameraPos, cameraForward, cameraUp, static_cast<float>(view.height), fov);

	//距離でサイズを変化させる
	float minSize = 0.5f;
	float maxSize = 1.0f;
	float sizeScale = std::clamp(1.0f - (minDistance / MACHINE_MAX_DISTANCE), minSize, maxSize);

	//注目用括弧の描画
	DrawRotaGraph(screenPos.x, screenPos.y, sizeScale * size, 0.0, attentionImg_, true);
}
