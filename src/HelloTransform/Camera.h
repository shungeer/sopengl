#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
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
	Camera(const glm::vec3& pos=glm::vec3(0.0f, 0.0f, 0.0f),
		const glm::vec3& up=glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	~Camera();

	glm::mat4* GetLookAt();
	void KeybordMove(Camera_Movement direction, float deltatime);
	void MouseMove(float offsetX, float offsetY, bool constrainPitch = true);
	void MouseScroll(float offsetY);
public:
	// 相机属性
	glm::vec3 _vPosition;
	glm::vec3 _vFront;
	glm::vec3 _vUp;
	glm::vec3 _vRight;
	glm::vec3 _vWorldUp;
	// 欧拉角
	float _fYaw;
	float _fPitch;
	// 相机控制选项
	float _fMoveSpeed;		// 键盘控制相机移动速度改正参数
	float _fSensitivty;		// 鼠标滑动产生位移变换为欧拉角的改正参数
	float _fZoom;			// 鼠标滚轮缩放fov角改正参数
private:
	void updateCameraVectors();	// 更新相机属性向量
};

