extends Node2D

var player: Node2D
var enemy: Node2D
var block: Node2D
var sprite_size = Vector2i(40, 40)
var pos_a := Vector2(100, 100)
var pos_b := Vector2(500, 400)
var duration := 2.0  # seconds to go from A -> B
var time := 0.0
var daabbcc := DaabbccExtension.new()
var group_id: int
var PLAYER_aabb_id: int
var ENEMY_aabb_id: int
var BLOCK_aabb_id: int

var ids = []
var contact_points = []
var normals = []
var depths = []
var distances = []
var result = []
var collision_bits = {
		PLAYER  = 1,     # (1 << 0)
		ENEMY   = 1 << 1,
		GROUND  = 1 << 2,
		BLOCK    = 1 << 3,
		WALL    = 1 << 4,
		POINTER = 1 << 5,
		BUTTON  = 1 << 6,
		ALL     = ~0     # all bits set
	}

var mouse_world = Vector2()
var mouse_screen = Vector2()

func _ready() -> void:
	
	player = $player as Node2D
	enemy = $enemy as Node2D
	block = $block as Node2D
	
	#var world_pos = player.global_position
	# var player = get_node("Player") as Node2D
	# var enemy = get_node("SomeWrapper/Enemy") as Node2D
	#var players = get_tree().get_nodes_in_group("players")  # Array of nodes
	#var player = players.sprite_size() > 0 ? players[0] as Node2D : null


	var default_mask_bits  = collision_bits.PLAYER | collision_bits.ENEMY | collision_bits.GROUND | collision_bits.BLOCK | collision_bits.WALL
	
	
	daabbcc.init(1, 16, 16)
	group_id = daabbcc.new_group(daabbcc.UPDATE_PARTIALREBUILD)
	print("group_id: ", group_id)

	PLAYER_aabb_id = daabbcc.insert_node2d(group_id, player, sprite_size,false, collision_bits.PLAYER)
	print("PLAYER_aabb_id: ", PLAYER_aabb_id)

	ENEMY_aabb_id = daabbcc.insert_aabb(group_id, enemy.position, sprite_size,collision_bits.ENEMY)
	print("ENEMY_aabb_id: ", ENEMY_aabb_id)

	BLOCK_aabb_id = daabbcc.insert_node2d(group_id,block,sprite_size, false, collision_bits.BLOCK)
	print("BLOCK_aabb_id: ", BLOCK_aabb_id)



	enemy.position = Vector2(600, 300)
	daabbcc.update_aabb(group_id,ENEMY_aabb_id, enemy.position, sprite_size)
	
	
	result = daabbcc.query_aabb(group_id, Vector2(520, 520), Vector2(100, 100), true)
	print()
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
		
	result = daabbcc.raycast_sort(group_id, Vector2(0,0), Vector2(300,300))
	if result.size() != 0:
		ids = result[0]
		distances = result[1]

	print("raycast_sort ========================================== ")
	for i in range(ids.size()):
		print("ID:", ids[i])
		print("Distance:", distances[i])


func _process(delta):
	queue_redraw()
	mouse_screen = get_viewport().get_mouse_position()   # screen/viewport pixels
	mouse_world  = get_global_mouse_position()           # world (global) position in 2D
	#print("screen:", mouse_screen, "world:", mouse_world)

	if player == null:
		return

	time += delta
	var cycle = fmod(time, duration * 2)         # 0 .. (2*duration)
	var t = cycle
	if t > duration:
		t = duration * 2 - t                     # ping-pong
	var alpha = t / duration                     # 0..1 back and forth
	player.position = pos_a.lerp(pos_b, alpha)
	#daabbcc.update_aabb(group_id,PLAYER_aabb_id, player.position, sprite_size)
	"""
	result = daabbcc.query_id(group_id, ENEMY_aabb_id, true)
		
	if result.size() != 0:
		print("query_id ========================================== ")
		ids = result[0]
		contact_points = result[1]
		normals = result[2]
		depths = result[3]
		distances = result[4]

		for i in range(ids.sprite_size()):
			print("ID:", ids[i])
			print("Contact:", contact_points[i])
			print("Normal:", normals[i])
			print("Depth:", depths[i])
			print("Distance:", distances[i])
	"""

	
	result = daabbcc.raycast(group_id, Vector2(0,0), mouse_world,false,collision_bits.PLAYER)
	if result.size() != 0:
		print("raycast ========================================== ")
		ids = result[0]

		for i in range(ids.size()):
			print("ID:", ids[i])
	else:
		print("NOPE")
	

	
	daabbcc.update_aabb(group_id,BLOCK_aabb_id, mouse_world, sprite_size)

	result = daabbcc.query_id(group_id, BLOCK_aabb_id, true, collision_bits.ENEMY)
	if result.size() != 0:
		#print("query_id ========================================== ")
		ids = result[0]
		contact_points = result[1]
		normals = result[2]
		depths = result[3]
		distances = result[4]

		for i in range(ids.size()):
			#print("ID:", ids[i])
			#print("Contact:", contact_points[i])
			#print("Normal:", normals[i])
			#print("Depth:", depths[i])
			#print("Distance:", distances[i])
			var offset = normals[i] * depths[i]
			mouse_world = mouse_world + offset

	block.position = mouse_world 


func _draw():
	draw_line(Vector2(0,0), mouse_world, Color.GREEN, 1.0)
