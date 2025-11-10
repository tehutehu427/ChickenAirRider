#include"../pch.h"
#include"LoaderManager.h"
#include"../Object/Player/Parameter/UnitParameter.h"
#include"../Manager/Game/StageManager.h"
#include"../Object/Stage/StageImportData.h"
#include"../Object/Player/AnimationImportData.h"
#include"DataRegistry.h"

void DataRegistry::CreateAll(void)
{
	//ユニットのパラメーター
	LoaderManager<UnitParameter>::CreateInstance();
	
	//ステージの情報
	LoaderManager<StageImportData>::CreateInstance();

	//アニメーション情報
	LoaderManager<AnimationImportData>::CreateInstance();

	//機体情報
	LoaderManager<AnimationImportData>::CreateInstance();
}
