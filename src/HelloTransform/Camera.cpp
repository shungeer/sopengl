#include "Camera.h"


const float Camera::YAW = -90.0f;
const float Camera::PITCH = 0.0f;
const float Camera::SPEED = 2.5f;
const float Camera::SENSITIVITY = 0.1f;
const float Camera::ZOOM = 45.0f;

Camera::Camera(const glm::vec3& pos, const glm::vec3& up, float yaw, float pitch)
	: _vFront(0.0f, 0.0f, -1.0f), 
	_fMoveSpeed(Camera::SPEED), 
	_fSensitivty(Camera::SENSITIVITY),
	_fZoom(Camera::ZOOM)
{
	_vPosition = pos;
	_vWorldUp = up;
	_fYaw = yaw;
	_fPitch = pitch;
	updateCameraVectors();	// 重新计算相机属性向量
}

Camera::Camera(float posX, float posY, float posZ, 
	float upX, float upY, float upZ, 
	float yaw, float pitch)
	:_vFront(0.0f, 0.0f, -1.0f),
	_fMoveSpeed(Camera::SPEED),
	_fSensitivty(Camera::SENSITIVITY),
	_fZoom(Camera::ZOOM)
{
	_vPosition = glm::vec3(posX, posY, posZ);
	_vWorldUp = glm::vec3(upX, upY, upZ);
	_fYaw = yaw;
	_fPitch = pitch;
	updateCameraVectors();
}

Camera::~Camera()
{
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(_fYaw)) * cos(glm::radians(_fPitch));
	front.y = sin(glm::radians(_fYaw));
	front.z = cos(glm::radians(_fYaw)) * sin(glm::radians(_fPitch));
	_vFront = glm::normalize(front);	// 计算相机目标向量
	_vRight = glm::normalize(glm::cross(_vFront, _vWorldUp)); // 计算相机右向量
	_vUp = glm::normalize(glm::cross(_vRight, _vFront));	  // 计算相机上向量
}

glm::mat4* Camera::GetLookAt()
{
	return &glm::lookAt(_vPosition, _vPosition + _vFront, _vUp);
}

void Camera::KeybordMove(Camera_Movement direction, float deltatime)
{
	float velocity = _fMoveSpeed * deltatime;
	if (direction == Camera::FORWARD)
	{
		_vPosition += velocity * _vFront;
	}
	if (direction == Camera::BACKWARD)
	{
		_vPosition -= velocity * _vFront;
	}
	if (direction == Camera::RIGHT)
	{
		_vPosition += velocity * _vRight;
	}
	if (direction == Camera::LEFT)
	{
		_vPosition -= velocity * _vRight;
	}
}

void Camera::MouseMove(float offsetX, float offsetY, bool constrainPitch)
{
	offsetX *= _fSensitivty;
	offsetY *= _fSensitivty;
	_fYaw += offsetX;
	_fPitch += offsetY;

	if (constrainPitch)
	{
		if (_fPitch > 89.0f)
		{
			_fPitch = 89.0f;
		}
		if (_fPitch < -89.0f)
		{
			_fPitch = -89.0f;
		}
	}
	updateCameraVectors();
}

void Camera::MouseScroll(float offsetY)
{
	if (_fZoom >= 1.0f && _fZoom <= Camera::ZOOM)
	{
		_fZoom -= offsetY;
	}
	if (_fZoom <= 1.0f)
	{
		_fZoom = 1.0f;
	}
	if (_fZoom >= Camera::ZOOM)
	{
		_fZoom = Camera::ZOOM;
	}
}
