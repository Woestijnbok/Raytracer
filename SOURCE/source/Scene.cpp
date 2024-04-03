#include "Scene.h"
#include "Utils.h"
#include "Material.h"

namespace dae
{
	Scene::Scene() :
		m_Camera{},
		m_Lights{},
		m_Materials{},
		m_Planes{},
		m_Spheres{},
		m_Triangles{},
		m_TriangleMeshes{}
	{
		m_Lights.reserve(32);
		m_Materials.reserve(32);
		m_Planes.reserve(32);
		m_Spheres.reserve(32);
		m_Triangles.reserve(32);
		m_TriangleMeshes.reserve(32);

		m_Materials.emplace_back(new Material_SolidColor{ ColorRGB{1.0f, 0.0f, 0.0f} });
	}

	Scene::~Scene()
	{
		for (const Material* pMaterial : m_Materials)
		{
			delete pMaterial;
		}

		m_Materials.clear();
	}

	void Scene::Update(Timer* pTimer)
	{
		m_Camera.Update(pTimer);
	}

	void Scene::GetClosestHit(const Ray& ray, HitRecord& closestHit) const
	{
		HitRecord hitRecord{};

		for (const Sphere& sphere : m_Spheres)
		{
			dae::GeometryUtils::HitTest_Sphere(sphere, ray, hitRecord);

			if (hitRecord.t < closestHit.t) closestHit = hitRecord;
		}

		for (const Plane& plane : m_Planes)
		{
			dae::GeometryUtils::HitTest_Plane(plane, ray, hitRecord);

			if (hitRecord.t < closestHit.t) closestHit = hitRecord;
		}

		for (const Triangle& triangle : m_Triangles)
		{
			dae::GeometryUtils::HitTest_Triangle(triangle, ray, hitRecord);

			if (hitRecord.t < closestHit.t) closestHit = hitRecord;
		}

		for (const TriangleMesh& mesh : m_TriangleMeshes)
		{
			dae::GeometryUtils::HitTest_TriangleMesh(mesh, ray, hitRecord);

			if (hitRecord.t < closestHit.t) closestHit = hitRecord;
		}
	}

	bool Scene::DoesHit(const Ray& ray) const
	{
		for (const Sphere& sphere : m_Spheres)
		{
			if (dae::GeometryUtils::HitTest_Sphere(sphere, ray)) return true;
		}

		for (const Plane& plane : m_Planes)
		{
			if (dae::GeometryUtils::HitTest_Plane(plane, ray)) return true;
		}

		for (const Triangle& triangle : m_Triangles)
		{
			if (dae::GeometryUtils::HitTest_Triangle(triangle, ray)) return true;
		}

		for (const TriangleMesh& mesh : m_TriangleMeshes)
		{
			if (dae::GeometryUtils::HitTest_TriangleMesh(mesh, ray)) return true;
		}

		return false;
	}

	Camera& Scene::GetCamera()
	{
		return m_Camera;
	}

	const std::vector<Light>& Scene::GetLights() const
	{ 
		return m_Lights;
	}

	const std::vector<const Material*>& Scene::GetMaterials() const
	{ 
		return m_Materials;
	}

	void Scene::AddPointLight(const Vector3& origin, float intensity, const ColorRGB& color)
	{
		m_Lights.emplace_back(Light{ origin, Vector3::Zero, color, intensity, LightType::Point });
	}

	void Scene::AddDirectionalLight(const Vector3& direction, float intensity, const ColorRGB& color)
	{
		m_Lights.emplace_back(Light{ Vector3::Zero, direction, color, intensity, LightType::Directional });
	}

	unsigned char Scene::AddMaterial(Material* material)
	{
		m_Materials.emplace_back(material);
		return static_cast<unsigned char>(m_Materials.size() - 1);
	}

	void Scene::AddPlane(const Vector3& origin, const Vector3& normal, unsigned char materialIndex)
	{
		m_Planes.emplace_back(Plane{ origin, normal, materialIndex });
	}

	void Scene::AddSphere(const Vector3& origin, float radius, unsigned char materialIndex)
	{
		m_Spheres.emplace_back(Sphere{ origin, radius, materialIndex });
	}

	void Scene::AddTriangleMesh(TriangleCullMode cullMode, unsigned char materialIndex)
	{
		TriangleMesh m{};
		m.cullMode = cullMode;
		m.materialIndex = materialIndex;

		m_TriangleMeshes.emplace_back(m);
	}

	void Scene_W1::Initialize()
	{
		// Materials
		constexpr unsigned char matId_Solid_Red = 0;
		const unsigned char matId_Solid_Blue{ AddMaterial(new Material_SolidColor{ colors::Blue }) };
		const unsigned char matId_Solid_Yellow{ AddMaterial(new Material_SolidColor{ colors::Yellow }) };
		const unsigned char matId_Solid_Green{ AddMaterial(new Material_SolidColor{ colors::Green }) };
		const unsigned char matId_Solid_Magenta{ AddMaterial(new Material_SolidColor{ colors::Magenta }) };

		//Spheres
		AddSphere(Vector3{ -25.0f, 0.0f, 100.0f }, 50.0f, matId_Solid_Red);
		AddSphere(Vector3{ 25.0f, 0.0f, 100.0f }, 50.0f, matId_Solid_Blue);

		//Plane
		AddPlane(Vector3{ -75.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 0.0f, 0.0f }, matId_Solid_Green);
		AddPlane(Vector3{ 75.0f, 0.0f, 0.0f }, Vector3{ -1.0f, 0.0f, 0.0f }, matId_Solid_Green);
		AddPlane(Vector3{ 0.0f, -75.0f, 0.0f }, Vector3{ 0.0f, 1.0f, 0.0f }, matId_Solid_Yellow);
		AddPlane(Vector3{ 0.0f, 75.0f, 0.0f }, Vector3{ 0.0f, -1.0f, 0.0f }, matId_Solid_Yellow);
		AddPlane(Vector3{ 0.0f, 0.0f, 125.0f }, Vector3{ 0.0f, 0.0f,-1.0f }, matId_Solid_Magenta);
	}

	void Scene_W2::Initialize()
	{
		// Camera Settings
		m_Camera.origin = Vector3{ 0.0f, 3.0f, -9.0f };
		m_Camera.fovAngle = 45.0f;

		// Material id's
		constexpr unsigned char matId_Solid_Red = 0;
		const unsigned char matId_Solid_Blue{ AddMaterial(new Material_SolidColor{ colors::Blue }) };
		const unsigned char matId_Solid_Yellow{ AddMaterial(new Material_SolidColor{ colors::Yellow }) };
		const unsigned char matId_Solid_Green{ AddMaterial(new Material_SolidColor{ colors::Green }) };
		const unsigned char matId_Solid_Magenta{ AddMaterial(new Material_SolidColor{ colors::Magenta }) };

		// Planes
		AddPlane(Vector3{ -5.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 0.0f, 0.0f }, matId_Solid_Green);
		AddPlane(Vector3{ 5.0f, 0.0f, 0.0f }, Vector3{ -1.0f, 0.0f, 0.0f }, matId_Solid_Green);
		AddPlane(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 0.0f, 1.0f, 0.0f }, matId_Solid_Yellow);
		AddPlane(Vector3{ 0.0f, 10.0f, 0.0f }, Vector3{ 0.0f, -1.0f, 0.0f }, matId_Solid_Yellow);
		AddPlane(Vector3{ 0.0f, 0.0f, 10.0f }, Vector3{ .0f, 0.0f, -1.0f }, matId_Solid_Magenta);

		// Spheres
		AddSphere(Vector3{ -1.75f, 1.0f, 0.0f }, 0.75f, matId_Solid_Red);
		AddSphere(Vector3{ 0.0f, 1.0f, 0.0f }, 0.75f, matId_Solid_Blue);
		AddSphere(Vector3{ 1.75f, 1.0f, 0.0f }, 0.75f, matId_Solid_Red);
		AddSphere(Vector3{ -1.75f, 3.0f, 0.0f }, 0.75f, matId_Solid_Blue);
		AddSphere(Vector3{ 0.0f, 3.0f, 0.0f }, 0.75f, matId_Solid_Red);
		AddSphere(Vector3{ 1.75f, 3.0f, 0.0f }, 0.75f, matId_Solid_Blue);

		// Lights
		AddPointLight(Vector3{ 0.0f, 5.0f, -5.0f }, 70.0f, colors::White);
	}

	void Scene_W3::Initialize()
	{
		m_Camera.origin = Vector3{ 0.0f, 3.0f, -9.0f };
		m_Camera.fovAngle = 45.0f;

		const unsigned char matCT_GrayRoughMetal{ AddMaterial(new Material_CookTorrence{ ColorRGB{ 0.972f, 0.960f, 0.915f }, 1.0f, 1.0f }) };
		const unsigned char matCT_GrayMediumMetal{ AddMaterial(new Material_CookTorrence{ ColorRGB{ 0.972f, 0.960f, 0.915f }, 1.0f, 0.6f }) };
		const unsigned char matCT_GraySmoothMetal{ AddMaterial(new Material_CookTorrence{ ColorRGB{ 0.972f, 0.960f, 0.915f }, 1.0f, 0.1f }) };
		const unsigned char matCT_GrayRoughPlastic{ AddMaterial(new Material_CookTorrence{ ColorRGB{ 0.75f, 0.75f, 0.75f }, 0.0f, 1.0f }) };
		const unsigned char matCT_GrayMediumPlastic{ AddMaterial(new Material_CookTorrence{ ColorRGB{ 0.75f, 0.75f, 0.75f }, 0.0f, 0.6f }) };
		const unsigned char matCT_GraySmoothPlastic{ AddMaterial(new Material_CookTorrence{ ColorRGB{ 0.75f, 0.75f, 0.75f }, 0.0f, 0.1f }) };
		const unsigned char matLambert_GrayBlue{ AddMaterial(new Material_Lambert{ ColorRGB{ 0.49f, 0.57f, 0.57f }, 1.0f }) };

		//Plane
		AddPlane(Vector3{ 0.0f, 0.0f, 10.0f }, Vector3{ 0.0f, 0.0f, -1.0f }, matLambert_GrayBlue);; //Back
		AddPlane(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 0.0f, 1.0f, 0.0f }, matLambert_GrayBlue);; //Bottom
		AddPlane(Vector3{ 0.0f, 10.0f, 0.0f }, Vector3{ 0.0f, -1.0f, 0.0f }, matLambert_GrayBlue);; //Top
		AddPlane(Vector3{ 5.0f, 0.0f, 0.0f }, Vector3{ -1.0f, 0.0f, 0.0f }, matLambert_GrayBlue);; //Right
		AddPlane(Vector3{ -5.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 0.0f, 0.0f }, matLambert_GrayBlue);; //Left

		// Todo 7
		/*const auto matLambertPhong1{ AddMaterial(new Material_LambertPhong{ colors::Blue, 0.5f, 0.5f, 3.0f }) };
		const auto matLambertPhong2{ AddMaterial(new Material_LambertPhong{ colors::Blue, 0.5f, 0.5f, 15.0f }) };
		const auto matLambertPhong3{ AddMaterial(new Material_LambertPhong{ colors::Blue, 0.5f, 0.5f, 50.0f }) };
		AddSphere(Vector3{ -1.75, 1.0f, 0.0f }, 0.75f, matLambertPhong1);
		AddSphere(Vector3{ 0.0f, 1.0f, 0.0f }, 0.75f, matLambertPhong2);
		AddSphere(Vector3{ 1.75, 1.0f, 0.0f }, 0.75f, matLambertPhong3);*/

		//Spheres
		AddSphere(Vector3{ -1.75, 1.0f, 0.0f }, 0.75f, matCT_GrayRoughMetal);
		AddSphere(Vector3{ 0.0f, 1.0f, 0.0f }, 0.75f, matCT_GrayMediumMetal);
		AddSphere(Vector3{ 1.75, 1.0f, 0.0f }, 0.75f, matCT_GraySmoothMetal);
		AddSphere(Vector3{ -1.75, 3.0f, 0.0f }, 0.75f, matCT_GrayRoughPlastic);
		AddSphere(Vector3{ 0.0f, 3.0f, 0.0f }, 0.75f, matCT_GrayMediumPlastic);
		AddSphere(Vector3{ 1.75, 3.0f, 0.0f }, 0.75f, matCT_GraySmoothPlastic);

		//Light
		AddPointLight(Vector3{ 0.0f, 5.0f, 5.0f }, 50.0f, ColorRGB{ 1.0f, 0.61f, 0.45f });
		AddPointLight(Vector3{ -2.5f, 5.f, -5.0f }, 70.0f, ColorRGB{ 1.0f, 0.8f, 0.45f });
		AddPointLight(Vector3{ 2.5f, 2.5f, -5.0f }, 50.0f, ColorRGB{ 0.34f, 0.47f, 0.68f });;
	}

	void Scene_W4_TestScene::Initialize()
	{
		m_Camera.origin = { 0.f,1.f,-5.f };
		m_Camera.fovAngle = 45.f;

		//Materials
		const unsigned char matLambert_GrayBlue{ AddMaterial(new Material_Lambert{ ColorRGB{ 0.49f, 0.57f, 0.57f }, 1.0f }) };
		const unsigned char matLambert_White{ AddMaterial(new Material_Lambert{ colors::White, 1.0f }) };

		//Planes
		AddPlane(Vector3{ 0.f, 0.f, 10.f }, Vector3{ 0.f, 0.f, -1.f }, matLambert_GrayBlue); //BACK
		AddPlane(Vector3{ 0.f, 0.f, 0.f }, Vector3{ 0.f, 1.f, 0.f }, matLambert_GrayBlue); //BOTTOM
		AddPlane(Vector3{ 0.f, 10.f, 0.f }, Vector3{ 0.f, -1.f, 0.f }, matLambert_GrayBlue); //TOP
		AddPlane(Vector3{ 5.f, 0.f, 0.f }, Vector3{ -1.f, 0.f, 0.f }, matLambert_GrayBlue); //RIGHT
		AddPlane(Vector3{ -5.f, 0.f, 0.f }, Vector3{ 1.f, 0.f, 0.f }, matLambert_GrayBlue); //LEFT

		// Triangle
		/*Triangle triangle{ Vector3{ -0.75f, 0.5f, 0.0f }, Vector3{ -0.75f, 2.0f, 0.0f }, Vector3{ 0.75f, 0.5f, 0.0f } };
		triangle.cullMode = TriangleCullMode::NoCulling;
		triangle.materialIndex = matLambert_White;
		m_Triangles.emplace_back(triangle);*/

		// Triangle Mesh
		AddTriangleMesh(TriangleCullMode::NoCulling, matLambert_White);

		m_TriangleMeshes[0].positions = {
			Vector3{ -0.75f, -1.0f, 0.0f },  // Vertex 0
			Vector3{ -0.75f, 1.0f, 0.0f },  // Vertex 1
			Vector3{ 0.75f, 1.0f, 1.0f },    // Vertex 2
			Vector3{ 0.75f, -1.0f, 0.0f } }; // Vertex 3

		m_TriangleMeshes[0].indices = {
			0,1,2, // Positions triangle 1
			0,2,3  // Positions triangle 2
		};

		m_TriangleMeshes[0].CalculateNormals();
		m_TriangleMeshes[0].UpdateAABB();
		m_TriangleMeshes[0].Translate({ 0.f,1.5f,0.f });
		m_TriangleMeshes[0].UpdateTransforms();

		//Light
		AddPointLight(Vector3{ 0.0f, 5.0f, 5.0f }, 50.0f, ColorRGB{ 1.0f, 0.61f, 0.45f }); //Backlight
		AddPointLight(Vector3{ -2.5f, 5.f, -5.f }, 70.0f, ColorRGB{ 1.0f, 0.8f, 0.45f }); //Front Light Left
		AddPointLight(Vector3{ 2.5f, 2.5f, -5.f }, 50.0f, ColorRGB{ 0.34f, 0.47f, 0.68f });
	}

	void Scene_W4_TestScene::Update(Timer* pTimer)
	{
		Scene::Update(pTimer);

		for (TriangleMesh& mesh : m_TriangleMeshes)
		{
			mesh.RotateY(PI_DIV_2 * pTimer->GetTotal());
			mesh.UpdateTransforms();
		}
	}

	void Scene_W4_ReferenceScene::Initialize()
	{
		m_Camera.origin = Vector3{ 0.0f, 3.0f, -9.0f };
		m_Camera.fovAngle = 45.0f;

		const unsigned char matCT_GrayRoughMetal{ AddMaterial(new Material_CookTorrence{ ColorRGB{ 0.972f, 0.960f, 0.915f }, 1.0f, 1.0f }) };
		const unsigned char matCT_GrayMediumMetal{ AddMaterial(new Material_CookTorrence{ ColorRGB{ 0.972f, 0.960f, 0.915f }, 1.0f, 0.6f }) };
		const unsigned char matCT_GraySmoothMetal{ AddMaterial(new Material_CookTorrence{ ColorRGB{ 0.972f, 0.960f, 0.915f }, 1.0f, 0.1f }) };
		const unsigned char matCT_GrayRoughPlastic{ AddMaterial(new Material_CookTorrence{ ColorRGB{ 0.75f, 0.75f, 0.75f }, 0.0f, 1.0f }) };
		const unsigned char matCT_GrayMediumPlastic{ AddMaterial(new Material_CookTorrence{ ColorRGB{ 0.75f, 0.75f, 0.75f }, 0.0f, 0.6f }) };
		const unsigned char matCT_GraySmoothPlastic{ AddMaterial(new Material_CookTorrence{ ColorRGB{ 0.75f, 0.75f, 0.75f }, 0.0f, 0.1f }) };
		const unsigned char matLambert_GrayBlue{ AddMaterial(new Material_Lambert{ ColorRGB{ 0.49f, 0.57f, 0.57f }, 1.0f }) };
		const unsigned char matLambert_White{ AddMaterial(new Material_Lambert{ colors::White, 1.0f }) };

		AddPlane(Vector3{ 0.0f, 0.0f, 10.0f }, Vector3{ 0.0f, 0.0f, -1.0f }, matLambert_GrayBlue); //BACK
		AddPlane(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 0.0f, 1.0f, 0.0f }, matLambert_GrayBlue); //BOTTOM
		AddPlane(Vector3{ 0.0f, 10.0f, 0.0f }, Vector3{ 0.0f, -1.0f, 0.0f }, matLambert_GrayBlue); //TOP
		AddPlane(Vector3{ 5.0f, 0.0f, 0.0f }, Vector3{ -1.0f, 0.0f, 0.0f }, matLambert_GrayBlue); //RIGHT
		AddPlane(Vector3{ -5.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 0.0f, 0.0f }, matLambert_GrayBlue); //LEFT

		AddSphere(Vector3{ -1.75f, 1.0f, 0.0f }, 0.75f, matCT_GrayRoughMetal);
		AddSphere(Vector3{ 0.0f, 1.0f, 0.0f }, 0.75f, matCT_GrayMediumMetal);
		AddSphere(Vector3{ 1.75f, 1.0f, 0.0f }, 0.75f, matCT_GraySmoothMetal);
		AddSphere(Vector3{ -1.75f, 3.0f, 0.0f }, 0.75f, matCT_GrayRoughPlastic);
		AddSphere(Vector3{ 0.0f, 3.0f, 0.0f }, 0.75f, matCT_GrayMediumPlastic);
		AddSphere(Vector3{ 1.75f, 3.0f, 0.0f }, 0.75f, matCT_GraySmoothPlastic);

		//CW Winding Order!
		const Triangle baseTriangle{ Vector3{ -0.75f, 1.5f, 0.0f}, Vector3{ 0.75f, 0.0f, 0.0f }, Vector3{ -0.75f, 0.0f, 0.0f} };

		AddTriangleMesh(TriangleCullMode::BackFaceCulling, matLambert_White);
		m_TriangleMeshes[0].AppendTriangle(baseTriangle, true);
		m_TriangleMeshes[0].UpdateAABB();
		m_TriangleMeshes[0].Translate({ -1.75f,4.5f,0.f });
		m_TriangleMeshes[0].UpdateTransforms();

		AddTriangleMesh(TriangleCullMode::FrontFaceCulling, matLambert_White);
		m_TriangleMeshes[1].AppendTriangle(baseTriangle, true);
		m_TriangleMeshes[1].UpdateAABB();
		m_TriangleMeshes[1].Translate({ 0.f,4.5f,0.f });
		m_TriangleMeshes[1].UpdateTransforms();

		AddTriangleMesh(TriangleCullMode::NoCulling, matLambert_White);
		m_TriangleMeshes[2].AppendTriangle(baseTriangle, true);
		m_TriangleMeshes[2].UpdateAABB();
		m_TriangleMeshes[2].Translate({ 1.75f,4.5f,0.f });
		m_TriangleMeshes[2].UpdateTransforms();

		AddPointLight(Vector3{ 0.0f, 5.0f, 5.0f }, 50.0f, ColorRGB{ 1.0f, 0.61f, 0.45f }); //Backlight
		AddPointLight(Vector3{ -2.5f, 5.0f, -5.0f }, 70.0f, ColorRGB{ 1.0f, 0.8f, 0.45f }); //Front Light Left
		AddPointLight(Vector3{ 2.5f, 2.5f, -5.0f }, 50.0f, ColorRGB{ 0.34f, 0.47f, 0.68f });
	}

	void Scene_W4_ReferenceScene::Update(Timer* pTimer)
	{
		Scene::Update(pTimer);

		for (TriangleMesh& mesh : m_TriangleMeshes)
		{
			mesh.RotateY(PI_DIV_2 * pTimer->GetTotal());
			mesh.UpdateTransforms();
		}
	}

	void Scene_W4_BunnyScene::Initialize()
	{
		m_Camera.origin = Vector3{ 0.f,1.f,-5.f };
		m_Camera.fovAngle = 45.0f;

		// Materials
		const unsigned char matLambert_GrayBlue{ AddMaterial(new Material_Lambert{ ColorRGB{ 0.49f, 0.57f, 0.57f }, 1.0f }) };
		const unsigned char matLambert_White{ AddMaterial(new Material_Lambert{ colors::White, 1.0f }) };

		// Planes
		AddPlane(Vector3{ 0.0f, 0.0f, 10.0f }, Vector3{ 0.0f, 0.0f, -1.0f }, matLambert_GrayBlue); //BACK
		AddPlane(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 0.0f, 1.0f, 0.0f }, matLambert_GrayBlue); //BOTTOM
		AddPlane(Vector3{ 0.0f, 10.0f, 0.0f }, Vector3{ 0.0f, -1.0f, 0.0f }, matLambert_GrayBlue); //TOP
		AddPlane(Vector3{ 5.0f, 0.0f, 0.0f }, Vector3{ -1.0f, 0.0f, 0.0f }, matLambert_GrayBlue); //RIGHT
		AddPlane(Vector3{ -5.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 0.0f, 0.0f }, matLambert_GrayBlue); //LEFT

		// Mesh
		AddTriangleMesh(TriangleCullMode::BackFaceCulling, matLambert_White);
		Utils::ParseOBJ("Resources/bunny.obj", m_TriangleMeshes[0].positions, m_TriangleMeshes[0].normals, m_TriangleMeshes[0].indices);
		m_TriangleMeshes[0].Scale(Vector3{ 1.5f, 1.5f, 1.5f });
		m_TriangleMeshes[0].UpdateAABB();
		m_TriangleMeshes[0].UpdateTransforms();

		// Lights
		AddPointLight(Vector3{ 0.0f, 5.0f, 5.0f }, 50.0f, ColorRGB{ 1.0f, 0.61f, 0.45f }); //Backlight
		AddPointLight(Vector3{ -2.5f, 5.0f, -5.0f }, 70.0f, ColorRGB{ 1.0f, 0.8f, 0.45f }); //Front Light Left
		AddPointLight(Vector3{ 2.5f, 2.5f, -5.0f }, 50.0f, ColorRGB{ 0.34f, 0.47f, 0.68f });
	}

	void Scene_W4_BunnyScene::Update(Timer* pTimer)
	{
		Scene::Update(pTimer);

		for (TriangleMesh& mesh : m_TriangleMeshes)
		{
			mesh.RotateY(PI_DIV_2 * pTimer->GetTotal());
			mesh.UpdateTransforms();
		}
	}

	void Scene_W4_ExtraScene::Initialize()
	{
		m_Camera.origin = Vector3{ 0.0f, 3.0f, -8.0f };
		m_Camera.fovAngle = 45.0f;

		// Materials
		const unsigned char matCT_GrayMediumMetal{ AddMaterial(new Material_CookTorrence{ ColorRGB{ 0.972f, 0.960f, 0.915f }, 1.0f, 0.6f }) };
		const unsigned char matCT_GraySmoothMetal{ AddMaterial(new Material_CookTorrence{ ColorRGB{ 0.972f, 0.960f, 0.915f }, 1.0f, 0.1f }) };
		const unsigned char matCT_GraySmoothPlastic{ AddMaterial(new Material_CookTorrence{ ColorRGB{ 0.75f, 0.75f, 0.75f }, 0.0f, 0.1f }) };

		const unsigned char matLambert_GrayBlue{ AddMaterial(new Material_Lambert{ ColorRGB{ 0.49f, 0.57f, 0.57f }, 1.0f }) };
		const unsigned char matLambert_Red{ AddMaterial(new Material_Lambert{ ColorRGB{ 1.0f, 0.0f, 0.0f }, 1.0f }) };
		const unsigned char matLambert_Green{ AddMaterial(new Material_Lambert{ ColorRGB{ 0.0f, 1.0f, 0.0f }, 1.0f }) };
		const unsigned char matLambert_Blue{ AddMaterial(new Material_Lambert{ ColorRGB{ 0.0f, 0.0f, 1.0f }, 1.0f }) };
		const unsigned char matLambert_Phong{ AddMaterial(new Material_LambertPhong{ ColorRGB{ 1.0f, 1.0f, 1.0f }, 0.7f, 0.8f, 0.7f }) };
		const unsigned char matLambert_Phong2{ AddMaterial(new Material_LambertPhong{ ColorRGB{ 1.0f, 1.0f, 1.0f }, 0.3f, 0.2f, 0.3f }) };
		

		// Planes
		AddPlane(Vector3{ 0.0f, 0.0f, 10.0f }, Vector3{ 0.0f, 0.0f, -1.0f }, matLambert_Green); //BACK
		AddPlane(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 0.0f, 1.0f, 0.0f }, matLambert_Phong); //BOTTOM
		AddPlane(Vector3{ 0.0f, 10.0f, 0.0f }, Vector3{ 0.0f, -1.0f, 0.0f }, matLambert_Phong2); //TOP
		AddPlane(Vector3{ 5.0f, 0.0f, 0.0f }, Vector3{ -1.0f, 0.0f, 0.0f }, matLambert_Blue); //RIGHT
		AddPlane(Vector3{ -5.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 0.0f, 0.0f }, matLambert_Red); //LEFT

		// Mesh
		AddSphere(Vector3{ 0.0f, 3.0f, 0.0f }, 0.5f, matCT_GraySmoothMetal);
		AddSphere(Vector3{ 0.0f, 3.0f, 0.0f }, 0.5f, matCT_GrayMediumMetal);

		// Lights
		AddPointLight(Vector3{ 0.0f, 5.0f, 5.0f }, 50.0f, ColorRGB{ 1.0f, 0.61f, 0.45f }); //Backlight
		AddPointLight(Vector3{ -2.5f, 5.0f, -5.0f }, 70.0f, ColorRGB{ 1.0f, 0.8f, 0.45f }); //Front Light Left
		AddPointLight(Vector3{ 2.5f, 2.5f, -5.0f }, 50.0f, ColorRGB{ 0.34f, 0.47f, 0.68f });
	}

	void Scene_W4_ExtraScene::Update(Timer* pTimer)
	{
		Scene::Update(pTimer);

		// First sphere which changes radius
		float sine{ sinf(pTimer->GetTotal()) };
		if (sine < 0.0f) sine *= -1.0f;
		m_Spheres[0].radius = sine;

		// Second sphere which rotates around the first sphere
		const float radius{ 2.0f };
		const float x{ m_Spheres[0].origin.x + (radius * cosf(pTimer->GetTotal())) };
		const float y{ m_Spheres[0].origin.y + (radius * sinf(pTimer->GetTotal())) };
		m_Spheres[1].origin = Vector3{ x, y, m_Spheres[0].origin.z };
	}
}