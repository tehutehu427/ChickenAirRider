#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "../Common/IntVector3.h"
#include "../Common/Quaternion.h"
#include "../Object/Common/Transform.h"
class Utility
{

public:

	enum class STRING_PLACE	//文字列を揃える位置
	{
		LEFT,		//左揃え
		CENTER,		//中央揃え
		RIGHT,		//右揃え
	};

	// ラジアン(rad)・度(deg)変換用
	static constexpr float RAD2DEG = (180.0f / DX_PI_F);
	static constexpr float DEG2RAD = (DX_PI_F / 180.0f);

	static constexpr VECTOR VECTOR_ZERO = { 0.0f, 0.0f, 0.0f };
	static constexpr VECTOR VECTOR_ONE = { 1.0f, 1.0f, 1.0f };

	// 回転軸
	static constexpr VECTOR AXIS_X = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR AXIS_Y = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR AXIS_Z = { 0.0f, 0.0f, 1.0f };
	static constexpr VECTOR AXIS_XZ = { 1.0f, 0.0f, 1.0f };
	static constexpr VECTOR AXIS_XY = { 1.0f, 1.0f, 1.0f };
	static constexpr VECTOR AXIS_YZ = { 0.0f, 1.0f, 1.0f };
	static constexpr VECTOR AXIS_XYZ = { 1.0f, 1.0f, 1.0f };

	// 方向
	static constexpr VECTOR DIR_F = { 0.0f, 0.0f, 1.0f };
	static constexpr VECTOR DIR_B = { 0.0f, 0.0f, -1.0f };
	static constexpr VECTOR DIR_R = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_L = { -1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_U = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR DIR_D = { 0.0f, -1.0f, 0.0f };

	//カラーコード
	static constexpr int BLACK = 0x000000;	//黒
	static constexpr int RED = 0xFF0000;	//赤
	static constexpr int BLUE = 0x0000FF;	//青
	static constexpr int YELLOW = 0xFFFF00;	//黄色
	static constexpr int GREEN = 0x008000;	//緑
	static constexpr int CYAN = 0x00FFFF;	//水色
	static constexpr int PINK = 0xFFC0CB;	//桃色
	static constexpr int ORANGE = 0xFFA500;	//オレンジ
	static constexpr int LIME = 0xADFF2F;	//黄緑
	static constexpr int PURPLE = 0x800080;	//紫
	static constexpr int WHITE = 0xFFFFFF;	//白
	static constexpr int GRAY = 0xBBBBBB;	//灰色
	static constexpr int BROWN = 0x8B4513;	//茶色

	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	//値を反転させる溜めのスケール
	static constexpr float REVERSE_SCALE = -1.0f;

	//アルファ最大値
	static constexpr int ALPHA_MAX = 255;

	// 四捨五入
	static int Round(float v);

	// 文字列の分割
	static std::vector <std::string> Split(std::string& line, char delimiter);

	// ラジアン(rad)から度(deg)
	static double Rad2DegD(double rad);
	static float Rad2DegF(float rad);
	static int Rad2DegI(int rad);

	// 度(deg)からラジアン(rad)
	static double Deg2RadD(double deg);
	static float Deg2RadF(float deg);
	static int Deg2RadI(int deg);

	// 0～360度の範囲に収める
	static double DegIn360(double deg);

	// 0(0)～2π(360度)の範囲に収める
	static double RadIn2PI(double rad);

	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundRad(float from, float to);
	
	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundDeg(float from, float to);
	
	// 線形補間
	static int Lerp(int start, int end, float t);
	static float Lerp(float start, float end, float t);
	static double Lerp(double start, double end, double t);
	static Vector2 Lerp(const Vector2& start, const Vector2& end, float t);
	static VECTOR Lerp(const VECTOR& start, const VECTOR& end, float t);

	// 角度の線形補間
	static double LerpDeg(double start, double end, double t);

	// 色の線形補間
	static COLOR_F Lerp(const COLOR_F& start, const COLOR_F& end, float t);

	// ベジェ曲線
	static Vector2 Bezier(const Vector2& p1, const Vector2& p2, const Vector2& p3, float t);
	static VECTOR Bezier(const VECTOR& p1, const VECTOR& p2, const VECTOR& p3, float t);
	
	// Y軸回転
	static VECTOR RotXZPos(const VECTOR& centerPos, const VECTOR& radiusPos, float rad);

	// ベクトルの長さ
	static double Magnitude(const Vector2& v);
	static double Magnitude(const VECTOR& v);
	static float MagnitudeF(const VECTOR& v);
	static int SqrMagnitude(const Vector2& v);
	static float SqrMagnitudeF(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v1, const VECTOR& v2);
	static double Distance(const Vector2& v1, const Vector2& v2);
	static double Distance(const VECTOR& v1, const VECTOR& v2);

	// 比較
	static bool Equals(const VECTOR& v1, const VECTOR& v2);
	static bool EqualsVZero(const VECTOR& v1);

	// 正規化
	static VECTOR Normalize(const Vector2& v);
	static VECTOR VNormalize(const VECTOR& v);

	// 2つのベクトルの間の角度
	static double AngleDeg(const VECTOR& from, const VECTOR& to);

	// 描画系
	static void DrawLineDir(const VECTOR& pos, const VECTOR& dir, int color, float len = 50.0f);
	static void DrawLineXYZ(const VECTOR& pos, const MATRIX& rot, float len = 50.0f);
	static void DrawLineXYZ(const VECTOR& pos, const Quaternion& rot, float len = 50.0f);

	//待機時間
	static bool IsTimeOver(float& totalTime, const float& waitTime);

	/// @brief 文字列を指定位置に揃えて描画
	/// @param _str 描画する文字列
	/// @param _line 揃えるX座標　左揃え：左の座標　中央揃え：中央の座標　右揃え：右の座標
	/// @param _posY 描画するY座標
	/// @param _color 描画する色コード
	/// @param _place 揃える位置
	static void DrawStringPlace(std::wstring _str, int _line, int _posY, int _color, STRING_PLACE _place);

	/// @brief とある点からとある点までの移動ベクトルを返す
	/// @param _start 狙う側
	/// @param _goal 向かう先
	/// @param _speed 設定速度(未設定だと、方向ベクトルのみを返す)
	/// @return 向かう先までの移動ベクトル
	static const VECTOR GetMoveVec(const VECTOR _start, const VECTOR _goal, const float _speed = 1.0f);

	/// @brief 対象に向かう回転軸
	/// @param _pos 自身の座標
	/// @param _targetPos 対象の座標
	/// @param _needAxis 必要な回転軸
	/// @return 回転軸
	static const VECTOR GetRotAxisToTarget(const VECTOR _pos, const VECTOR _targetPos, const VECTOR _needAxis = AXIS_XYZ);

	/// @brief 対象に向く
	/// @param _trans 回転させるモデルの情報
	/// @param _toTargetAxis 対象までの回転軸
	/// @param _time 補正完了までの全体時間(指定がないなら補正なし)
	/// @param _relativePos モデル自体の相対座標
	static void LookAtTarget(Transform& _trans, const VECTOR _toTargetAxis, const float _time = 0.0f, const VECTOR _relativePos = VECTOR_ZERO);

	/// @brief 特定の範囲内に座標が含まれてるか調べる
	/// @param _pos 調べたい座標
	/// @param _leftTop 範囲左上
	/// @param _rightBotm 範囲右下
	/// @return 判定結果を返す
	static bool IsPointInRect(const Vector2 _pos, const Vector2 _leftTop, const Vector2 _rightBotm);

	/// <summary>
	/// 特定の円の範囲内に座標が含まれてるか調べる
	/// </summary>
	/// <param name="_pos">調べる座標</param>
	/// <param name="_circlePos">円の座標</param>
	/// <param name="_radius">半径</param>
	/// <returns></returns>

	/// @brief 特定の円の範囲内に座標が含まれてるか調べる
	/// @param _pos 調べる座標
	/// @param _circlePos 円の座標
	/// @param _radius 半径
	/// @return true:含まれていた
	static bool IsPointInRectCircle(const Vector2 _pos, const Vector2 _circlePos, const float _radius);

	/// @brief スクリーン座標からワールド座標に
	/// @param screenPos スクリーン座標
	/// @param distance 距離
	/// @param cameraPos カメラ位置
	/// @param cameraDir カメラ方向
	/// @return ワールド座標
	static VECTOR GetWorldPosAtScreen(const Vector2 screenPos, const float distance, const VECTOR cameraPos, const VECTOR cameraDir);

	//符号を返す
	static int GetSign(float f);
	static int GetSign(int f);

	//エクスプローラーからファイルを開く
	static std::string OpenFileDialog();

	//数値の反転
	static VECTOR ReverseValue(const VECTOR _vec);
	static  int ReverseValue(const int _i);
	static float ReverseValue(const float _f);
	static IntVector3 ReverseValue(const IntVector3 _iv);

	/// @brief 範囲外の値を指定した範囲に収める
	/// @param index 収めたい値
	/// @param max 範囲の最大値
	/// @return 0からN-1の範囲に補正した値
	static int WrapIndex(int index, int max);

	/// @brief 点線を描画する
	/// @param sPos 始点
	/// @param ePos 終点
	/// @param color 色
	/// @param len 間隔
	static void DrawPointLine3D(const VECTOR sPos, const VECTOR ePos, int color, float len = 50.0f);

	/// @brief イーズイン
	/// @param _time 経過時間
	/// @param _totalTime アニメーション総時間
	/// @param _start 開始位置
	/// @param _end 終了位置
	/// @return 変化量
	static float EaseInQuad(float _time,const float _totalTime, const float _start, const float _end);

	/// @brief イーズアウト
	/// @param _time 経過時間
	/// @param _totalTime アニメーション総時間
	/// @param _start 開始位置
	/// @param _end 終了位置
	/// @return 変化量
	static float EaseOutQuad(float _time, const float _totalTime, const float _start, const float _end);

	/// @brief イーズインアウトバック
	/// @param _time 経過時間
	/// @param _totalTime アニメーション総時間
	/// @param _start 開始位置
	/// @param _end 終了位置
	/// @return 変化量
	static float EaseInOutBack(float _time, const float _totalTime, const float _start, const float _end);

	static constexpr float DEFAULT_SHAKE_SPEED = 5.0f;		//揺れの振幅
	static constexpr float DEFAULT_SHAKE_AMPLITUDE = 10.0f;	//揺れの速さ

	/// @brief 揺らす計算した値を返す
	/// @param _value 揺らしたい値
	/// @param _time 時間
	/// @param _speed 速度
	/// @param _amplitude 振幅
	/// @return 揺らす計算した値
	static float GetShake(const float _value, float _time, const float _speed = DEFAULT_SHAKE_SPEED, const float _amplitude = DEFAULT_SHAKE_AMPLITUDE);

	/// @brief 指定した桁数の数字を返す
	/// @param _value 桁値を求める値
	/// @param _digit 桁数
	/// @return 桁値
	static int GetDigit(const int _value, const int _digit);

	/// @brief 桁数を返す
	/// @param _value 桁数を調べる値
	/// @return 桁数
	static int GetDigitCount(const int _value);

	/// @brief JSON保存ダイアログを表示し、パスをUTF-8文字列で返す
	/// @return UTF-8文字列で書いたパス
	static std::string ShowSaveJsonDialog();

	//wstring(Unicode文字列)をstring(UTF-8)に変換する
	static std::string WStrToStr(const std::wstring& wstr);
	
	//string(UTF-8)をwstring(Unicode文字列)に変換する
	static std::wstring StrToWStr(const std::string& str);

	/// @brief 値を上下に動かす処理
	/// @param _value 現在の値
	/// @param _step 増減量
	/// @param _max 最大値
	/// @param _min 最小値
	/// @param _dir 増加方向（参照）1なら増加中、-1なら減少中
	/// @return 処理後の値
	static float PingPongUpdate(const float _value, const float _step, const float _max, const float _min, int& _dir);
};
