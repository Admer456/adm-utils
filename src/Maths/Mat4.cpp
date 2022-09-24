
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

// Adapted from glm::eulerAnglesXYZ by trying out different combinations until I got what I wanted
// Positive pitch will make the forward axis go up
// Positive yaw will make forward and right spin counter-clockwise (if you want it the other way, put -angles.y)
// Positive roll will make the up axis rotate clockwise about the forward axis
// TODO: Move this elsewhere
void CalculateDirections( const Vec3& angles, Vec3& outForward, Vec3& outRight, Vec3& outUp )
{
	constexpr float deg2rad = 3.14159f / 180.0f;

	const float cosPitch = std::cos( -angles.x * deg2rad );
	const float cosYaw = std::cos( angles.y * deg2rad );
	const float cosRoll = std::cos( angles.z * deg2rad );

	const float sinPitch = std::sin( -angles.x * deg2rad );
	const float sinYaw = std::sin( angles.y * deg2rad );
	const float sinRoll = std::sin( angles.z * deg2rad );

	// This one's simple to figure out because this is basic spherical coordinates
	outForward = Vec3{
		cosYaw* cosPitch,
		sinYaw* cosPitch,
		-sinPitch
	};
	// This one was derived from glm::eulerAnglesXYZ, took me a while to
	// decipher stuff like c1, c2, c3, s1, s2 and s3, but this seems to work
	// really well with roll angles
	outUp = Vec3{
		-sinYaw * -sinRoll + cosYaw * sinPitch * cosRoll,
		cosYaw * -sinRoll + sinYaw * sinPitch * cosRoll,
		(cosPitch * cosRoll)
	};
	// Finally, right is just the cross product of these two
	outRight = outForward.Cross( outUp );
}

// ============================
// Mat4::View
// 
// Derived from glm::lookAt initially in my Learnin' NVRHI project:
// https://github.com/Admer456/learnin-nvrhi/blob/master/src/Main.cpp#L674
// ============================
Mat4 Mat4::View( const Vec3& position, const Vec3& angles )
{
	constexpr int forward = 2;
	constexpr int right = 0;
	constexpr int up = 1;
	constexpr int pos = 3;

	constexpr int x = 0;
	constexpr int y = 1;
	constexpr int z = 2;
	constexpr int w = 3;

	Vec3 vForward, vRight, vUp;
	Mat4 m;

	CalculateDirections( angles, vForward, vRight, vUp );

	//m( x, forward ) = -vForward.x;
	//m( y, forward ) = -vForward.y;
	//m( z, forward ) = -vForward.z;
	//m( x, right ) = vRight.x;
	//m( y, right ) = vRight.y;
	//m( z, right ) = vRight.z;
	//m( x, up ) = vUp.x;
	//m( y, up ) = vUp.y;
	//m( z, up ) = vUp.z;
	//m( pos, forward ) = vForward * position;
	//m( pos, right ) = -(vRight * position);
	//m( pos, up ) = -(vUp * position);

	return Mat4{
		Vec4{ vRight,     -(vRight * position) },
		Vec4{ vUp,           -(vUp * position) },
		Vec4{ -vForward,   vForward * position },
		Vec4{ Vec3::Zero,                 1.0f }
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
