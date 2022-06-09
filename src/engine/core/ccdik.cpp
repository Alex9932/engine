/*
 * ccdik.cpp
 *
 *  Created on: May 20, 2022
 *      Author: alex9932
 */

#include <engine/core/ccdik.h>

//#include <engine/core/math/math.h>
#include <engine/core/math/vector.h>
#include <engine/core/math/matrix.h>

namespace Engine {

	static void CalcFK(Skeleton* skeleton, Uint32 b_id, BoneIK* ik, Bone* state) {
		// TODO: Optimize this if possible
		skeleton->Rebuild(state);
	}

	static void SolveOne(Skeleton* skeleton, Uint32 ik_num, Bone* state) {
		BoneIK* ik = skeleton->GetBoneIK(ik_num);

		Uint32 effector_id = ik->effector;
		Uint32 target_id = ik->target;

		Math::vec3 effector_pos;
		Math::vec3 target_pos;
	//			Engine::Math::mat4 tar_mat = skeleton->GetBone(target_id)->transform;
		Math::mat4 tar_mat = state[target_id].transform;
		target_pos.x = tar_mat.m03;
		target_pos.y = tar_mat.m13;
		target_pos.z = tar_mat.m23;

	//			Uint32 iters = SDL_min(ik->max_iterations, 20);
		Uint32 iters = ik->max_iterations;
		for (Uint32 k = 0; k < iters; ++k) {
			bool rotated = false;

			for (Uint32 i = 0; i < ik->bones; ++i) {
				Uint32 link = ik->list[i];
	//					Engine::Math::mat4 m = skeleton->GetBone(link)->transform;
				Math::mat4 m = state[link].transform;
				Math::vec3 p;
				Math::quat q;
				Math::vec3 s;
				Math::mat4_decompose(&p, &q, &s, m);
				Math::quat inv;
				Math::quat_invert(&inv, q);

	//					Engine::Math::mat4 eff_mat = skeleton->GetBone(effector_id)->transform;
				Math::mat4 eff_mat = state[effector_id].transform;
				effector_pos.x = eff_mat.m03;
				effector_pos.y = eff_mat.m13;
				effector_pos.z = eff_mat.m23;

				// Effector vector
				Math::vec3 eff_vec;
				eff_vec.x = effector_pos.x - p.x;
				eff_vec.y = effector_pos.y - p.y;
				eff_vec.z = effector_pos.z - p.z;
				Math::vec3 eff2_vec;
	//			VecByQuat(&eff2_vec, eff_vec, inv);
				Math::vec3_mul(&eff2_vec, eff_vec, inv);
				Math::vec3_normalize(&eff_vec, eff2_vec);

				// Target vector
				Math::vec3 tar_vec;
				tar_vec.x = target_pos.x - p.x;
				tar_vec.y = target_pos.y - p.y;
				tar_vec.z = target_pos.z - p.z;
				Math::vec3 tar2_vec;
	//			VecByQuat(&tar2_vec, tar_vec, inv);
				Math::vec3_mul(&tar2_vec, tar_vec, inv);
				Math::vec3_normalize(&tar_vec, tar2_vec);

				float dot = Math::vec3_dot(eff_vec, tar_vec);

				if (dot > 1.0) { dot = 1.0; }
				else if (dot < -1.0) { dot = -1.0; }

				float angle = SDL_acos(dot);
				if (angle < 1e-5) { continue; }
	//					if (angle < 0) { angle = 0; }
				if (angle > ik->angle_limit) { angle = ik->angle_limit; }
				Math::vec3 c_vec;
				Math::vec3 cn_vec;
				Math::vec3_cross(&c_vec, eff_vec, tar_vec);
				Math::vec3_normalize(&cn_vec, c_vec);
				Math::vec4 axis_angle;
				axis_angle.x = cn_vec.x;
				axis_angle.y = cn_vec.y;
				axis_angle.z = cn_vec.z;
				axis_angle.w = angle;

				Math::quat _q;
				Math::quat_axisAngle(&_q, axis_angle);

	//					Engine::Math::quat _r = skeleton->GetBone(link)->rotation;
				Math::quat _r = state[link].rotation;
				Math::quat _res;
				Math::quat_mul(&_res, _r, _q);
				Math::quat rotation = _res;

				// Angle limits
				Bone* bone = &state[link]; //skeleton->GetBone(link);

//				if(link == 37 || link == 59) { // Rin's limits
//				if(link == 60 || link == 63) { // Kaito's limits
//				if(link == 41 || link == 73) { // Miku's limits
//				if(link == 151 || link == 154) { // Flandre's limits
				if(bone->has_limits) {
					float c = rotation.w;
					if ( c > 1.0 ) c = 1.0;
					float c2 = SDL_sqrtf(1 - c*c);
					rotation.x = bone->limitation.x * c2;
					rotation.y = bone->limitation.y * c2;
					rotation.z = bone->limitation.z * c2;
					rotation.w = c;
				}

				bone->rotation = rotation;

				CalcFK(skeleton, link, ik, state);
				rotated = true;
			}

			if (!rotated) { break; }
		}
	}

	void CCDIK_Solver::Solve(Skeleton* skeleton) {
		Solve(skeleton, skeleton->GetBone(0));
	}

	void CCDIK_Solver::Solve(Skeleton* skeleton, Bone* state) {
		for (Uint32 i = 0; i < skeleton->GetBoneIKCount(); ++i) {
			SolveOne(skeleton, i, state);
		}
	}

}
