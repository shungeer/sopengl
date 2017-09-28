#pragma once
#include "CellVec3.h"
#include "CellMat4.h"

using namespace cell;

class CellCamera
{
public:
	enum Camera_Movement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};
	static const float YAW;
	static const float PITCH;
	static const float SPEED;
	static const float SENSITIVITY;
	static const float ZOOM;
public:
	CellCamera(const Vec3<float>& pos = Vec3<float>(0.0f, 0.0f, 0.0f),
		const Vec3<float>& up=Vec3<float>(0.0f, 1.0f, 0.0f),
		float yaw = YAW, float pitch = PITCH);
	CellCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	~CellCamera();

	Mat4<float> GetLookAt();
	Mat4<float> CalculateLookAt();
	void KeybordMove(Camera_Movement direction, float deltatime);
	void MouseMove(float offsetX, float offsetY, bool constrainPitch = true);
	void MouseScroll(float offsetY);
public:
	// 相机属性
	Vec3<float> _vPosition;
	Vec3<float> _vFront;
	Vec3<float> _vUp;
	Vec3<float> _vRight;
	Vec3<float> _vWorldUp;
	// 欧拉角
	float _fYaw;
	float _fPitch;
	// 相机控制选项
	float _fMoveSpeed;		// 键盘控制相机移动速度改正参数
	float _fSensitivty;		// 鼠标滑动产生位移变换为欧拉角的改正参数
	float _fZoom;			// 鼠标滚轮缩放fov角
private:
	void updateCameraVectors();	// 更新相机属性向量
};

