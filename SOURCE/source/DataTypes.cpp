// External includes

// Project includes
#include "DataTypes.h"

namespace dae
{
	Triangle::Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2) :
		v0{ v0 },
		v1{ v1 },
		v2{ v2 },
		normal{ Vector3::Cross((v1 - v0), (v2 - v0)).Normalized() },
		cullMode{ TriangleCullMode::FrontFaceCulling },
		materialIndex{ 0 }
	{

	}

	Triangle::Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& normal) :
		v0{ v0 },
		v1{ v1 },
		v2{ v2 },
		normal{ normal.Normalized() },
		cullMode{ TriangleCullMode::FrontFaceCulling },
		materialIndex{ 0 }
	{

	}

	TriangleMesh::TriangleMesh() :
		positions{},
		normals{},
		indices{},
		materialIndex{},
		cullMode{ TriangleCullMode::BackFaceCulling },
		rotationTransform{},
		translationTransform{},
		scaleTransform{},
		transformedPositions{},
		transformedNormals{},
		minAABB{},
		maxAABB{},
		transformedMinAABB{},
		transformedMaxAABB{}
	{

	}

	TriangleMesh::TriangleMesh(const std::vector<Vector3>& positions, const std::vector<int>& indices, TriangleCullMode cullMode) :
		positions{ positions },
		normals{},
		indices{ indices },
		materialIndex{},
		cullMode{ cullMode },
		rotationTransform{},
		translationTransform{},
		scaleTransform{},
		transformedPositions{},
		transformedNormals{},
		minAABB{},
		maxAABB{},
		transformedMinAABB{},
		transformedMaxAABB{}
	{
		CalculateNormals();
		UpdateAABB();
		UpdateTransforms();
	}

	TriangleMesh::TriangleMesh(const std::vector<Vector3>& positions, const std::vector<int>& indices, const std::vector<Vector3>& normals, TriangleCullMode cullMode) :
		positions{ positions },
		normals{ normals },
		indices{ indices },
		materialIndex{},
		cullMode{ cullMode },
		rotationTransform{},
		translationTransform{},
		scaleTransform{},
		transformedPositions{},
		transformedNormals{},
		minAABB{},
		maxAABB{},
		transformedMinAABB{},
		transformedMaxAABB{}
	{
		UpdateAABB();
		UpdateTransforms();
	}

	void TriangleMesh::Translate(const Vector3& translation)
	{
		translationTransform = Matrix::CreateTranslation(translation);
	}

	void TriangleMesh::RotateY(float yaw)
	{
		rotationTransform = Matrix::CreateRotationY(yaw);
	}

	void TriangleMesh::Scale(const Vector3& scale)
	{
		scaleTransform = Matrix::CreateScale(scale);
	}

	void TriangleMesh::AppendTriangle(const Triangle& triangle, bool ignoreTransformUpdate)
	{
		int startIndex = static_cast<int>(positions.size());

		positions.push_back(triangle.v0);
		positions.push_back(triangle.v1);
		positions.push_back(triangle.v2);

		indices.push_back(startIndex);
		indices.push_back(++startIndex);
		indices.push_back(++startIndex);

		normals.push_back(triangle.normal);

		//Not ideal, but making sure all vertices are updated
		if (!ignoreTransformUpdate)
			UpdateTransforms();
	}

	void TriangleMesh::CalculateNormals()
	{
		for (int index{}; index < indices.size(); index += 3)
		{
			const Vector3 vector1{ positions[index + 1] - positions[index] };
			const Vector3 vector2{ positions[index + 2] - positions[index] };
			const Vector3 normal{ Vector3::Cross(vector1, vector2).Normalized() };

			normals.push_back(normal);
		}
	}

	void TriangleMesh::UpdateTransforms()
	{
		if (positions.size() != transformedPositions.size() || normals.size() != transformedNormals.size())
		{
			transformedPositions = positions;
			transformedNormals = normals;
		}

		const Matrix finalTransform{ scaleTransform * rotationTransform * translationTransform };

		for (int index{ 0 }; index < positions.size(); index++)
		{
			transformedPositions[index] = finalTransform.TransformPoint(positions[index]);
		}

		for (int index{ 0 }; index < normals.size(); index++)
		{
			transformedNormals[index] = finalTransform.TransformVector(normals[index]);
		}

		UpdateTranformedAABB(finalTransform);
	}

	void TriangleMesh::UpdateAABB()
	{
		if (positions.size() > 0)
		{
			minAABB = positions[0];
			maxAABB = positions[0];

			for (const Vector3& position : positions)
			{
				minAABB = Vector3::Min(minAABB, position);
				maxAABB = Vector3::Max(maxAABB, position);
			}
		}
	}

	void TriangleMesh::UpdateTranformedAABB(const Matrix& finalTransform)
	{
		// min min min
		Vector3 tMinAABB{ finalTransform.TransformPoint(minAABB) };
		Vector3 tMaxAABB{ tMinAABB };

		// min min max
		Vector3 tAABB{ finalTransform.TransformPoint(minAABB.x, minAABB.y, maxAABB.z) };
		tMinAABB = Vector3::Min(tMinAABB, tAABB);
		tMaxAABB = Vector3::Max(tMaxAABB, tAABB);

		// min max min
		tAABB = finalTransform.TransformPoint(minAABB.x, maxAABB.y, minAABB.z);
		tMinAABB = Vector3::Min(tMinAABB, tAABB);
		tMaxAABB = Vector3::Max(tMaxAABB, tAABB);

		// max min min
		tAABB = finalTransform.TransformPoint(maxAABB.x, minAABB.y, minAABB.z);
		tMinAABB = Vector3::Min(tMinAABB, tAABB);
		tMaxAABB = Vector3::Max(tMaxAABB, tAABB);

		// max max max
		tAABB = finalTransform.TransformPoint(maxAABB);
		tMinAABB = Vector3::Min(tMinAABB, tAABB);
		tMaxAABB = Vector3::Max(tMaxAABB, tAABB);

		// max max min
		tAABB = finalTransform.TransformPoint(maxAABB.x, maxAABB.y, minAABB.z);
		tMinAABB = Vector3::Min(tMinAABB, tAABB);
		tMaxAABB = Vector3::Max(tMaxAABB, tAABB);

		// max min max
		tAABB = finalTransform.TransformPoint(maxAABB.x, minAABB.y, maxAABB.z);
		tMinAABB = Vector3::Min(tMinAABB, tAABB);
		tMaxAABB = Vector3::Max(tMaxAABB, tAABB);

		// min max max
		tAABB = finalTransform.TransformPoint(minAABB.x, maxAABB.y, maxAABB.z);
		tMinAABB = Vector3::Min(tMinAABB, tAABB);
		tMaxAABB = Vector3::Max(tMaxAABB, tAABB);

		transformedMinAABB = tMinAABB;
		transformedMaxAABB = tMaxAABB;
	}

	Ray::Ray() :
		origin{ Vector3::Zero },
		direction{ Vector3::Zero },
		min{ 0.0001f },
		max{ FLT_MAX }
	{

	}

	Ray::Ray(const Vector3& origin, const Vector3& direction) :
		origin{ origin },
		direction{ direction },
		min{ 0.0001f },
		max{ FLT_MAX }
	{

	}

	HitRecord::HitRecord() :
		origin{ Vector3::Zero },
		normal{ Vector3::Zero },
		t{ FLT_MAX },
		didHit{ false },
		materialIndex{ 0 }
	{

	}
}