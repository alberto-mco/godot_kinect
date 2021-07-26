extends Area2D

enum SkeletonPoints { HIP_CENTER, SPINE, SHOULDER_CENTER, HEAD, SHOULDER_LEFT, ELBOW_LEFT, WRIST_LEFT, HAND_LEFT, SHOULDER_RIGHT, ELBOW_RIGHT, WRIST_RIGHT, HAND_RIGHT, HIP_LEFT, KNEE_LEFT, ANKLE_LEFT, FOOT_LEFT, HIP_RIGHT, KNEE_RIGHT, ANKLE_RIGHT, FOOT_RIGHT, COUNT}
enum SkeletonPointTrackedStates { NOT_TRACKED, INFERRED, TRACKED}

export(SkeletonPoints) var SkeletonPoint = SkeletonPoints.HIP_CENTER

var tracked_textures = { SkeletonPointTrackedStates.NOT_TRACKED : "res://assets/red_point.png", SkeletonPointTrackedStates.TRACKED : "res://assets/green_point.png", SkeletonPointTrackedStates.INFERRED : "res://assets/red_point.png" }

onready var TrackedState = $Sprite

func proc_skeleton_point(pSkeletonBody:Dictionary):
	var _skeleton_point:Dictionary = pSkeletonBody["body"][SkeletonPoint]
	
	TrackedState.texture = load(tracked_textures[_skeleton_point["tracking_state"]])
	global_position = Vector2(_skeleton_point["x"], _skeleton_point["y"])
