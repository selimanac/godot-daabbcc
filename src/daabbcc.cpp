#include "daabbcc.h"

void DaabbccClass::_bind_methods() {
	godot::ClassDB::bind_method(D_METHOD("print_type", "DaabbccClass"), &DaabbccClass::print_type);
}

void DaabbccClass::print_type(const Variant &p_variant) const {
	print_line(vformat("Type: %d", p_variant.get_type()));
}
