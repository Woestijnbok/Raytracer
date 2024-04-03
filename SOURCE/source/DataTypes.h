#pragma once

// External includes
#include <vector>

// Project includes
#include "Math.h"

namespace dae
{
	struct Sphere
	{
		Vector3 origin;
		float radius;
		unsigned char materialIndex;
	};

	struct Plane
	{
		Vector3 origin;
		Vector3 normal;
		unsigned char materialIndex;
	};

	enum class TriangleCullMode
	{
		FrontFaceCulling,
		BackFaceCulling,
		NoCulling
	};

	struct Triangle
	{
		Triangle() = default;
		Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& normal);
		Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2);

		Vector3 v0;
		Vector3 v1;
		Vector3 v2;
		Vector3 normal;
		TriangleCullMode cullMode;
		unsigned char materialIndex;
	};

	struct TriangleMesh
	{
		TriangleMesh();
		TriangleMesh(const std::vector<Vector3>& _positions, const std::vector<int>& _indices, TriangleCullMode _cullMode);
		TriangleMesh(const std::vector<Vector3>& positions, const std::vector<int>& indices, const std::vector<Vector3>& normals, TriangleCullMode cullMode);

		void Translate(const Vector3& translation);
		void RotateY(float yaw);
		void Scale(const Vector3& scale);
		void AppendTriangle(const Triangle& triangle, bool ignoreTransformUpdate = false);
		void CalculateNormals();
		void UpdateTransforms();
		void UpdateAABB();
		void UpdateTranformedAABB(const Matrix& finalTransform);

		std::vector<Vector3> positions;
		std::vector<Vector3> normals;
		std::vector<int> indices;
		unsigned char materialIndex;
		TriangleCullMode cullMode;
		Matrix rotationTransform;
		Matrix translationTransform;
		Matrix scaleTransform;
		std::vector<Vector3> transformedPositions;
		std::vector<Vector3> transformedNormals;
		Vector3 minAABB;
		Vector3 maxAABB;
		Vector3 transformedMinAABB;
		Vector3 transformedMaxAABB;
	};

	enum class LightType
	{
		Point,
		Directional
	};

	struct Light
	{
		Vector3 origin;
		Vector3 direction;
		ColorRGB color;
		float intensity;
		LightType type;
	};

	struct Ray
	{
		Ray();
		Ray(const Vector3& origin, const Vector3& direction);

		Vector3 origin;
		Vector3 direction;
		float min;
		float max;
	};

	struct HitRecord
	{
		HitRecord();

		Vector3 origin;
		Vector3 normal;
		float t;
		bool didHit;
		unsigned char materialIndex;
	};
}