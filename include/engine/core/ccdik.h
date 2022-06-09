/*
 * ccdik.h
 *
 *  Created on: May 20, 2022
 *      Author: alex9932
 */

#ifndef CCDIK_H_
#define CCDIK_H_

#include <engine/engine.h>
#include <engine/core/skeleton.h>

namespace Engine {

	class RG_DECLSPEC CCDIK_Solver {
		public:
			CCDIK_Solver() {}
			~CCDIK_Solver() {}

	//		void CalcFK(Engine::Skeleton* skeleton, Uint32 b_id, Engine::BoneIK* ik, Engine::Bone* state);
	//		void VecByQuat(Engine::Math::vec3* vec, const Engine::Math::vec3& v, const Engine::Math::quat& q);
	//		void SolveOne(Engine::Skeleton* skeleton, Uint32 ik_num, Engine::Bone* state);
			void Solve(Skeleton* skeleton);
			void Solve(Skeleton* skeleton, Bone* state);
	};

}


#endif /* CCDIK_H_ */
