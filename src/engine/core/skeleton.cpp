/*
 * skeleton.cpp
 *
 *  Created on: Apr 12, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/core/skeleton.h>
#include <engine/core/math/matrix.h>

namespace Engine {

	Skeleton::Skeleton(Uint32 bones, Uint32 ik) {
		this->bone_count = bones;
		this->bone_ik_count = ik;
		this->bones = (Bone*)malloc(sizeof(Bone) * bones);
		if(ik > 0) {
			this->bone_iks = (BoneIK*)malloc(sizeof(BoneIK) * ik);
		} else {
			this->bone_iks = NULL;
		}
	}

	Skeleton::Skeleton(Uint32 bones): Skeleton(bones, 0) {}

	Skeleton::~Skeleton() {
		free(this->bones);
		if(this->bone_iks) {
			free(this->bone_iks);
		}
	}

	Uint32 Skeleton::GetBoneCount() {
		return bone_count;
	}

	Bone* Skeleton::GetBone(Uint32 id) {
		return &bones[id];
	}

	Uint32 Skeleton::GetBoneIKCount() {
		return bone_ik_count;
	}

	BoneIK* Skeleton::GetBoneIK(Uint32 id) {
		return &bone_iks[id];
	}

	Bone* Skeleton::GetBoneByName(String s) {
		Uint32 s_len = strlen(s);

		for (Uint32 i = 0; i < bone_count; ++i) {
			Bone* bone = &bones[i];
			Uint32 b_len = strlen(bone->name);
			if(s_len != b_len) { continue; }

			bool ok = true;

			for (Uint32 k = 0; k < s_len; ++k) {
				if(s[k] != bone->name[k]) {
					ok = false;
					break;
				}
			}

			if(ok) {
				return bone;
			}

		}

		return NULL;

//		for (Uint32 i = 0; i < bone_count; ++i) {
//			Bone* bone = &bones[i];
//			bool ok = true;
//			Uint32 len = strlen(s);
//			for (Uint32 k = 0; k < len; ++k) {
//				if(s[k] == 0 && bone->name[k] == 0) { break; }
//				if(s[k] != bone->name[k]) {
//					ok = false;
//					break;
//				}
//			}
//			if(ok) { return bone; }
//		}
//
//		return NULL;
	}

	void Skeleton::Rebuild() {
		Rebuild(bones);
	}

	void Skeleton::Rebuild(Bone* state) {
		for (Uint32 i = 0; i < bone_count; ++i) {
			Bone* bone = &state[i];
			Engine::Math::mat4 transform;
			Engine::Math::mat4 translate = Engine::Math::IDENTITY_MAT4;
			Engine::Math::mat4 rotate    = Engine::Math::IDENTITY_MAT4;
//			Engine::Math::mat4_identity(&translate);
//			Engine::Math::mat4_identity(&rotate);
			Engine::Math::mat4_translate(&translate, bone->position);
			Engine::Math::mat4_quat(&rotate, bone->rotation);
			Engine::Math::mat4_mul(&transform, &translate, &rotate);

			if(bone->parent == -1) { // Root bone
				bone->transform = transform;
			} else {
				Bone* parent = &state[bone->parent];
				Engine::Math::mat4_mul(&bone->transform, parent->transform, transform);
			}
		}
	}

}
