extends Node2D

const ITEM_SCENE = preload("res://dot.tscn")  # root of this scene should be Node2D

var instances : Array = []

func spawn_many(count: int) -> void:
	for i in range(count):
		var inst = ITEM_SCENE.instantiate() as Node2D
		inst.position = Vector2(100 * (i % 10), 100 * (i / 10))
		add_child(inst)
		instances.append(inst)   # index-based access later: instances[0], instances[1], ...


func _ready() -> void:
	spawn_many(10)
