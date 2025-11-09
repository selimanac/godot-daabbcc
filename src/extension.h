#pragma once

#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/core/binder_common.hpp"
#include <godot_cpp/classes/object.hpp>

#include <daabbcc/daabbcc.h>

using namespace godot;

VARIANT_ENUM_CAST(daabbcc::TreeBuildType);

class DaabbccExtension : public Object {
	GDCLASS(DaabbccExtension, Object)

protected:
	static const uint64_t CATEGORY_ALL;
	static void _bind_methods();

	static void generate_results(uint32_t queryResultSize, dmArray<uint16_t> &queryResult, Array &result);
	static void generate_short_results(uint32_t queryResultSize, dmArray<daabbcc::ManifoldResult> &queryResult, Array &result);
	static void generate_manifold_results(uint32_t queryResultSize, dmArray<daabbcc::ManifoldResult> &queryResult, Array &result);

public:
	DaabbccExtension() = default;
	~DaabbccExtension() override = default;

	void init(uint8_t max_group_count, uint16_t max_gameobject_count, uint16_t max_query_count);

	// ===========================================
	// Group Operations
	// ===========================================
	uint8_t new_group(uint8_t tree_build_type = daabbcc::UPDATE_INCREMENTAL);
	void remove_group(uint8_t group_id);

	// ===========================================
	// Proxy Operations
	// ===========================================
	int32_t insert_aabb(uint8_t group_id, Vector2 position, Vector2 size, uint64_t category_bits = CATEGORY_ALL);
	void update_aabb(uint8_t group_id, int32_t proxy_id, Vector2 position, Vector2 size);
	void remove(uint8_t group_id, int32_t proxy_id);

	// ===========================================
	// Query Operations
	// ===========================================
	Array query_aabb(uint8_t group_id, Vector2 position, Vector2 size, bool is_manifold = false, uint64_t category_bits = CATEGORY_ALL);
	Array query_id(uint8_t group_id, int32_t proxy_id, bool is_manifold = false, uint64_t category_bits = CATEGORY_ALL);
	Array query_aabb_sort(uint8_t group_id, Vector2 position, Vector2 size, bool is_manifold = false, uint64_t category_bits = CATEGORY_ALL);
	Array query_id_sort(uint8_t group_id, int32_t proxy_id, bool is_manifold = false, uint64_t category_bits = CATEGORY_ALL);

	// ===========================================
	// Raycast Operations
	// ===========================================
	Array raycast(uint8_t group_id, Vector2 start_position, Vector2 end_position, bool is_manifold = false, uint64_t category_bits = CATEGORY_ALL);
	Array raycast_sort(uint8_t group_id, Vector2 start_position, Vector2 end_position, bool is_manifold = false, uint64_t category_bits = CATEGORY_ALL);

	// ===========================================
	// Tree Operations
	// ===========================================
	void rebuild(uint8_t group_id, bool full_build = true);
	void rebuild_all(bool full_build = true);
	void reset();
};