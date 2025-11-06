#include"../pch.h"
#include"LoaderManager.h"
#include"../Object/Player/Parameter/UnitParameter.h"
#include"../Manager/Game/StageManager.h"
#include"../Object/Stage/StageImportData.h"
#include"../Object/Player/Character/CharaImportData.h"
#include"DataRegistry.h"

void DataRegistry::CreateAll(void)
{
	//ユニットのパラメーター
	LoaderManager<UnitParameter>::CreateInstance();
	
	//ステージの情報
	LoaderManager<StageImportData>::CreateInstance();

	//キャラクター情報
	LoaderManager<CharaImportData>::CreateInstance();
}
