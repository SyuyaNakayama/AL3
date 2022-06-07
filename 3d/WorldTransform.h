#pragma once

#include "Matrix4.h"
#include "Vector3.h"
#include <d3d12.h>
#include <wrl.h>

// 定数バッファ用データ構造体
struct ConstBufferDataWorldTransform {
	Matrix4 matWorld; // ローカル → ワールド変換行列
};

/// <summary>
/// ワールド変換データ
/// </summary>
struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataWorldTransform* constMap = nullptr;
	// ローカルスケール
	Vector3 scale_ = {1, 1, 1};
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = {0, 0, 0};
	// ローカル座標
	Vector3 translation_ = {0, 0, 0};
	// ローカル → ワールド変換行列
	Matrix4 matWorld_;
	// 親となるワールド変換へのポインタ
	WorldTransform* parent_ = nullptr;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();

	void UpdateMatrix() {
		Matrix4 matScale, matRot, matTrans;

		matScale.GetScaleMat(scale_);
		matRot.GetRotMat(rotation_);
		matTrans.GetTransMat(translation_);

		matScale *= matRot;
		matScale *= matTrans;
		matWorld_ = matScale;
	}

	Vector3 MatrixProduct(const Vector3& v) {
		Vector3 temp(v);
		Vector3 ans(0, 0, 0);

		ans.x = temp.x * m[0][0] + temp.y * m[1][0] + temp.z * m[2][0];
		ans.y = temp.x * m[0][1] + temp.y * m[1][1] + temp.z * m[2][1];
		ans.z = temp.x * m[0][2] + temp.y * m[1][2] + temp.z * m[2][2];

		return ans;
	}
};
