/*
* Copyright (c) 2012-2013 Guang Zhu http://runnersoft.net
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

#ifndef D2D_JOINT_DEFINE_H
#define D2D_JOINT_DEFINE_H

#include <iostream>

namespace d2d
{
	struct JointDef
	{
		float positive_speed, negative_speed;
		float left_angle_small, left_angle_large,
			right_angle_small, right_angle_large;
		float max_torque;

		bool enableLimit, enableMotor;

		JointDef();

	}; // JointDef 

	inline JointDef::JointDef()
	{
		positive_speed = 2.0f;
		negative_speed = -2.0f;
		max_torque = 0.0f;

		enableLimit = enableMotor = true;
	}

	inline std::istream& operator >> (std::istream& is, JointDef& params)
	{
		is >> params.positive_speed >> params.negative_speed
			>> params.left_angle_small >> params.left_angle_large
			>> params.right_angle_small >> params.right_angle_large
			>> params.max_torque;
		return is;
	}

	inline std::ostream& operator << (std::ostream& os, const JointDef& params)
	{
		os << params.positive_speed << " " << params.negative_speed << " "
			<< params.left_angle_small << " " << params.left_angle_large << " " 
			<< params.right_angle_small << " " << params.right_angle_large << " "
			<< params.max_torque;
		return os;
	}
}

#endif // D2D_JOINT_DEFINE_H
