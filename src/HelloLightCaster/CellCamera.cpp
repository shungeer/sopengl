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
	updateCameraVectors();	// 重新计算相机属性向量
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
	_vFront = tmp.normalize();	// 计算相机目标向量
	tmp = _vFront.cross(_vWorldUp);
	_vRight = tmp.normalize(); // 计算相机右向量
	_vUp = _vRight.cross(_vFront);
	//_vUp = tmp.normalize();	  // 计算相机上向量
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
	Vec3<float> target = tmp.normalize();	// 使得相机永远指向物体
	// 相机属性向量
	tmp = _vPosition - target;
	Vec3<float> zAxis = tmp.normalize();		// 相机方向向量
	tmpnup = _vWorldUp;
	tmpnup.normalize();
	tmp = tmpnup.cross(zAxis);
	Vec3<float> xAxis = tmp.normalize();	// 相机右轴
	tmp = zAxis.cross(xAxis);
	Vec3<float> yAxis = tmp.normalize();	// 相机上轴

	// 创建lookat的旋转矩阵和位移矩阵(列主次序)
	Mat4<float> translation;
	translation.setColumn(3, -_vPosition);// 第三列
	Mat4<float> rotation;
	rotation.setRow(0, xAxis); // 第一行
	rotation.setRow(1, yAxis);		// 第二行
	rotation.setRow(2, zAxis);		// 第三行

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
