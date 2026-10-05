#pragma once

#include"Geometry.h"

class Model : public Geometry
{
public:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_movedPos">追従する親の移動後座標</param>
	/// <param name="_rot">追従する親の回転</param>
	/// <param name="_modelId">追従する親のモデルID</param>
	Model(const VECTOR& _pos, const VECTOR& _movedPos, const Quaternion& _rot, const int _modelId);

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="_copyBase">コピー元</param>
	Model(const Model& _copyBase);

	//デストラクタ
	~Model(void)override;

	//描画
	void Draw(const int _color = NORMAL_COLOR)override;

	//各種当たり判定
	const bool IsHit(Geometry& _geometry)override;
	const bool IsHit(Model& _model)override;
	const bool IsHit(Cube& _cube)override;
	const bool IsHit(Sphere& _sphere)override;
	const bool IsHit(Capsule& _capsule)override;
	const bool IsHit(Line& _line) override;

	//ヒット後の処理
	void HitAfter(void)override;

	//親モデルIDの取得
	inline const int GetParentModel(void)const { return parentModelId_; };

	//当たった時の情報取得
	inline const MV1_COLL_RESULT_POLY& GetHitLineInfo(void)const { return hitLineInfo_; }

	//当たった時の情報取得
	inline const MV1_COLL_RESULT_POLY_DIM& GetHitInfo(void)const { return hitInfo_; }

	//当たった時の情報設定
	inline void SetHitLineInfo(const MV1_COLL_RESULT_POLY _hitInfo) { hitLineInfo_ = _hitInfo; }

	//当たった時の情報設定
	inline void SetHitInfo(MV1_COLL_RESULT_POLY_DIM _hitInfo) { std::swap(hitInfo_, _hitInfo); }

private:

	//親のモデルID
	int parentModelId_;					
	
	//当たった時の情報(線)
	MV1_COLL_RESULT_POLY hitLineInfo_;	

	//当たった時の情報(球、カプセル)
	MV1_COLL_RESULT_POLY_DIM hitInfo_;
};
