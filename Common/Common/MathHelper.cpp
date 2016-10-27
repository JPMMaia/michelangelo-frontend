#include "MathHelper.h"

using namespace DirectX;
using namespace GraphicsEngine;

void MathHelper::CalculateTangentBinormal(const XMFLOAT3& coord0, const XMFLOAT3& coord1, const XMFLOAT3& coord2, const XMFLOAT2& texCoord0, const XMFLOAT2 texCoord1, const XMFLOAT2 texCoord2, XMFLOAT3& tangent, XMFLOAT3& binormal)
{
	//
	// [ Tx Ty Tz ]     [ deltaU0 deltaV0 ] [ edge0.x edge0.y edge0.z ]
	// [          ]  =  [                 ] [                         ]
	// [ Bx By Bz ]     [ deltaU1 deltaV1 ] [ edge1.x edge1.y edge1.z ]
	//

	// Set texture edge matrix:
	auto textureEdgeMatrix = XMMatrixSet(
		texCoord1.x - texCoord0.x, texCoord1.y - texCoord0.y, 0.0f, 0.0f,
		texCoord2.x - texCoord0.x, texCoord2.y - texCoord0.y, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// Calculate inverse of texture edge matrix:
	auto determinant = XMMatrixDeterminant(textureEdgeMatrix);
	auto textureEdgeInverseMatrix = XMMatrixInverse(&determinant, textureEdgeMatrix);

	// Set edge matrix:
	auto edgeMatrix = DirectX::XMMatrixSet(
		coord1.x - coord0.x, coord1.y - coord0.y, coord1.z - coord0.z, 0.0f,
		coord2.x - coord0.x, coord2.y - coord0.y, coord2.z - coord0.z, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// Calculate the tangent and binormal matrix:
	DirectX::XMFLOAT3X3 tangentBinormalMatrix;
	XMStoreFloat3x3(&tangentBinormalMatrix, XMMatrixMultiply(textureEdgeInverseMatrix, edgeMatrix));

	tangent = DirectX::XMFLOAT3(tangentBinormalMatrix(0, 0), tangentBinormalMatrix(0, 1), tangentBinormalMatrix(0, 2));
	binormal = DirectX::XMFLOAT3(tangentBinormalMatrix(1, 0), tangentBinormalMatrix(1, 1), tangentBinormalMatrix(1, 2));
}
