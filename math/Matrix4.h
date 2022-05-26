#pragma once
#include <math.h>
/// <summary>
/// 行列
/// </summary>

class Matrix4 {
  private:
	// 行x列
	float m[4][4];

	void MatNorm() {
		for (int y = 0; y < 4; y++) {
			for (size_t x = 0; x < 4; x++) {
				if (x == y) {
					m[y][x] = 1.0f;
				} else {
					m[y][x] = 0;
				}
			}
		}
	}

  public:
	// コンストラクタ
	Matrix4();
	// 成分を指定しての生成
	Matrix4(
	  float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
	  float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);

	// 代入演算子オーバーロード
	Matrix4& operator*=(const Matrix4& m2);

	void GetScaleMat(Vector3 scale) {
		MatNorm();
		m[0][0] = scale.x;
		m[1][1] = scale.y;
		m[2][2] = scale.z;
	}

	void GetRotMat(Vector3 rot) {
		Matrix4 matRot, rotX, rotY, rotZ;

		rotX = Matrix4(
		  1.0f, 0, 0, 0, 0, cos(rot.x), sin(rot.x), 0, 0, -sin(rot.x), cos(rot.x), 0, 0, 0, 0,
		  1.0f);

		rotY = Matrix4(
		  cos(rot.y), 0, -sin(rot.y), 0, 0, 1.0f, 0, 0, sin(rot.y), 0, cos(rot.y), 0, 0, 0, 0,
		  1.0f);

		rotZ = Matrix4(
		  cos(rot.z), sin(rot.z), 0, 0, -sin(rot.z), cos(rot.z), 0, 0, 0, 0, 1.0f, 0, 0, 0, 0,
		  1.0f);

		rotZ *= rotX;
		rotZ *= rotY;

		for (int y = 0; y < 4; y++) {
			for (size_t x = 0; x < 4; x++) {
				m[y][x] = rotZ.m[y][x];
			}
		}
	}

		void GetTransMat(Vector3 trans) {
		MatNorm();
		m[3][0] = trans.x;
		m[3][1] = trans.y;
		m[3][2] = trans.z;
	}
};