/*
* Copyright (c) 2012-2013 XZRUNNER http://runnersoft.net
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef D2D_JOINT_H
#define D2D_JOINT_H

class b2Body;
class b2World;
class b2RevoluteJoint;

namespace d2d
{
	struct JointDef;
	class Vector;

	class Joint
	{
	public:
		Joint(b2World* world, const JointDef& params, const Vector& anchor, 
			b2Body* b0, b2Body* b1, bool isMotor, float scale);
		~Joint();

		void setMotorSpeed(bool isPositive);
		void setAngleLimit(bool isLeft);

	private:
		void initialize(const JointDef& params, const Vector& anchor, b2Body* b0, b2Body* b1, bool isMotor);

	private:
		float m_scale;

		b2World* m_world;
		b2RevoluteJoint* m_joint;

		float m_negativeSpeed, m_positiveSpeed;
		float m_leftAngleSmall, m_leftAngleLarge,
			m_rightAngleSmall, m_rightAngleLarge;

	}; // Joint
}

#endif // D2D_JOINT_H
