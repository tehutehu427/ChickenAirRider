#include"../pch.h"
#include"../System/ResourceManager.h"
#include "SoundManager.h"

SoundManager::SoundManager(void)
{  
	// 音量の初期化
	for (int i = 0; i < static_cast<int>(TYPE::MAX); ++i)
	{
		volume_[i] = DEFAULT_VOLUME;
	}
}

SoundManager::~SoundManager(void)
{
}

void SoundManager::LoadOutSide(void)
{
    //リソース
    ResourceManager& resMng = ResourceManager::GetInstance();

#pragma region BGM

    //タイトルBGM
    int id = resMng.Load(ResourceManager::SRC::TITLE_BGM).handleId_;
    Add(SOUND_NAME::TITLE_BGM, id, TYPE::BGM, TITLE_BGM_VOL);

    //セレクトBGM
    id = resMng.Load(ResourceManager::SRC::SELECT_BGM).handleId_;
    Add(SOUND_NAME::SELECT_BGM, id, TYPE::BGM);

    //ゲームBGM
    id = resMng.Load(ResourceManager::SRC::MAIN_GAME_BGM).handleId_;
    Add(SOUND_NAME::MAIN_GAME_BGM, id, TYPE::BGM);

	//最終ゲームBGM
    id = resMng.Load(ResourceManager::SRC::LAST_GAME_BGM).handleId_;
    Add(SOUND_NAME::LAST_GAME_BGM, id, TYPE::BGM);

    //リザルトBGM
    id = resMng.Load(ResourceManager::SRC::RESULT_BGM).handleId_;
    Add(SOUND_NAME::RESULT_BGM, id, TYPE::BGM);

#pragma endregion BGM

#pragma region SE

    //決定音
    id = resMng.Load(ResourceManager::SRC::ENTER_SE).handleId_;
    Add(SOUND_NAME::ENTER, id, TYPE::SE, OPTION_SE_VOL);

	//選択音
    id = resMng.Load(ResourceManager::SRC::SELECT_SE).handleId_;
    Add(SOUND_NAME::SELECT_SE, id, TYPE::SE, OPTION_SE_VOL);

	//キャンセル音
    id = resMng.Load(ResourceManager::SRC::CANCEL_SE).handleId_;
    Add(SOUND_NAME::CANCEL, id, TYPE::SE, OPTION_SE_VOL);

    //エンジン音
    id = resMng.Load(ResourceManager::SRC::ENGINE_SE).handleId_;
    Add(SOUND_NAME::ENGINE, id, TYPE::SE, ENGINE_SE_VOL);

    //チャージ
    id = resMng.Load(ResourceManager::SRC::CHARGE_SE).handleId_;
    Add(SOUND_NAME::CHARGE, id, TYPE::SE, CHARGE_SE_VOL);

    //チャージ完了
    id = resMng.Load(ResourceManager::SRC::CHARGE_MAX_SE).handleId_;
    Add(SOUND_NAME::CHARGE_MAX, id, TYPE::SE, CHARGE_MAX_SE_VOL);

    //ブースト
    id = resMng.Load(ResourceManager::SRC::BOOST_SE).handleId_;
    Add(SOUND_NAME::BOOST, id, TYPE::SE, BOOST_SE_VOL);

    //スピン
    id = resMng.Load(ResourceManager::SRC::SPIN_SE).handleId_;
    Add(SOUND_NAME::SPIN, id, TYPE::SE, SPIN_SE_VOL);

    //ジャンプ
    id = resMng.Load(ResourceManager::SRC::JUMP_SE).handleId_;
    Add(SOUND_NAME::JUMP, id, TYPE::SE);

    //アイテムゲット
    id = resMng.Load(ResourceManager::SRC::GET_ITEM_SE).handleId_;
    Add(SOUND_NAME::GET_ITEM, id, TYPE::SE);

    //ダメージ
    id = resMng.Load(ResourceManager::SRC::DAMAGE_SE).handleId_;
    Add(SOUND_NAME::DAMAGE, id, TYPE::SE);

    //カウントダウン
    id = resMng.Load(ResourceManager::SRC::COUNT_DOWN_SE).handleId_;
    Add(SOUND_NAME::COUNT_DOWN_SE, id, TYPE::SE);
    
	//タイムアップ
    id = resMng.Load(ResourceManager::SRC::TIME_UP_SE).handleId_;
    Add(SOUND_NAME::TIME_UP_SE, id, TYPE::SE);

#pragma endregion SE

}

void SoundManager::Destroy(void)
{
    for (auto& p : info_)
    {
        DeleteSoundMem(p.second.handleId);
    }

    info_.clear();

    if (instance_ != nullptr)
    {
        delete instance_;
        instance_ = nullptr;
    }
}

const bool SoundManager::Add(const SOUND_NAME _name, const int _id, const TYPE _soundType, const int _volumePercent)
{
    //IDがない　または　タイプが不適切
    if (_id == -1 || _soundType == TYPE::MAX)
    {
        //追加失敗
        return false;
    }

    //既にある場合
    if (info_.contains(_name))
    {
        //そのまま(入っているのでtrue)
        return true;
    }

    //保存
    SoundInfo soundInfo;
    soundInfo.handleId = _id;
    soundInfo.type = _soundType;
    soundInfo.volumePercent = _volumePercent;

    //最大音量
    constexpr int VOLUME_MAX = 255;  
    float soundPer = static_cast<float>(soundInfo.volumePercent) / static_cast<float>(PERCENT_MAX);
    int soundPal = static_cast<int>(VOLUME_MAX * volume_[static_cast<int>(_soundType)] * soundPer);
    soundPal /= PERCENT_MAX;

    //音量の設定
    ChangeVolumeSoundMem(soundPal, soundInfo.handleId);

    //追加
    info_.emplace(_name, soundInfo);

    return true;
}

void SoundManager::Play(const SOUND_NAME _name, const PLAYTYPE _playType)
{
	//音源が読み込まれていない場合はエラー
	assert(loadedMap_[_src].handleId != -1, "音源が読み込まれていない又は見つかりません");

    //音源が再生済みか調べる
	if (CheckSoundMem(info_[_name].handleId) == 1 &&
        _playType != PLAYTYPE::BACK)
	{
		Stop(_name);  // 再生済みなら停止
	}

    //音源の再生
    PlaySoundMem(info_[_name].handleId, GetPlayType(_playType));
}

void SoundManager::Stop(const SOUND_NAME _name)
{
    //音源の停止
    StopSoundMem(info_[_name].handleId);
}

void SoundManager::StopAll(void)
{
    for (auto& info : info_)
    {
        //音源の停止
        StopSoundMem(info.second.handleId);
    }
}

bool SoundManager::IsPlay(const SOUND_NAME _name) const
{
    const auto it = info_.find(_name);
    if (it == info_.end())
    {
        return false; // 見つからない場合は未再生とする
    }
    return CheckSoundMem(it->second.handleId) == 1;
}

void SoundManager::SetVolume(const SOUND_NAME _name, const int _volumePercent)
{
	//音量の設定
    info_[_name].volumePercent = _volumePercent;

    float soundPer = static_cast<float>(info_[_name].volumePercent) / static_cast<float>(PERCENT_MAX);
    int soundPal = static_cast<int>(VOLUME_MAX * volume_[static_cast<int>(info_[_name].type)] * soundPer);
    ChangeVolumeSoundMem(soundPal / PERCENT_MAX, info_[_name].handleId);
}

void SoundManager::SetSystemVolume(const int _volumePercent, const int _type)
{    
    //音量設定
    volume_[_type] = _volumePercent;
   
    //音量調整
	for (const auto& pair : info_)
	{
        //種類が異なるものはスキップ
		if (pair.second.type != static_cast<TYPE>(_type)) 
        {
			continue;
		}
        float soundPer = static_cast<float>(pair.second.volumePercent) / static_cast<float>(PERCENT_MAX);
        int soundPal = static_cast<int>(VOLUME_MAX * volume_[_type] * soundPer);

        ChangeVolumeSoundMem(soundPal / PERCENT_MAX, pair.second.handleId);
	}
}

int SoundManager::GetPlayType(const PLAYTYPE _playType)
{
    switch (_playType)
    {
    case PLAYTYPE::NORMAL:
        return DX_PLAYTYPE_NORMAL;
        break;

    case PLAYTYPE::LOOP:
        return DX_PLAYTYPE_LOOP;
        break;

    case PLAYTYPE::BACK:
        return DX_PLAYTYPE_BACK;
        break;

    default:
        return DX_PLAYTYPE_NORMAL;
        break;
    }
}
