
#include "Precompiled.hpp"
using namespace adm;

const Plane Plane::Zero		= Plane( 0.0f, 0.0f, 0.0f, 0.0f );
const Plane Plane::Forward	= Plane( Vec3::Forward, 0.0f );
const Plane Plane::Right	= Plane( Vec3::Right, 0.0f );
const Plane Plane::Up		= Plane( Vec3::Up, 0.0f );
