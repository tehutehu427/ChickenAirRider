#pragma once
#include <vector>
#include <algorithm>

// テンプレート専用ユーティリティクラス
class UtilityTemplates
{
public:
    
    /// @brief 配列内に値があるか調べる
    /// @tparam T 配列内の要素の型
    /// @param vec 検索対象の配列
    /// @param value 検索する値
    /// @return 値が見つかればtrue,なければfalseを返す
    template<typename T>
    static bool ContainsValue(const std::vector<T>& vec, const T& value) 
    {
        return std::find(vec.begin(), vec.end(), value) != vec.end();
    }
};