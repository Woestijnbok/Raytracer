#pragma once
#include <cstdint>
#include "Math.h"
#include "Material.h"
#include "Scene.h"
#include "DataTypes.h"
#include "Camera.h"

struct SDL_Window;
struct SDL_Surface;

namespace dae
{
	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer() = default;

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void SetScene(Scene* pScene);
		void Render() const;
		bool SaveBufferToImage() const;
		void CycleLigtingMode();
		void ToggleShadows();

	private:
		enum class LightingMode
		{
			ObservedArea,
			Radiance,
			BDRF,
			Combined
		};

		SDL_Window* m_pWindow;
		SDL_Surface* m_pBuffer;
		uint32_t* m_pBufferPixels; 
		int m_Width; 
		int m_Height;
		LightingMode m_CurrentLightingMode;
		bool m_ShadowsEnabled;
		const Scene* m_pScene;
		Camera* m_Camera;
		const std::vector<const Material*>* m_Materials;
		const std::vector<Light>* m_Lights;
		uint32_t m_NrOfPixels;
		float m_FieldOfVieuw;
		float m_AscpectRatio;
		std::vector<uint32_t> m_Pixelindices;

		float LambertsCosineLaw(const Vector3& normalSurface, const Vector3& incomingLight, float incomingLightMagnitude) const;
		void RenderPixel(uint32_t pixelIndex) const;
	};
}
