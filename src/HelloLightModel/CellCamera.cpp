#include "CellCamera.h"


const float CellCamera::YAW = -90.0f;
const float CellCamera::PITCH = 0.0f;
const float CellCamera::SPEED = 2.5f;
const float CellCamera::SENSITIVITY = 0.01f;
const float CellCamera::ZOOM = 45.0f;

CellCamera::CellCamera(const Vec3<float>& pos, const Vec3<float>& up, float yaw, float pitch)
	: _vFront(0.0f, 0.0f, -1.0f), 
	_fMoveSpeed(CellCamera::SPEED), 
	_fSensitivty(CellCamera::SENSITIVITY),
	_fZoom(CellCamera::ZOOM)
{
	_vPosition = pos;
	_vWorldUp = up;
	_fYaw = yaw;
	_fPitch = pitch;
	updateCameraVectors();	// ���¼��������������
}

CellCamera::CellCamera(float posX, float posY, float posZ, 
	float upX, float upY, float upZ, 
	float yaw, float pitch)
	:_vFront(0.0f, 0.0f, -1.0f),
	_fMoveSpeed(CellCamera::SPEED),
	_fSensitivty(CellCamera::SENSITIVITY),
	_fZoom(CellCamera::ZOOM)
{
	_vPosition = Vec3<float>(posX, posY, posZ);
	_vWorldUp = Vec3<float>(upX, upY, upZ);
	_fYaw = yaw;
	_fPitch = pitch;
	updateCameraVectors();
}

CellCamera::~CellCamera()
{
}

void CellCamera::updateCameraVectors()
{
	Vec3<float> tmp;
	tmp[0] = cos(DegreesToRadians(_fPitch)) * cos(DegreesToRadians(_fYaw));
	tmp[1] = sin(DegreesToRadians(_fPitch));
	tmp[2] = cos(DegreesToRadians(_fPitch)) * sin(DegreesToRadians(_fYaw));
	_vFront = tmp.normalize();	// �������Ŀ������
	tmp = _vFront.cross(_vWorldUp);
	_vRight = tmp.normalize(); // �������������
	_vUp = _vRight.cross(_vFront);
	//_vUp = tmp.normalize();	  // �������������
}

Mat4<float> CellCamera::GetLookAt()
{
	Mat4<float> m;
	m.lookAt(_vPosition, _vFront + _vPosition, _vUp);
	return m;
}

Mat4<float> CellCamera::CalculateLookAt()
{
	Vec3<float> tmp, tmpnup;
	tmp = _vPosition + _vFront;
	Vec3<float> target = tmp.normalize();	// ʹ�������Զָ������
	// �����������
	tmp = _vPosition - target;
	Vec3<float> zAxis = tmp.normalize();		// �����������
	tmpnup = _vWorldUp;
	tmpnup.normalize();
	tmp = tmpnup.cross(zAxis);
	Vec3<float> xAxis = tmp.normalize();	// �������
	tmp = zAxis.cross(xAxis);
	Vec3<float> yAxis = tmp.normalize();	// �������

	// ����lookat����ת�����λ�ƾ���(��������)
	Mat4<float> translation;
	translation.setColumn(3, -_vPosition);// ������
	Mat4<float> rotation;
	rotation.setRow(0, xAxis); // ��һ��
	rotation.setRow(1, yAxis);		// �ڶ���
	rotation.setRow(2, zAxis);		// ������

	return rotation * translation;
}

void CellCamera::KeybordMove(Camera_Movement direction, float deltatime)
{
	float velocity = _fMoveSpeed * deltatime;
	if (direction == CellCamera::FORWARD)
	{
		_vPosition += velocity * _vFront;
	}
	if (direction == CellCamera::BACKWARD)
	{
		_vPosition -= velocity * _vFront;
	}
	if (direction == CellCamera::RIGHT)
	{
		_vPosition += velocity * _vRight;
	}
	if (direction == CellCamera::LEFT)
	{
		_vPosition -= velocity * _vRight;
	}
}

void CellCamera::MouseMove(float offsetX, float offsetY, bool constrainPitch)
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

void CellCamera::MouseScroll(float offsetY)
{
	if (_fZoom >= 1.0f && _fZoom <= CellCamera::ZOOM + 45.0f)
	{
		_fZoom -= offsetY;
	}
	if (_fZoom <= 1.0f)
	{
		_fZoom = 1.0f;
	}
	if (_fZoom >= CellCamera::ZOOM + 45.0f)
	{
		_fZoom = CellCamera::ZOOM + 45.0f;
	}
}
