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
	// �������
	glm::vec3 _vPosition;
	glm::vec3 _vFront;
	glm::vec3 _vUp;
	glm::vec3 _vRight;
	glm::vec3 _vWorldUp;
	// ŷ����
	float _fYaw;
	float _fPitch;
	// �������ѡ��
	float _fMoveSpeed;		// ���̿�������ƶ��ٶȸ�������
	float _fSensitivty;		// ��껬������λ�Ʊ任Ϊŷ���ǵĸ�������
	float _fZoom;			// ����������fov�Ǹ�������
private:
	void updateCameraVectors();	// ���������������
};

