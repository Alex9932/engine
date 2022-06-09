/*
 * skeleton.h
 *
 *  Created on: Apr 12, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_CORE_SKELETON_H_
#define ENGINE_CORE_SKELETON_H_

#include <engine/engine.h>
#include <engine/core/math/math.h>

#define RG_SKELETON_MAX_IK_LINKS 32

namespace Engine {

	typedef struct Bone {
		char name[32];
		Uint32 id;
		Sint32 parent;
		Sint32 child;
		Math::mat4 transform;
		Math::mat4 offset;
		Math::vec3 position;
		Math::quat rotation;
		Math::vec3 pos_offset;
		bool has_limits;
		Math::vec3 angle_limit_min;
		Math::vec3 angle_limit_max;
		Math::vec3 limitation;
	} Bone;

	typedef struct BoneIK {
		Uint16 target;
		Uint16 effector;
		Uint16 bones;
		Uint16 max_iterations;
		float angle_limit;
		Uint16 list[RG_SKELETON_MAX_IK_LINKS];
	} BoneIK;

	class RG_DECLSPEC Skeleton {
		private:
			Uint32 bone_count;
			Bone* bones;
			Uint32 bone_ik_count;
			BoneIK* bone_iks;

		public:
			Skeleton(Uint32 bones, Uint32 ik);
			Skeleton(Uint32 bones);
			virtual ~Skeleton();
			Uint32 GetBoneCount();
			Bone* GetBone(Uint32 id);
			Uint32 GetBoneIKCount();
			BoneIK* GetBoneIK(Uint32 id);
			Bone* GetBoneByName(String s);
			void Rebuild();
			void Rebuild(Bone* state);
	};

}


#endif /* ENGINE_CORE_SKELETON_H_ */
