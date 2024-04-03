#include "Camera.h"

namespace dae
{
	Camera::Camera() :
		origin{ Vector3::Zero },
		fovAngle{ 90.0f },
		forward{ Vector3::UnitZ },
		up{ Vector3::UnitY },
		right{ Vector3::UnitX },
		totalPitch{ 0.0f },
		totalYaw{ 0.0f },
		cameraToWorld{ Matrix{} }
	{

	}

	Camera::Camera(const Vector3& origin, float fovAngle) :
		origin{ origin },
		fovAngle{ fovAngle },
		forward{ Vector3::UnitZ },
		up{ Vector3::UnitY },
		right{ Vector3::UnitX },
		totalPitch{ 0.0f },
		totalYaw{ 0.0f },
		cameraToWorld{ Matrix{} }
	{

	}

	Matrix Camera::CalculateCameraToWorld()
	{
		right = Vector3{ Vector3::Cross(Vector3::UnitY, forward).Normalized() };
		up = Vector3{ Vector3::Cross(forward, right).Normalized() };

		cameraToWorld = Matrix{ right, up, forward, origin };

		return cameraToWorld;
	}

	void Camera::Update(Timer* pTimer)
	{
		const float deltaTime = pTimer->GetElapsed();
		const float movementInterval{ 5.0f };
		const float rotationInterval{ dae::TO_RADIANS * 5.0f };

		//Keyboard Input
		const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);

		if (pKeyboardState[SDL_SCANCODE_W])
		{
			origin += forward * movementInterval * deltaTime;
		}
		if (pKeyboardState[SDL_SCANCODE_S])
		{
			origin -= forward * movementInterval * deltaTime;
		}
		if (pKeyboardState[SDL_SCANCODE_D])
		{
			origin += right * movementInterval * deltaTime;
		}
		if (pKeyboardState[SDL_SCANCODE_A])
		{
			origin -= right * movementInterval * deltaTime;
		}


		//Mouse Input
		int mouseX{}, mouseY{};
		const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

		const bool LeftMouseButtonDown{ bool(mouseState & SDL_BUTTON_LMASK) };
		const bool RightMouseButtonDown{ bool(mouseState & SDL_BUTTON_RMASK) };

		if (LeftMouseButtonDown)
		{
			if (RightMouseButtonDown)
			{
				origin -= up * float(mouseY) * deltaTime;
			}
			else
			{
				origin -= forward * float(mouseY) * deltaTime;
				totalYaw += float(mouseX) * rotationInterval * deltaTime;
			}
		}
		else if (RightMouseButtonDown)
		{
			totalYaw += float(mouseX) * rotationInterval * deltaTime;
			totalPitch += float(mouseY) * rotationInterval * deltaTime;
		}

		Matrix finalRotation{ Matrix::CreateRotation(totalPitch, totalYaw, 0.0f) };
		forward = finalRotation.TransformVector(Vector3::UnitZ);
		forward.Normalize();
	}
}