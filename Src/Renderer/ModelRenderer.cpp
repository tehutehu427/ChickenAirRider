#include"../pch.h"
#include "ModelRenderer.h"

ModelRenderer::ModelRenderer(void)
{
}

ModelRenderer::~ModelRenderer(void)
{
}

void ModelRenderer::Draw(const int _modelId, const ModelMaterial& _modelMaterial)
{
	// オリジナルシェーダ設定(ON)
	MV1SetUseOrigShader(true);

	// シェーダ設定(頂点)
	SetReserveVS(_modelMaterial);

	// シェーダ設定(ピクセル)
	SetReservePS(_modelMaterial);

	// テクスチャアドレスタイプの取得
	auto texA = _modelMaterial.GetTextureAddress();
	int texAType = static_cast<int>(texA);

	// テクスチャアドレスタイプを変更
	SetTextureAddressModeUV(texAType, texAType);

	// 描画
	MV1DrawModel(_modelId);

	// テクスチャアドレスタイプを元に戻す
	SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);

	// 後始末
	//-----------------------------------------

	// テクスチャ解除
	const auto& textures = _modelMaterial.GetTextures();
	size_t size = textures.size();
	if (size == 0)
	{
		// 前回使用分のテクスチャを引き継がないように
		SetUseTextureToShader(0, -1);
	}
	else
	{
		for (const auto& pair : textures)
		{
			SetUseTextureToShader(pair.first, -1);
		}
	}

	// 頂点シェーダ解除
	SetUseVertexShader(-1);

	// ピクセルシェーダ解除
	SetUsePixelShader(-1);

	// オリジナルシェーダ設定(OFF)
	MV1SetUseOrigShader(false);
	//-----------------------------------------

}

void ModelRenderer::DrawMeshes(const int _modelId, const ModelMaterial& _modelMaterial)
{
	// オリジナルシェーダ設定(ON)
	MV1SetUseOrigShader(true);

	// シェーダ設定(頂点)
	SetReserveVS(_modelMaterial);

	//ピクセルシェーダの設定
	// --------------------------------------------------------------
	// 定数バッファハンドル
	int constBuf = _modelMaterial.GetConstBufPS();

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = _modelMaterial.GetConstBufsPS();

	size_t size = constBufs.size();
	for (int i = 0; i < size; i++)
	{
		if (i != 0)
		{
			constBufsPtr++;
		}
		constBufsPtr->x = constBufs[i].x;
		constBufsPtr->y = constBufs[i].y;
		constBufsPtr->z = constBufs[i].z;
		constBufsPtr->w = constBufs[i].w;
	}

	// ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(constBuf);

	// ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
	SetShaderConstantBuffer(
		constBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	// ピクセルシェーダー設定
	SetUsePixelShader(_modelMaterial.GetShaderPS());
	// --------------------------------------------------------------

	// テクスチャアドレスタイプの取得
	auto texA = _modelMaterial.GetTextureAddress();
	int texAType = static_cast<int>(texA);

	// テクスチャアドレスタイプを変更
	SetTextureAddressModeUV(texAType, texAType);

	int meshNum = MV1GetMeshNum(_modelId);
	for (int i = 0; i < meshNum; ++i)
	{
		//マテリアル番号取得
		int materialIndex = MV1GetMeshMaterial(_modelId, i);

		// テクスチャ取得
		int texHandle = MV1GetTextureGraphHandle(_modelId, materialIndex);

		//テクスチャの設定
		SetUseTextureToShader(0, texHandle);

		// 対象メッシュを描画（自作描画処理でバッファを設定して DrawPrimitive などを呼ぶ）
		MV1DrawMesh(_modelId, i);
	}

	// テクスチャアドレスタイプを元に戻す
	SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);

	// 後始末
	//-----------------------------------------

	// テクスチャ解除
	const auto& textures = _modelMaterial.GetTextures();
	size = textures.size();
	if (size == 0)
	{
		// 前回使用分のテクスチャを引き継がないように
		SetUseTextureToShader(0, -1);
	}
	else
	{
		for (const auto& pair : textures)
		{
			SetUseTextureToShader(pair.first, -1);
		}
	}

	// 頂点シェーダ解除
	SetUseVertexShader(-1);

	// ピクセルシェーダ解除
	SetUsePixelShader(-1);

	// オリジナルシェーダ設定(OFF)
	MV1SetUseOrigShader(false);
	//-----------------------------------------

}

void ModelRenderer::SetReserveVS(const ModelMaterial& _modelMaterial)
{
	// 定数バッファハンドル
	int constBuf = _modelMaterial.GetConstBufVS();

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = _modelMaterial.GetConstBufsVS();

	size_t size = constBufs.size();
	for (int i = 0; i < size; i++)
	{
		if (i != 0)
		{
			constBufsPtr++;
		}
		constBufsPtr->x = constBufs[i].x;
		constBufsPtr->y = constBufs[i].y;
		constBufsPtr->z = constBufs[i].z;
		constBufsPtr->w = constBufs[i].w;
	}

	// 頂点シェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(constBuf);

	// 頂点シェーダー用の定数バッファを定数バッファレジスタにセット
	SetShaderConstantBuffer(
		constBuf, DX_SHADERTYPE_VERTEX, CONSTANT_BUF_SLOT_BEGIN_VS);

	// 頂点シェーダー設定
	SetUseVertexShader(_modelMaterial.GetShaderVS());

}

void ModelRenderer::SetReservePS(const ModelMaterial& _modelMaterial)
{

	// ピクセルシェーダーにテクスチャを転送
	const auto& textures = _modelMaterial.GetTextures();
	size_t size = textures.size();
	if (size == 0)
	{
		// 前回使用分のテクスチャを引き継がないように
		SetUseTextureToShader(0, -1);
	}
	else
	{
		for (const auto& pair : textures)
		{
			SetUseTextureToShader(pair.first, pair.second);
		}
	}

	// 定数バッファハンドル
	int constBuf = _modelMaterial.GetConstBufPS();

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = _modelMaterial.GetConstBufsPS();

	size = constBufs.size();
	for (int i = 0; i < size; i++)
	{
		if (i != 0)
		{
			constBufsPtr++;
		}
		constBufsPtr->x = constBufs[i].x;
		constBufsPtr->y = constBufs[i].y;
		constBufsPtr->z = constBufs[i].z;
		constBufsPtr->w = constBufs[i].w;
	}

	// ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(constBuf);

	// ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
	SetShaderConstantBuffer(
		constBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	// ピクセルシェーダー設定
	SetUsePixelShader(_modelMaterial.GetShaderPS());

}
