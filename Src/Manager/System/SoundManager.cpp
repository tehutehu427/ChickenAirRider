#include"../pch.h"
#include "SoundManager.h"
#include "../../Application.h"

SoundManager::SoundManager()
{  
	// 音量の初期化
	for (int i = 0; i < TYPE_MAX; ++i)
	{
		volume_[i] = DEFAULT_VOLUME;
	}
}

SoundManager::~SoundManager()
{
}

void SoundManager::Destroy()
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

void SoundManager::Init()
{
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
    int soundPal = VOLUME_MAX * volume_[static_cast<int>(_soundType)] * soundPer;
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

void SoundManager::SetSystemVolume(const int _volumePercent, const int _type)
{    
    constexpr int VOLUME_MAX = 255;  //最大音量

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
        int soundPal = VOLUME_MAX * volume_[_type] * soundPer;

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
