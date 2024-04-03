#pragma once
#include <cassert>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>
#include <iostream>

#pragma once
#include "Math.h"
#include "Timer.h"

namespace dae
{
	struct Camera final
	{
		Camera();
		Camera(const Vector3& origin, float fovAngle);
		~Camera() = default;

		Camera(const Camera&) = delete;
		Camera(Camera&&) noexcept = delete;
		Camera& operator=(const Camera&) = delete;
		Camera& operator=(Camera&&) noexcept = delete;

		Matrix CalculateCameraToWorld();
		void Update(Timer* pTimer);

		Vector3 origin;
		float fovAngle;
		Vector3 forward;
		Vector3 up;
		Vector3 right;
		float totalPitch;
		float totalYaw;
		Matrix cameraToWorld;
	};
}
