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
	updateCameraVectors();	// ���¼��������������
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
	front.x = cos(glm::radians(_fPitch)) * cos(glm::radians(_fYaw));
	front.y = sin(glm::radians(_fPitch));
	front.z = cos(glm::radians(_fPitch)) * sin(glm::radians(_fYaw));
	_vFront = glm::normalize(front);	// �������Ŀ������
	_vRight = glm::normalize(glm::cross(_vFront, _vWorldUp)); // �������������
	_vUp = glm::normalize(glm::cross(_vRight, _vFront));	  // �������������
}

glm::mat4 Camera::GetLookAt()
{
	return glm::lookAt(_vPosition, _vPosition + _vFront, _vUp);
}

glm::mat4 Camera::CalculateLookAt()
{
	glm::vec3 target = glm::normalize(_vPosition + _vFront);	// ʹ�������Զָ������
	// �����������
	glm::vec3 zAxis = glm::normalize(_vPosition - target);		// �����������
	glm::vec3 xAxis = glm::normalize(glm::cross(glm::normalize(_vWorldUp), zAxis));	// �������
	glm::vec3 yAxis = glm::normalize(glm::cross(zAxis, xAxis));	// �������

	// ����lookat����ת�����λ�ƾ���(��������)
	glm::mat4 translation;
	translation[3][0] = -_vPosition.x;	// �����е�һ��
	translation[3][1] = -_vPosition.y;
	translation[3][2] = -_vPosition.z;
	glm::mat4 rotation;
	rotation[0][0] = xAxis.x;		// ��һ�� ��һ��
	rotation[1][0] = xAxis.y;
	rotation[2][0] = xAxis.z;
	rotation[0][1] = yAxis.x;		// ��һ�� �ڶ���
	rotation[1][1] = yAxis.y;
	rotation[2][1] = yAxis.z;
	rotation[0][2] = zAxis.x;		// ��һ�� ������
	rotation[1][2] = zAxis.y;
	rotation[2][2] = zAxis.z;

	return rotation * translation;
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
	if (_fZoom >= 1.0f && _fZoom <= Camera::ZOOM+45.0f)
	{
		_fZoom -= offsetY;
	}
	if (_fZoom <= 1.0f)
	{
		_fZoom = 1.0f;
	}
	if (_fZoom >= Camera::ZOOM + 45.0f)
	{
		_fZoom = Camera::ZOOM + 45.0f;
	}
}
