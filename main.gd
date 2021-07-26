extends Node

const mKinectClass = preload("res://bin/kinect.gdns")
var mKinect
var _screensize:Vector2		= Vector2.ZERO

onready var SkeletonPoints = $Skeleton

func _ready():
	#Centramos el juego en pantalla
	OS.center_window()
	
	#Obtenemos la resolucion de la pantalla
	_screensize = get_viewport().get_visible_rect().size
	
	#Creamos la instancia del controlador de kinect
	mKinect = mKinectClass.new()
	
	#Establecemos la resolucion de la pantalla
	mKinect.screen_width = _screensize.x
	mKinect.screen_height = _screensize.y
	
	#Inicializamos el sensor
	mKinect.initialize_sensor(0)
	
	#Obtenemos el grado de elevacion actual
	$HSlider.value = mKinect.get_elevation()

func _exit_tree():
	#Liberamos el sensor
	mKinect.release_sensor()

func _process(delta):
	var mSkeletonFrame:Array = mKinect.get_skeleton_frame()
	
	#Comprobamos que se haya obtenido algun esqueleto
	if (mSkeletonFrame.size() > 0):
		#Recorremos todos los puntos del esqueleto
		for idx_skeleton_point in range(SkeletonPoints.get_child_count()):
			var skeleton_point = SkeletonPoints.get_child(idx_skeleton_point)
			
			#Mandamos a procesar el punto del esqueleto
			skeleton_point.proc_skeleton_point(mSkeletonFrame[0])

func _on_btnSetElevation_pressed():
	#Establecemos el nuevo angulo de elevacion
	mKinect.set_elevation($HSlider.value)
