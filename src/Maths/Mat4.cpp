
#include "Precompiled.hpp"
using namespace adm;

// ============================
// Mat4::Perspective
// ============================
Mat4 Mat4::Perspective( float fovY, float aspectRatio, float zNear, float zFar )
{
	const float height = 1.0f / std::tan( fovY * 0.5f );
	const float width = height / aspectRatio;
	const float range = zFar / (zNear - zFar);

	return Mat4{
		Vec4{ width, 0.0f, 0.0f, 0.0f },
		Vec4{ 0.0f, height, 0.0f, 0.0f },
		Vec4{ 0.0f, 0.0f, range, -1.0f },
		Vec4{ 0.0f, 0.0f, range * zNear, 0.0f }
	};
}

// ============================
// Mat4::Orthographic
// ============================
Mat4 Mat4::Orthographic( float left, float right, float bottom, float top, float zNear, float zFar )
{
	// Might wanna flip this if stuff is acting weird
	const float sign = 1.0f;
	const float range = zFar - zNear;

	const float width = 2.0f / (right - left);
	const float height = 2.0f / (top - bottom);

	const float horizontalRange = -(right + left) / (right - left);
	const float verticalRange = -(top + bottom) / (top - bottom);

	return Mat4{
		Vec4{ width, 0.0f,   0.0f,         horizontalRange },
		Vec4{ 0.0f,  height, 0.0f,         verticalRange },
		Vec4{ 0.0f,  0.0f,   sign / range, -zNear / range },
		Vec4{ 0.0f,  0.0f,   0.0f,         1.0f }
	};
}

const Mat4 Mat4::Identity = Mat4{
	Vec4{ 1.0f, 0.0f, 0.0f, 0.0f },
	Vec4{ 0.0f, 1.0f, 0.0f, 0.0f },
	Vec4{ 0.0f, 0.0f, 1.0f, 0.0f },
	Vec4{ 0.0f, 0.0f, 0.0f, 1.0f }
};

const Mat4 Mat4::One = Mat4{ 1.0f };
const Mat4 Mat4::Zero = Mat4{ 0.0f };
