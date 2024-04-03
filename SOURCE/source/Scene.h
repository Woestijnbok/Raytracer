#pragma once
#include <vector>
#include "Math.h"
#include "DataTypes.h"
#include "Camera.h"

namespace dae
{
	// Forward Declerations
	class Timer;
	class Material;

	class Scene
	{
		public:
			Scene();
			virtual ~Scene();

			Scene(const Scene&) = delete;
			Scene(Scene&&) noexcept = delete;
			Scene& operator=(const Scene&) = delete;
			Scene& operator=(Scene&&) noexcept = delete;

			virtual void Initialize() = 0;
			virtual void Update(Timer* pTimer);
			void GetClosestHit(const Ray& ray, HitRecord& closestHit) const;
			bool DoesHit(const Ray& ray) const;
			Camera& GetCamera();
			const std::vector<Light>& GetLights() const;
			const std::vector<const Material*>& GetMaterials() const;

		protected:
			Camera m_Camera;
			std::vector<Light> m_Lights;
			std::vector<const Material*> m_Materials;
			std::vector<Plane> m_Planes;
			std::vector<Sphere> m_Spheres;
			std::vector<Triangle> m_Triangles;
			std::vector<TriangleMesh> m_TriangleMeshes;

			void AddPointLight(const Vector3& origin, float intensity, const ColorRGB& color);
			void AddDirectionalLight(const Vector3& direction, float intensity, const ColorRGB& color);
			unsigned char AddMaterial(Material* material);
			void AddPlane(const Vector3& origin, const Vector3& normal, unsigned char materialIndex = 0);
			void AddSphere(const Vector3& origin, float radius, unsigned char materialIndex = 0);
			void AddTriangleMesh(TriangleCullMode cullMode, unsigned char materialIndex = 0);

		private:
			
	};

	class Scene_W1 final : public Scene
	{
		public:
			Scene_W1() = default;
			~Scene_W1() override = default;

			Scene_W1(const Scene_W1&) = delete;
			Scene_W1(Scene_W1&&) noexcept = delete;
			Scene_W1& operator=(const Scene_W1&) = delete;
			Scene_W1& operator=(Scene_W1&&) noexcept = delete;

			void Initialize() override;

		private:

	};

	class Scene_W2 final : public Scene
	{
		public:
			Scene_W2() = default;
			~Scene_W2() override = default;

			Scene_W2(const Scene_W2&) = delete;
			Scene_W2(Scene_W2&&) noexcept = delete;
			Scene_W2& operator=(const Scene_W2&) = delete;
			Scene_W2& operator=(Scene_W2&&) noexcept = delete;

			void Initialize() override;

		private:

	};

	class Scene_W3 final : public Scene
	{
		public:
			Scene_W3() = default;
			~Scene_W3() override = default;

			Scene_W3(const Scene_W3&) = delete;
			Scene_W3(Scene_W3&&) noexcept = delete;
			Scene_W3& operator=(const Scene_W3&) = delete;
			Scene_W3& operator=(Scene_W3&&) noexcept = delete;

			void Initialize() override;

		private:

	};

	class Scene_W4_TestScene final : public Scene
	{
		public:
			Scene_W4_TestScene() = default;
			~Scene_W4_TestScene() override = default;

			Scene_W4_TestScene(const Scene_W4_TestScene&) = delete;
			Scene_W4_TestScene(Scene_W4_TestScene&&) noexcept = delete;
			Scene_W4_TestScene& operator=(const Scene_W4_TestScene&) = delete;
			Scene_W4_TestScene& operator=(Scene_W4_TestScene&&) noexcept = delete;

			virtual void Initialize() override;
			virtual void Update(Timer* pTimer) override;
		
		private:
			
	};

	class Scene_W4_ReferenceScene final : public Scene
	{
		public:
			Scene_W4_ReferenceScene() = default;
			~Scene_W4_ReferenceScene() override = default;

			Scene_W4_ReferenceScene(const Scene_W4_ReferenceScene&) = delete;
			Scene_W4_ReferenceScene(Scene_W4_ReferenceScene&&) noexcept = delete;
			Scene_W4_ReferenceScene& operator=(const Scene_W4_ReferenceScene&) = delete;
			Scene_W4_ReferenceScene& operator=(Scene_W4_ReferenceScene&&) noexcept = delete;

			virtual void Initialize() override;
			virtual void Update(Timer* pTimer) override;

		private:

	};

	class Scene_W4_BunnyScene final : public Scene
	{
		public:
			Scene_W4_BunnyScene() = default;
			~Scene_W4_BunnyScene() override = default;

			Scene_W4_BunnyScene(const Scene_W4_BunnyScene&) = delete;
			Scene_W4_BunnyScene(Scene_W4_BunnyScene&&) noexcept = delete;
			Scene_W4_BunnyScene& operator=(const Scene_W4_BunnyScene&) = delete;
			Scene_W4_BunnyScene& operator=(Scene_W4_BunnyScene&&) noexcept = delete;

			virtual void Initialize() override;
			virtual void Update(Timer* pTimer) override;

		private:

	};

	class Scene_W4_ExtraScene final : public Scene
	{
	public:
		Scene_W4_ExtraScene() = default;
		~Scene_W4_ExtraScene() override = default;

		Scene_W4_ExtraScene(const Scene_W4_ExtraScene&) = delete;
		Scene_W4_ExtraScene(Scene_W4_ExtraScene&&) noexcept = delete;
		Scene_W4_ExtraScene& operator=(const Scene_W4_ExtraScene&) = delete;
		Scene_W4_ExtraScene& operator=(Scene_W4_ExtraScene&&) noexcept = delete;

		virtual void Initialize() override;
		virtual void Update(Timer* pTimer) override;
	private:

	};
}
