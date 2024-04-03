#pragma once
#include <cassert>
#include <fstream>
#include "Math.h"
#include "DataTypes.h"

namespace dae
{
	namespace GeometryUtils
	{
		inline bool HitTest_Sphere(const Sphere& sphere, const Ray& ray, HitRecord& hitRecord)
		{
			const float A = Vector3::Dot(ray.direction, ray.direction);
			const float B = Vector3::Dot(2 * ray.direction, ray.origin - sphere.origin);
			const float C = Vector3::Dot(ray.origin - sphere.origin, ray.origin - sphere.origin) - std::powf(sphere.radius, 2);

			const float discriminant{ powf(B, 2) - 4 * A * C };

			if (discriminant > 0)
			{
				// calculate t, the lenght between the origin of the ray and the intersection of the sphere
				const float sqrtDiscriminant{ sqrtf(discriminant) };

				float t{ (-B - sqrtDiscriminant) / (2 * A) };

				if (t < ray.min)
				{
					t = (-B + sqrtDiscriminant) / (2 * A);
				}

				if (t >= ray.min && t <= ray.max)
				{
					hitRecord.origin = ray.origin + (t * ray.direction);
					hitRecord.normal = (hitRecord.origin - sphere.origin) / sphere.radius;
					hitRecord.t = t;
					hitRecord.didHit = true;
					hitRecord.materialIndex = sphere.materialIndex;

					return true;
				}
			}

			return false;
		}

		inline bool HitTest_Sphere(const Sphere& sphere, const Ray& ray)
		{
			const float A = Vector3::Dot(ray.direction, ray.direction);
			const float B = Vector3::Dot(2 * ray.direction, ray.origin - sphere.origin);
			const float C = Vector3::Dot(ray.origin - sphere.origin, ray.origin - sphere.origin) - std::powf(sphere.radius, 2.0f);

			const float discriminant{ powf(B, 2.0f) - 4 * A * C };

			if (discriminant > 0)
			{
				// calculate t, the lenght between the origin of the ray and the intersection of the sphere
				const float sqrtDiscriminant{ sqrtf(discriminant) };

				float t{ (-B - sqrtDiscriminant) / (2 * A) };

				if (t < ray.min)
				{
					t = (-B + sqrtDiscriminant) / (2 * A);
				}

				if (t >= ray.min && t <= ray.max)
				{
					return true;
				}
			}

			return false;
		}

		inline bool HitTest_Plane(const Plane& plane, const Ray& ray, HitRecord& hitRecord)
		{
			const float t = (Vector3::Dot((plane.origin - ray.origin), plane.normal)) / (Vector3::Dot(ray.direction, plane.normal));

			if (t >= ray.min && t <= ray.max)
			{
				hitRecord.origin = ray.origin + (t * ray.direction);
				hitRecord.normal = plane.normal;
				hitRecord.t = t;
				hitRecord.didHit = true;
				hitRecord.materialIndex = plane.materialIndex;

				return true;
			}

			return false;
		}

		inline bool HitTest_Plane(const Plane& plane, const Ray& ray)
		{
			const float t = (Vector3::Dot((plane.origin - ray.origin), plane.normal)) / (Vector3::Dot(ray.direction, plane.normal));

			if (t >= ray.min && t <= ray.max) return true;

			return false;
		}

		inline bool SlabTest_TriangleMesh(const TriangleMesh& mesh, const Ray& ray)
		{
			const float tx1{ (mesh.transformedMinAABB.x - ray.origin.x) / ray.direction.x };
			const float tx2{ (mesh.transformedMaxAABB.x - ray.origin.x) / ray.direction.x };

			float tmin{ std::min(tx1, tx2) };
			float tmax{ std::max(tx1, tx2) };

			const float ty1{ (mesh.transformedMinAABB.y - ray.origin.y) / ray.direction.y };
			const float ty2{ (mesh.transformedMaxAABB.y - ray.origin.y) / ray.direction.y };

			tmin = std::max(tmin, std::min(ty1, ty2));
			tmax = std::min(tmax, std::max(ty1, ty2));

			const float tz1{ (mesh.transformedMinAABB.z - ray.origin.z) / ray.direction.z };
			const float tz2{ (mesh.transformedMaxAABB.z - ray.origin.z) / ray.direction.z };

			tmin = std::max(tmin, std::min(tz1, tz2));
			tmax = std::min(tmax, std::max(tz1, tz2));

			return (tmax > 0) && (tmax >= tmin);
		}

		inline bool HitTest_Triangle(const Triangle& triangle, const Ray& ray, HitRecord& hitRecord)
		{
			const Vector3 a{ triangle.v1 - triangle.v0 };
			const Vector3 b{ triangle.v2 - triangle.v0 };
			const Vector3 n{ Vector3::Cross(a, b).Normalized() };

			switch (triangle.cullMode)
			{
				case TriangleCullMode::NoCulling:
					if (AreEqual(Vector3::Dot(n, ray.direction), 0.0f)) return false;
					break;
				case TriangleCullMode::FrontFaceCulling:
					if (Vector3::Dot(n, ray.direction) < 0.0f) return false;
					break;
				case TriangleCullMode::BackFaceCulling:
					if (Vector3::Dot(n, ray.direction) > 0.0f) return false;
					break;
			}

			const Vector3 L{ triangle.v0 - ray.origin };
			const float t{ Vector3::Dot(L, n) / Vector3::Dot(ray.direction, n) };

			if (t < ray.min || t > ray.max) return false;

			const Vector3 P{ ray.origin + (ray.direction * t) };

			const Vector3 e0{ triangle.v1 - triangle.v0 };
			const Vector3 e1{ triangle.v2 - triangle.v1 };
			const Vector3 e2{ triangle.v0 - triangle.v2 };

			const Vector3 p0{ P - triangle.v0 };
			const Vector3 p1{ P - triangle.v1 };
			const Vector3 p2{ P - triangle.v2 };

			if(Vector3::Dot(Vector3::Cross(e0, p0), n) < 0.0f) return false;
			if(Vector3::Dot(Vector3::Cross(e1, p1), n) < 0.0f) return false;
			if(Vector3::Dot(Vector3::Cross(e2, p2), n) < 0.0f) return false;

			hitRecord.origin = P;
			hitRecord.normal = triangle.normal;
			hitRecord.t = t;
			hitRecord.didHit = true;
			hitRecord.materialIndex = triangle.materialIndex;

			return true;
		}

		inline bool HitTest_Triangle(const Triangle& triangle, const Ray& ray)
		{
			const Vector3 a{ triangle.v1 - triangle.v0 };
			const Vector3 b{ triangle.v2 - triangle.v0 };
			const Vector3 n{ Vector3::Cross(a, b).Normalized() };

			switch (triangle.cullMode)
			{
				case TriangleCullMode::NoCulling:
					if (AreEqual(Vector3::Dot(n, ray.direction), 0.0f)) return false;
					break;
				case TriangleCullMode::FrontFaceCulling:
					if (Vector3::Dot(n, ray.direction) > 0.0f) return false;
					break;
				case TriangleCullMode::BackFaceCulling:
					if (Vector3::Dot(n, ray.direction) < 0.0f) return false;
					break;
			}

			const Vector3 L{ triangle.v0 - ray.origin };
			const float t{ Vector3::Dot(L, n) / Vector3::Dot(ray.direction, n) };

			if (t < ray.min || t > ray.max) return false;

			const Vector3 P{ ray.origin + (ray.direction * t) };

			const Vector3 e0{ triangle.v1 - triangle.v0 };
			const Vector3 e1{ triangle.v2 - triangle.v1 };
			const Vector3 e2{ triangle.v0 - triangle.v2 };

			const Vector3 p0{ P - triangle.v0 };
			const Vector3 p1{ P - triangle.v1 };
			const Vector3 p2{ P - triangle.v2 };

			if (Vector3::Dot(Vector3::Cross(e0, p0), n) < 0.0f) return false;
			if (Vector3::Dot(Vector3::Cross(e1, p1), n) < 0.0f) return false;
			if (Vector3::Dot(Vector3::Cross(e2, p2), n) < 0.0f) return false;

			return true;
		}

		inline bool HitTest_TriangleMesh(const TriangleMesh& mesh, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
		{
			if (!SlabTest_TriangleMesh(mesh, ray)) return false;

			HitRecord closestHit{};

			for (int index{}; index < mesh.indices.size(); index += 3)
			{
				Triangle triangle{ mesh.transformedPositions[mesh.indices[index]],
					mesh.transformedPositions[mesh.indices[index + 1]],
					mesh.transformedPositions[mesh.indices[index + 2]],
					mesh.transformedNormals[index / 3] };

				triangle.cullMode = mesh.cullMode;
				triangle.materialIndex = mesh.materialIndex;

				HitTest_Triangle(triangle, ray, hitRecord);
				if (hitRecord.t < closestHit.t) closestHit = hitRecord;
			}

			hitRecord = closestHit;

			return hitRecord.didHit;
		}

		inline bool HitTest_TriangleMesh(const TriangleMesh& mesh, const Ray& ray)
		{
			HitRecord temp{};
			return HitTest_TriangleMesh(mesh, ray, temp, true);
		}
	}

	namespace LightUtils
	{
		inline Vector3 GetDirectionToLight(const Light& light, const Vector3 origin)
		{
			switch (light.type)
			{
			case LightType::Directional:
				return Vector3::Zero;
				break;
			case LightType::Point:
				return Vector3{ light.origin - origin };
				break;
			}

			return Vector3::Zero;
		}

		inline ColorRGB GetRadiance(const Light& light, const Vector3& target)
		{
			switch (light.type)
			{
			case LightType::Directional:
				return light.color * light.intensity;
				break;
			case LightType::Point:
				return light.color * (light.intensity / (light.origin - target).SqrMagnitude());
				break;
			}

			return colors::Black;
		}
	}

	namespace Utils
	{
		#pragma warning(push)
		#pragma warning(disable : 4505) //Warning unreferenced local function
		static bool ParseOBJ(const std::string& filename, std::vector<Vector3>& positions, std::vector<Vector3>& normals, std::vector<int>& indices)
		{
			std::ifstream file(filename);
			if (!file)
				return false;

			std::string sCommand;
			// start a while iteration ending when the end of file is reached (ios::eof)
			while (!file.eof())
			{
				//read the first word of the string, use the >> operator (istream::operator>>) 
				file >> sCommand;
				//use conditional statements to process the different commands	
				if (sCommand == "#")
				{
					// Ignore Comment
				}
				else if (sCommand == "v")
				{
					//Vertex
					float x, y, z;
					file >> x >> y >> z;
					positions.push_back({ x, y, z });
				}
				else if (sCommand == "f")
				{
					float i0, i1, i2;
					file >> i0 >> i1 >> i2;

					indices.push_back((int)i0 - 1);
					indices.push_back((int)i1 - 1);
					indices.push_back((int)i2 - 1);
				}
				//read till end of line and ignore all remaining chars
				file.ignore(1000, '\n');

				if (file.eof())
					break;
			}

			//Precompute normals
			for (uint64_t index = 0; index < indices.size(); index += 3)
			{
				uint32_t i0 = indices[index];
				uint32_t i1 = indices[index + 1];
				uint32_t i2 = indices[index + 2];

				Vector3 edgeV0V1 = positions[i1] - positions[i0];
				Vector3 edgeV0V2 = positions[i2] - positions[i0];
				Vector3 normal = Vector3::Cross(edgeV0V1, edgeV0V2);

				if (isnan(normal.x))
				{
					int k = 0;
				}

				normal.Normalize();
				if (isnan(normal.x))
				{
					int k = 0;
				}

				normals.push_back(normal);
			}

			return true;
		}
		#pragma warning(pop)
	}
}