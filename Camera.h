#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

struct Camera {
	vec3 position;
	vec3 target;
	vec3 up;
	float radius;
	float azimuthAngle;
	float polarAngle;
	float fov = 90.f;
	float aspect = 4.0f / 3.0f;
	float near = 0.1f;
	float far = 100.f;

	Camera(vec3 p, vec3 t, vec3 u)
		: position(p)
		, target(t)
		, up(u)
		, radius(length(p - t))
		, azimuthAngle(0.0f)
		, polarAngle(0.0f)
	{
		
	}

	void OnMouseMove(int x, int y) {
		azimuthAngle += x * 0.01;
		constexpr auto fullCircle = 2.0f * glm::pi<float>();
		azimuthAngle = fmodf(azimuthAngle, fullCircle);
		if (azimuthAngle < 0.0f) {
			azimuthAngle = fullCircle + azimuthAngle;
		}

		polarAngle += y * 0.01;
		constexpr auto polarCap = glm::pi<float>() / 2.0f - 0.001f;
		if (polarAngle > polarCap) {
			polarAngle = polarCap;
		}

		if (polarAngle < -polarCap) {
			polarAngle = -polarCap;
		}

	}

	glm::vec3 UpdatePosition() const {
		auto sinAzimuth = sin(azimuthAngle);
		auto cosAzimuth = cos(azimuthAngle);
		auto sinPolar = sin(polarAngle);
		auto cosPolar = cos(polarAngle);

		auto x = target.x + radius * cosPolar * cosAzimuth;
		auto y = target.y + radius * sinPolar;
		auto z = target.z + radius * cosPolar * sinAzimuth;

		return glm::vec3(x, y, z);
	}


	mat4 GetViewMatrix() {
		position = UpdatePosition();
		return lookAt(position, target, up);
	}

	mat4 GetProjectionMatrix() {
		return perspective(glm::radians(fov), aspect, near, far);
	}
	glm::vec3 GetRightVector()  
	{
		return glm::transpose(GetViewMatrix())[0]; 
	}

};