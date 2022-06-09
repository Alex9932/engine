/*
 * geometry.h
 *
 *  Created on: Mar 1, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_CORE_GEOMETRY_H_
#define ENGINE_CORE_GEOMETRY_H_

#include <engine/render/material.h>
#include <engine/render/mesh.h>
#include <engine/core/allocator.h>

namespace Engine {

	typedef struct Geometry {
		Render::Material** materials;  // Materials
		Render::Vertex* vertices;      // All vertices of the model
		Uint32* indices;               // All indices of the model
		Uint32* indices_count;         // Each value indicates how faces be affected by material
		Uint32 indices_total;          // Total count of indices
		Uint32 vertex_count;           // Count of vertices
		Uint32 meshes;                 // Count of materials and "indices_count"
	} Geometry;

	typedef struct RiggedGeometry {
		Render::Material** materials;  // Materials
		Render::Vertex* vertices;      // All vertices of the model
		Render::Weight* weights;       // All weights of the model
		Uint32* indices;               // All indices of the model
		Uint32* indices_count;         // Each value indicates how faces be affected by material
		Uint32 indices_total;          // Total count of indices
		Uint32 vertex_count;           // Count of vertices & weights
		Uint32 meshes;                 // Count of materials and "indices_count"
	} RiggedGeometry;


	class RG_DECLSPEC GeometryLoader {
		protected:
			Allocator* allocator;

		public:
			GeometryLoader(Allocator* allocator);
			virtual ~GeometryLoader();
			virtual Geometry* LoadGeometry(String path);
			void FreeGeometry(Geometry* ptr);
	};

	class RG_DECLSPEC RiggedGeometryLoader {
		protected:
			Allocator* allocator;

		public:
			RiggedGeometryLoader(Allocator* allocator);
			virtual ~RiggedGeometryLoader();
			virtual RiggedGeometry* LoadGeometry(String path);
			void FreeGeometry(RiggedGeometry* ptr);
	};

	class RG_DECLSPEC RGMGeometryLoader: public GeometryLoader {
		public:
			RGMGeometryLoader(Allocator* allocator);
			virtual ~RGMGeometryLoader();
			virtual Geometry* LoadGeometry(String path);
	};

}


#endif /* ENGINE_CORE_GEOMETRY_H_ */
