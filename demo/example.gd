extends Node

func _ready() -> void:
	var collision_bits = {
		PLAYER  = 1,     # (1 << 0)
		ENEMY   = 1 << 1,
		GROUND  = 1 << 2,
		ITEM    = 1 << 3,
		WALL    = 1 << 4,
		POINTER = 1 << 5,
		BUTTON  = 1 << 6,
		ALL     = ~0     # all bits set
	}

	var default_mask_bits  = collision_bits.PLAYER | collision_bits.ENEMY | collision_bits.GROUND | collision_bits.ITEM | collision_bits.WALL
	
	var daabbcc := DaabbccExtension.new()
	daabbcc.init(1, 16, 16)
	var group_id = daabbcc.new_group(daabbcc.UPDATE_PARTIALREBUILD)
	print("group_id: ", group_id)
	var PLAYER_aabb_id = daabbcc.insert_aabb(group_id, Vector2(100, 100), Vector2(100, 100),default_mask_bits)
	print("PLAYER_aabb_id: ", PLAYER_aabb_id)
	var ENEMY_aabb_id = daabbcc.insert_aabb(group_id, Vector2(200, 200), Vector2(100, 100),collision_bits.ENEMY)
	print("ENEMY_aabb_id: ", ENEMY_aabb_id)
	
	var ids = []
	var contact_points = []
	var normals = []
	var depths = []
	var distances = []
	var result = []
	
	result = daabbcc.query_aabb(group_id, Vector2(520, 520), Vector2(100, 100), true)

	print("query_aabb ========================================== ")
	if result.size() != 0:
		ids = result[0]
		contact_points = result[1]
		normals = result[2]
		depths = result[3]
		distances = result[4]
		
		for i in range(ids.size()):
			print("ID:", ids[i])
			print("Contact:", contact_points[i])
			print("Normal:", normals[i])
			print("Depth:", depths[i])
			print("Distance:", distances[i])

	result = daabbcc.query_id(group_id, ENEMY_aabb_id, true)
	
	if result.size() != 0:
		ids = result[0]
		contact_points = result[1]
		normals = result[2]
		depths = result[3]
		distances = result[4]

	print("query_id ========================================== ")
	for i in range(ids.size()):
		print("ID:", ids[i])
		print("Contact:", contact_points[i])
		print("Normal:", normals[i])
		print("Depth:", depths[i])
		print("Distance:", distances[i])

	result = daabbcc.query_aabb_sort(group_id, Vector2(20, 20), Vector2(100, 100), true)
	
	if result.size() != 0:
		ids = result[0]
		contact_points = result[1]
		normals = result[2]
		depths = result[3]
		distances = result[4]

	print("query_aabb_sort ========================================== ")
	for i in range(ids.size()):
		print("ID:", ids[i])
		print("Contact:", contact_points[i])
		print("Normal:", normals[i])
		print("Depth:", depths[i])
		print("Distance:", distances[i])

	result = daabbcc.query_id_sort(group_id, ENEMY_aabb_id, true)
	
	if result.size() != 0:
		ids = result[0]
		contact_points = result[1]
		normals = result[2]
		depths = result[3]
		distances = result[4]

	print("query_id_sort ========================================== ")
	for i in range(ids.size()):
		print("ID:", ids[i])
		print("Contact:", contact_points[i])
		print("Normal:", normals[i])
		print("Depth:", depths[i])
		print("Distance:", distances[i])
	
	result = daabbcc.raycast(group_id, Vector2(0,0), Vector2(200,200))
	if result.size() != 0:
		ids = result[0]

	print("raycast ========================================== ")
	for i in range(ids.size()):
		print("ID:", ids[i])
		
	result = daabbcc.raycast_sort(group_id, Vector2(0,0), Vector2(200,200))
	if result.size() != 0:
		ids = result[0]
		distances = result[1]

	print("raycast ========================================== ")
	for i in range(ids.size()):
		print("ID:", ids[i])
		print("Distance:", distances[i])
