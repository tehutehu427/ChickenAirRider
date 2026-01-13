#include"../pch.h"
#include"LoaderManager.h"
#include"../Manager/Game/StageManager.h"
#include"../Object/Stage/StageImportData.h"
#include"../Object/Player/AnimationImportData.h"
#include"../Object/Player/Machine/MachineImportData.h"
#include"../Object/Item/ItemImportData.h"
#include"../Object/Item/BoxCreatePositionData.h"
#include"DataRegistry.h"

void DataRegistry::CreateAll(void)
{	
	//ステージの情報
	LoaderManager<StageImportData>::CreateInstance(SingletonRegistry::DESTROY_TIMING::ALL_END);

	//アニメーション情報
	LoaderManager<AnimationImportData>::CreateInstance(SingletonRegistry::DESTROY_TIMING::ALL_END);

	//機体情報
	LoaderManager<MachineImportData>::CreateInstance(SingletonRegistry::DESTROY_TIMING::ALL_END);

	//アイテム情報
	LoaderManager<PowerUpItemImportData>::CreateInstance(SingletonRegistry::DESTROY_TIMING::ALL_END);

	//アイテム情報
	LoaderManager<BattleItemImportData>::CreateInstance(SingletonRegistry::DESTROY_TIMING::ALL_END);

	//アイテム情報
	LoaderManager<BoxCreatePositionData>::CreateInstance(SingletonRegistry::DESTROY_TIMING::ALL_END);
}
