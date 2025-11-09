#include "extension.h"
#include "daabbcc.h"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/print_string.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/packed_float32_array.hpp"
#include "godot_cpp/variant/packed_int32_array.hpp"

using namespace daabbcc;

const uint64_t DaabbccExtension::CATEGORY_ALL = ~0ULL;

void DaabbccExtension::_bind_methods() {
	godot::ClassDB::bind_method(
			D_METHOD("init", "max_group_count", "max_gameobject_count", "max_query_count"),
			&DaabbccExtension::init);

	//--------------------------------------------
	// Group Operations
	//--------------------------------------------
	godot::ClassDB::bind_method(
			D_METHOD("new_group", "tree_build_type"),
			&DaabbccExtension::new_group,
			DEFVAL(daabbcc::UPDATE_INCREMENTAL));

	godot::ClassDB::bind_method(
			D_METHOD("remove_group", "group_id"),
			&DaabbccExtension::remove_group);

	//--------------------------------------------
	// Proxy Operations
	//--------------------------------------------
	godot::ClassDB::bind_method(
			D_METHOD("insert_aabb", "group_id", "position", "size", "category_bits"),
			&DaabbccExtension::insert_aabb,
			DEFVAL(CATEGORY_ALL));

	godot::ClassDB::bind_method(
			D_METHOD("update_aabb", "group_id", "proxy_id", "position", "size"),
			&DaabbccExtension::update_aabb);

	godot::ClassDB::bind_method(
			D_METHOD("remove", "group_id", "proxy_id"),
			&DaabbccExtension::remove);

	//--------------------------------------------
	// Query Operations
	//--------------------------------------------

	godot::ClassDB::bind_method(
			D_METHOD("query_aabb", "group_id", "position", "size", "is_manifold", "category_bits"),
			&DaabbccExtension::query_aabb,
			DEFVAL(false),
			DEFVAL(CATEGORY_ALL));

	godot::ClassDB::bind_method(
			D_METHOD("query_id", "group_id", "proxy_id", "is_manifold", "category_bits"),
			&DaabbccExtension::query_id,
			DEFVAL(false),
			DEFVAL(CATEGORY_ALL));

	godot::ClassDB::bind_method(
			D_METHOD("query_aabb_sort", "group_id", "position", "size", "is_manifold", "category_bits"),
			&DaabbccExtension::query_aabb_sort,
			DEFVAL(false),
			DEFVAL(CATEGORY_ALL));

	godot::ClassDB::bind_method(
			D_METHOD("query_id_sort", "group_id", "proxy_id", "is_manifold", "category_bits"),
			&DaabbccExtension::query_id_sort,
			DEFVAL(false),
			DEFVAL(CATEGORY_ALL));

	//--------------------------------------------
	// RAYCAST
	//--------------------------------------------

	godot::ClassDB::bind_method(
			D_METHOD("raycast", "group_id", "start_position", "end_position", "is_manifold", "category_bits"),
			&DaabbccExtension::raycast,
			DEFVAL(false),
			DEFVAL(CATEGORY_ALL));

	godot::ClassDB::bind_method(
			D_METHOD("raycast_sort", "group_id", "start_position", "end_position", "is_manifold", "category_bits"),
			&DaabbccExtension::raycast_sort,
			DEFVAL(false),
			DEFVAL(CATEGORY_ALL));

	//--------------------------------------------
	// UTILS
	//--------------------------------------------
	godot::ClassDB::bind_method(
			D_METHOD("rebuild", "group_id", "full_build"),
			&DaabbccExtension::rebuild,
			DEFVAL(true));

	godot::ClassDB::bind_method(
			D_METHOD("rebuild_all", "full_build"),
			&DaabbccExtension::rebuild_all,
			DEFVAL(true));

	godot::ClassDB::bind_method(
			D_METHOD("reset"),
			&DaabbccExtension::reset);

	//--------------------------------------------
	// ENUMS
	//--------------------------------------------
	BIND_ENUM_CONSTANT(UPDATE_INCREMENTAL);
	BIND_ENUM_CONSTANT(UPDATE_FULLREBUILD);
	BIND_ENUM_CONSTANT(UPDATE_PARTIALREBUILD);
}

void DaabbccExtension::init(uint8_t max_group_count, uint16_t max_gameobject_count, uint16_t max_query_count) {
	daabbcc::Setup(max_group_count, max_gameobject_count, max_query_count);
}

// ===========================================
// Group Operations
// ===========================================

uint8_t DaabbccExtension::new_group(uint8_t tree_build_type) {
	return daabbcc::AddGroup(tree_build_type);
}

void DaabbccExtension::remove_group(uint8_t group_id) {
	bool isSet = daabbcc::SetTreeGroup(group_id);
	if (!isSet) {
		ERR_FAIL_MSG("daabbcc.remove_group(): invalid group id");
	}

	daabbcc::RemoveGroup(group_id);
}

// ===========================================
// Proxy Operations
// ===========================================

int32_t DaabbccExtension::insert_aabb(uint8_t group_id, Vector2 position, Vector2 size, uint64_t category_bits) {
	bool isSet = daabbcc::SetTreeGroup(group_id);

	if (!isSet) {
		ERR_FAIL_V_MSG(int32_t(), "daabbcc.insert_aabb(): invalid group id");
	}

	return daabbcc::AddProxy(group_id, position.x, position.y, size.x, size.y, category_bits);
}

void DaabbccExtension::update_aabb(uint8_t group_id, int32_t proxy_id, Vector2 position, Vector2 size) {
	bool isSet = daabbcc::SetTreeGroup(group_id);
	if (!isSet) {
		ERR_FAIL_MSG("daabbcc.update_aabb(): invalid group id");
	}

	daabbcc::MoveProxy(proxy_id, position.x, position.y, size.width, size.height);
}

void DaabbccExtension::remove(uint8_t group_id, int32_t proxy_id) {
	bool isSet = daabbcc::SetTreeGroup(group_id);
	if (!isSet) {
		ERR_FAIL_MSG("daabbcc.remove(): invalid group id");
	}
	daabbcc::RemoveProxy(group_id, proxy_id);
}

// ===========================================
// Query Operations
// ===========================================

void DaabbccExtension::generate_results(uint32_t queryResultSize, dmArray<uint16_t> &queryResult, Array &result) {
	PackedInt32Array ids;
	ids.resize(queryResultSize);

	for (int i = 0; i < queryResultSize; i++) {
		ids[i] = queryResult[i];
	}
	result.append(ids);
}

void DaabbccExtension::generate_short_results(uint32_t queryResultSize, dmArray<daabbcc::ManifoldResult> &queryResult, Array &result) {
	PackedInt32Array ids;
	PackedFloat32Array distances;

	ids.resize(queryResultSize);
	distances.resize(queryResultSize);

	for (int i = 0; i < queryResultSize; i++) {
		const ManifoldResult &r = queryResult[i]; // const reference
		ids[i] = r.m_proxyID;
		distances[i] = r.m_distance;
	}
	result.append(ids);
	result.append(distances);
}

void DaabbccExtension::generate_manifold_results(uint32_t queryResultSize, dmArray<daabbcc::ManifoldResult> &queryResult, Array &result) {
	PackedInt32Array ids;
	PackedVector2Array contact_points; // Vector2 for contact_point
	PackedVector2Array normals; // Vector2 for normal
	PackedFloat32Array depths;
	PackedFloat32Array distances;

	// Reserve capacity upfront (IMPORTANT for performance)
	ids.resize(queryResultSize);
	contact_points.resize(queryResultSize);
	normals.resize(queryResultSize);
	depths.resize(queryResultSize);
	distances.resize(queryResultSize);

	for (int i = 0; i < queryResultSize; i++) {
		const ManifoldResult &r = queryResult[i]; // const reference

		ids[i] = r.m_proxyID;
		depths[i] = r.m_manifold.depth;
		contact_points[i] = Vector2(r.m_manifold.contact_point.x, r.m_manifold.contact_point.y);
		normals[i] = Vector2(r.m_manifold.n.x, r.m_manifold.n.y); // Note: Vector2, not Vector2i
		distances[i] = r.m_distance;
	}
	result.append(ids);
	result.append(contact_points);
	result.append(normals);
	result.append(depths);
	result.append(distances);
}

Array DaabbccExtension::query_aabb(uint8_t group_id, Vector2 position, Vector2 size, bool is_manifold, uint64_t category_bits) {
	bool isSet = daabbcc::SetTreeGroup(group_id);
	if (!isSet) {
		ERR_FAIL_V_MSG(Array(), "daabbcc.query_aabb(): invalid group id");
	}

	daabbcc::QueryAABB(position.x, position.y, size.width, size.height, category_bits, is_manifold);

	uint32_t queryResultSize = 0;
	if (!is_manifold) {
		queryResultSize = daabbcc::GetQueryResultSize();
	} else {
		queryResultSize = daabbcc::GetQueryManifoldResultSize();
	}

	Array result;

	if (queryResultSize > 0) {
		if (!is_manifold) {
			//PackedInt32Array ids;
			dmArray<uint16_t> &queryResult = daabbcc::GetQueryResults();
			generate_results(queryResultSize, queryResult, result);

		} else {
			dmArray<daabbcc::ManifoldResult> &queryResult = daabbcc::GetQueryManifoldResults();
			generate_manifold_results(queryResultSize, queryResult, result);
		}
	}

	return result;
}

Array DaabbccExtension::query_id(uint8_t group_id, int32_t proxy_id, bool is_manifold, uint64_t category_bits) {
	bool isSet = daabbcc::SetTreeGroup(group_id);
	if (!isSet) {
		ERR_FAIL_V_MSG(Array(), "daabbcc.query_id(): invalid group id");
	}

	daabbcc::QueryID(proxy_id, category_bits, is_manifold);

	uint32_t queryResultSize = 0;
	if (!is_manifold) {
		queryResultSize = daabbcc::GetQueryResultSize();
	} else {
		queryResultSize = daabbcc::GetQueryManifoldResultSize();
	}

	Array result;

	if (queryResultSize > 0) {
		if (!is_manifold) {
			//PackedInt32Array ids;
			dmArray<uint16_t> &queryResult = daabbcc::GetQueryResults();
			generate_results(queryResultSize, queryResult, result);

		} else {
			dmArray<daabbcc::ManifoldResult> &queryResult = daabbcc::GetQueryManifoldResults();
			generate_manifold_results(queryResultSize, queryResult, result);
		}
	}

	return result;
}

Array DaabbccExtension::query_aabb_sort(uint8_t group_id, Vector2 position, Vector2 size, bool is_manifold, uint64_t category_bits) {
	bool isSet = daabbcc::SetTreeGroup(group_id);
	if (!isSet) {
		ERR_FAIL_V_MSG(Array(), "daabbcc.query_aabb_sort(): invalid group id");
	}

	daabbcc::QueryAABBSort(position.x, position.y, size.width, size.height, category_bits, is_manifold);

	uint32_t queryResultSize = daabbcc::GetQueryManifoldResultSize();

	Array result;

	if (queryResultSize > 0) {
		dmArray<daabbcc::ManifoldResult> &queryResult = daabbcc::GetQueryManifoldResults();
		if (!is_manifold) {
			generate_short_results(queryResultSize, queryResult, result);
		} else {
			generate_manifold_results(queryResultSize, queryResult, result);
		}
	}

	return result;
}

Array DaabbccExtension::query_id_sort(uint8_t group_id, int32_t proxy_id, bool is_manifold, uint64_t category_bits) {
	bool isSet = daabbcc::SetTreeGroup(group_id);
	if (!isSet) {
		ERR_FAIL_V_MSG(Array(), "daabbcc.query_id_sort(): invalid group id");
	}

	daabbcc::QueryIDSort(proxy_id, category_bits, is_manifold);

	uint32_t queryResultSize = daabbcc::GetQueryManifoldResultSize();

	Array result;

	if (queryResultSize > 0) {
		if (!is_manifold) {
			//PackedInt32Array ids;
			dmArray<daabbcc::ManifoldResult> &queryResult = daabbcc::GetQueryManifoldResults();
			generate_short_results(queryResultSize, queryResult, result);

		} else {
			dmArray<daabbcc::ManifoldResult> &queryResult = daabbcc::GetQueryManifoldResults();
			generate_manifold_results(queryResultSize, queryResult, result);
		}
	}

	return result;
}

// ===========================================
// Raycast Operations
// ===========================================

Array DaabbccExtension::raycast(uint8_t group_id, Vector2 start_position, Vector2 end_position, bool is_manifold, uint64_t category_bits) {
	bool isSet = daabbcc::SetTreeGroup(group_id);
	if (!isSet) {
		ERR_FAIL_V_MSG(Array(), "daabbcc.raycast(): invalid group id");
	}

	daabbcc::RayCast(start_position.x, start_position.y, end_position.x, end_position.y, category_bits, is_manifold);

	uint32_t queryResultSize = 0;
	if (!is_manifold) {
		queryResultSize = daabbcc::GetQueryResultSize();
	} else {
		queryResultSize = daabbcc::GetQueryManifoldResultSize();
	}

	Array result;

	if (queryResultSize > 0) {
		if (!is_manifold) {
			//PackedInt32Array ids;
			dmArray<uint16_t> &queryResult = daabbcc::GetQueryResults();
			generate_results(queryResultSize, queryResult, result);

		} else {
			dmArray<daabbcc::ManifoldResult> &queryResult = daabbcc::GetQueryManifoldResults();
			generate_manifold_results(queryResultSize, queryResult, result);
		}
	}

	return result;
}

Array DaabbccExtension::raycast_sort(uint8_t group_id, Vector2 start_position, Vector2 end_position, bool is_manifold, uint64_t category_bits) {
	bool isSet = daabbcc::SetTreeGroup(group_id);
	if (!isSet) {
		ERR_FAIL_V_MSG(Array(), "daabbcc.raycast(): invalid group id");
	}

	daabbcc::RayCastSort(start_position.x, start_position.y, end_position.x, end_position.y, category_bits, is_manifold);

	uint32_t queryResultSize = daabbcc::GetQueryManifoldResultSize();

	Array result;

	if (queryResultSize > 0) {
		if (!is_manifold) {
			//PackedInt32Array ids;
			dmArray<daabbcc::ManifoldResult> &queryResult = daabbcc::GetQueryManifoldResults();
			generate_short_results(queryResultSize, queryResult, result);

		} else {
			dmArray<daabbcc::ManifoldResult> &queryResult = daabbcc::GetQueryManifoldResults();
			generate_manifold_results(queryResultSize, queryResult, result);
		}
	}

	return result;
}

// ===========================================
// Tree Operations
// ===========================================

void DaabbccExtension::rebuild(uint8_t group_id, bool full_build) {
	bool isSet = daabbcc::SetTreeGroup(group_id);
	if (!isSet) {
		ERR_FAIL_MSG("daabbcc.rebuild(): invalid group id");
	}
	daabbcc::Rebuild(group_id, full_build);
}

void DaabbccExtension::rebuild_all(bool full_build) {
	daabbcc::RebuildAll(full_build);
}
void DaabbccExtension::reset() {
	daabbcc::Reset();
}
