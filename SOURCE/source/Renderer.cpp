#include <SDL.h>
#include <SDL_surface.h>
#include <execution>
#include "Renderer.h"
#include "Utils.h"

#define PARALLEL_EXECUTION

using namespace dae;

Renderer::Renderer(SDL_Window* pWindow) :
	m_pWindow{ pWindow },
	m_pBuffer{ SDL_GetWindowSurface(pWindow) },
	m_pBufferPixels{ static_cast<uint32_t*>(m_pBuffer->pixels) },
	m_Width{ 0 },
	m_Height{ 0 },
	m_CurrentLightingMode{ LightingMode::Combined },
	m_ShadowsEnabled{ true },
	m_pScene{ nullptr },
	m_Camera{ nullptr },
	m_Materials{ nullptr },
	m_Lights{ nullptr },
	m_NrOfPixels{ uint32_t(0) },
	m_FieldOfVieuw{ 0.0f },
	m_AscpectRatio{ 0.0f },
	m_Pixelindices{}
{
	SDL_GetWindowSize(pWindow, &m_Width, &m_Height);
	m_NrOfPixels = uint32_t(m_Width * m_Height);
	m_AscpectRatio = float(m_Width) / float(m_Height);
	m_Pixelindices.reserve(m_NrOfPixels);
	for (uint32_t index{ uint32_t(0) }; index < m_NrOfPixels; index++) m_Pixelindices.emplace_back(index);
}

void Renderer::SetScene(Scene* pScene)
{
	m_pScene = pScene;
	m_Camera = &pScene->GetCamera();
	m_Materials = &pScene->GetMaterials();
	m_Lights = &pScene->GetLights();
	m_FieldOfVieuw = tanf((dae::TO_RADIANS * m_Camera->fovAngle) / 2);
}

void Renderer::Render() const
{
	m_Camera->CalculateCameraToWorld();

	#if defined(PARALLEL_EXECUTION)
		std::for_each(std::execution::par, m_Pixelindices.begin(), m_Pixelindices.end(),
			[&](int i)
			{
				RenderPixel(i);
			}
		);
	#else
		for (uint32_t pixelIndex{}; pixelIndex < m_NrOfPixels; pixelIndex++)
		{
			RenderPixel(pixelIndex);
		}
	#endif

	SDL_UpdateWindowSurface(m_pWindow);
}

bool Renderer::SaveBufferToImage() const
{
	return SDL_SaveBMP(m_pBuffer, "RayTracing_Buffer.bmp");
}

void Renderer::CycleLigtingMode()
{
	m_CurrentLightingMode = LightingMode((int(m_CurrentLightingMode) + 1) % 4);

	switch (m_CurrentLightingMode)
	{
		case LightingMode::ObservedArea:
			std::cout << "Current lighting mode: Observed area / Lambert's cosine law" << std::endl;
			break;
		case LightingMode::Radiance:
			std::cout << "Current lighting mode: Radiance" << std::endl;
			break;
		case LightingMode::BDRF:
			std::cout << "Current lighting mode: BDRF" << std::endl;
			break;
		case LightingMode::Combined:
			std::cout << "Current lighting mode: Combined" << std::endl;
			break;
	}
}

void Renderer::ToggleShadows()
{
	m_ShadowsEnabled = !m_ShadowsEnabled;
}

float Renderer::LambertsCosineLaw(const dae::Vector3& normalSurface, const dae::Vector3& incomingLight, float incomingLightMagnitude) const
{
	return (Vector3::Dot(normalSurface, incomingLight) / incomingLightMagnitude);
}

void Renderer::RenderPixel(uint32_t pixelIndex) const
{
	const uint32_t px{ pixelIndex % m_Width };
	const uint32_t py{ pixelIndex / m_Width };
	float rx{ px + 0.5f }, ry{ py + 0.5f };
	float worldX{ (2 * (rx / float(m_Width)) - 1) * m_AscpectRatio * m_FieldOfVieuw };
	float worldY{ (1 - (2 * (ry / float(m_Height)))) * m_FieldOfVieuw };

	Vector3 cameraRayDirection{ m_Camera->cameraToWorld.TransformVector(Vector3{ worldX, worldY, 1.0f }.Normalized()) };
	Ray cameraRay{ m_Camera->origin, cameraRayDirection };

	ColorRGB color{ colors::Black };
	HitRecord closestHit{};

	m_pScene->GetClosestHit(cameraRay, closestHit);

	if (closestHit.didHit)
	{
		for (const Light& light : *m_Lights)
		{
			const Vector3 lightRayOrigin{ closestHit.origin };
			const Vector3 lightRayDirection{ LightUtils::GetDirectionToLight(light, closestHit.origin) };
			const float lightRayDirectionMagnitude{ lightRayDirection.Magnitude() };
			Ray lightRay{ lightRayOrigin, lightRayDirection / lightRayDirectionMagnitude };
			lightRay.min = 0.01f;
			lightRay.max = lightRayDirectionMagnitude;

			const float observedArea{ LambertsCosineLaw(closestHit.normal, lightRayDirection, lightRayDirectionMagnitude) };

			if (m_ShadowsEnabled && m_pScene->DoesHit(lightRay))
			{
				color *= 0.5f;
			}
			else
			{
				switch (m_CurrentLightingMode)
				{
				case dae::Renderer::LightingMode::ObservedArea:
					if (observedArea > 0.0f)
					{
						color += colors::White * observedArea;
					}
					break;
				case dae::Renderer::LightingMode::Radiance:
					color += LightUtils::GetRadiance(light, closestHit.origin);
					break;
				case dae::Renderer::LightingMode::BDRF:
					color += m_Materials->at(closestHit.materialIndex)->Shade(closestHit, lightRay.direction, -cameraRayDirection);
					break;
				case dae::Renderer::LightingMode::Combined:
					if (observedArea > 0.0f)
					{
						color += LightUtils::GetRadiance(light, closestHit.origin) *
							m_Materials->at(closestHit.materialIndex)->Shade(closestHit, lightRay.direction, -cameraRayDirection) *
							observedArea;
					}
					break;
				}
			}

		}
	}

	color.MaxToOne();
	m_pBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBuffer->format, static_cast<uint8_t>(color.r * 255), static_cast<uint8_t>(color.g * 255), static_cast<uint8_t>(color.b * 255));
}