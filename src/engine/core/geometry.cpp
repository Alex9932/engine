/*
 * geometry.cpp
 *
 *  Created on: Mar 1, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/core/geometry.h>
#include <engine/core/filesystem.h>
#include <engine/core/math/vector.h>
#include <engine/render/material.h>
#include <engine/render/mesh.h>

namespace Engine {

	GeometryLoader::GeometryLoader(Allocator* allocator) { this->allocator = allocator; }
	GeometryLoader::~GeometryLoader() {}
	Geometry* GeometryLoader::LoadGeometry(String path) { return NULL; }
	void GeometryLoader::FreeGeometry(Geometry* geom) {
		allocator->Deallocate(geom->materials);
		allocator->Deallocate(geom->vertices);
		allocator->Deallocate(geom->indices);
		allocator->Deallocate(geom->indices_count);
		allocator->Deallocate(geom);
	}


	RiggedGeometryLoader::RiggedGeometryLoader(Allocator* allocator) { this->allocator = allocator; }
	RiggedGeometryLoader::~RiggedGeometryLoader() {}
	RiggedGeometry* RiggedGeometryLoader::LoadGeometry(String path) { return NULL; }
	void RiggedGeometryLoader::FreeGeometry(RiggedGeometry* geom) {
		allocator->Deallocate(geom->materials);
		allocator->Deallocate(geom->vertices);
		allocator->Deallocate(geom->weights);
		allocator->Deallocate(geom->indices);
		allocator->Deallocate(geom->indices_count);
		allocator->Deallocate(geom);
	}


	// RGM Implementation

	struct rg_rmlHeader_t {
		char magic[4];
		Uint32 vertices;
		Uint32 indices;
		Uint32 rml_version;
	};

	RGMGeometryLoader::RGMGeometryLoader(Allocator* allocator) : GeometryLoader(allocator) { }
	RGMGeometryLoader::~RGMGeometryLoader() {}
	Geometry* RGMGeometryLoader::LoadGeometry(String path) {
		Filesystem::Resource* res = Filesystem::GetResource(path);

		size_t pointer = 0;
		char* data = (char*)res->data;

		rg_rmlHeader_t header = *(rg_rmlHeader_t*)data[pointer];
		pointer += sizeof(rg_rmlHeader_t);
		Render::Vertex* vertices = (Render::Vertex*)data[pointer];
		pointer += sizeof(Render::Vertex) * header.vertices;
		Uint32* indices = (Uint32*)data[pointer];

		Geometry* geom = (Geometry*)allocator->Allocate(sizeof(Geometry));
//		Geometry* geom = (Geometry*)malloc(sizeof(Geometry));

		geom->meshes = 1;
		geom->materials = (Render::Material**)allocator->Allocate(sizeof(Render::Material*));
		geom->materials[0] = NULL; // Set manually
		geom->indices_count = (Uint32*)allocator->Allocate(sizeof(Uint32));
		geom->indices_count[0] = header.indices;
		geom->indices_total = header.indices;
		geom->vertex_count = header.vertices;

		geom->vertices = (Render::Vertex*)allocator->Allocate(sizeof(Render::Vertex) * header.vertices);
		SDL_memcpy(geom->vertices, vertices, sizeof(Render::Vertex) * header.vertices);

		geom->indices = (Uint32*)allocator->Allocate(sizeof(Uint32) * header.indices);
		bool error = false;
		for (Uint32 i = 0; i < header.indices; ++i) {
			if(indices[i] >= 65536) { error = true; }
			geom->indices[i] = (Uint32)indices[i];
		}

		if(error) {
			rgLogInfo(RG_LOG_SYSTEM, "RGM: Model loaded with error! This may cause some graphical glitches!");
		}

		Filesystem::FreeResource(res);

		return geom;
	}

}
