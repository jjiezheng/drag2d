#pragma once

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

