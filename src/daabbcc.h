#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"

using namespace godot;

class DaabbccClass : public RefCounted {
	GDCLASS(DaabbccClass, RefCounted)

protected:
	static void _bind_methods();

public:
	DaabbccClass() = default;
	~DaabbccClass() override = default;

	void print_type(const Variant &p_variant) const;
};