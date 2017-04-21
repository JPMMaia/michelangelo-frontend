#include "Michelangelo.h"
#include "CameraParameters.h"

MichelangeloAPI::CameraParameters::CameraParameters(const std::array<float, 3>& cameraPosition, const std::array<float, 3>& cameraUpDirection, const std::array<float, 3>& cameraForwardDirection, float cameraFov, size_t width, size_t height) :
	m_cameraPosition(cameraPosition),
	m_cameraUpDirection(cameraUpDirection),
	m_cameraForwardDirection(cameraForwardDirection),
	m_cameraFOV(cameraFov),
	m_width(width),
	m_height(height)
{
}

const std::array<float, 3>& MichelangeloAPI::CameraParameters::GetCameraPosition() const
{
	return m_cameraPosition;
}

void MichelangeloAPI::CameraParameters::SetCameraPosition(const std::array<float, 3>& cameraPosition)
{
	m_cameraPosition = cameraPosition;
}

const std::array<float, 3>& MichelangeloAPI::CameraParameters::GetCameraUpDirection() const
{
	return m_cameraUpDirection;
}

void MichelangeloAPI::CameraParameters::SetCameraUpDirection(const std::array<float, 3>& cameraUpDirection)
{
	m_cameraUpDirection = cameraUpDirection;
}

const std::array<float, 3>& MichelangeloAPI::CameraParameters::GetCameraForwardDirection() const
{
	return m_cameraForwardDirection;
}

void MichelangeloAPI::CameraParameters::SetCameraForwardDirection(const std::array<float, 3>& cameraForwardDirection)
{
	m_cameraForwardDirection = cameraForwardDirection;
}

float MichelangeloAPI::CameraParameters::GetCameraFov() const
{
	return m_cameraFOV;
}

void MichelangeloAPI::CameraParameters::SetCameraFov(float cameraFov)
{
	m_cameraFOV = cameraFov;
}

size_t MichelangeloAPI::CameraParameters::GetWidth() const
{
	return m_width;
}

void MichelangeloAPI::CameraParameters::SetWidth(size_t width)
{
	m_width = width;
}

size_t MichelangeloAPI::CameraParameters::GetHeight() const
{
	return m_height;
}

void MichelangeloAPI::CameraParameters::SetHeight(size_t height)
{
	m_height = height;
}
