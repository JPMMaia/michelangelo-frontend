#pragma once

#include <array>

namespace MichelangeloAPI
{
	class CameraParameters
	{
	public:
		CameraParameters() = default;
		CameraParameters(const std::array<float, 3>& cameraPosition, const std::array<float, 3>& cameraUpDirection, const std::array<float, 3>& cameraForwardDirection, float cameraFov, size_t width, size_t height);

		const std::array<float, 3>& GetCameraPosition() const;
		void SetCameraPosition(const std::array<float, 3>& cameraPosition);
		const std::array<float, 3>& GetCameraUpDirection() const;
		void SetCameraUpDirection(const std::array<float, 3>& cameraUpDirection);
		const std::array<float, 3>& GetCameraForwardDirection() const;
		void SetCameraForwardDirection(const std::array<float, 3>& cameraForwardDirection);
		float GetCameraFov() const;
		void SetCameraFov(float cameraFov);
		size_t GetWidth() const;
		void SetWidth(size_t width);
		size_t GetHeight() const;
		void SetHeight(size_t height);

	private:
		std::array<float, 3> m_cameraPosition;
		std::array<float, 3> m_cameraUpDirection;
		std::array<float, 3> m_cameraForwardDirection;
		float m_cameraFOV = 90.0f;
		size_t m_width = 0;
		size_t m_height = 0;
	};
}
