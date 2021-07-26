#ifndef KINECT_H
#define KINECT_H

#include <Godot.hpp>
#include <Reference.hpp>
#include <core/Ref.hpp>

#include <Windows.h>
#include <NuiApi.h>
#include <NuiSensor.h>
#include <NuiSkeleton.h>

namespace godot
{
	class Kinect : public Reference
	{
		GODOT_CLASS(Kinect, Reference)

		static const int cScreenWidth  = 320;
		static const int cScreenHeight = 240;

		private:
			//Variables privadas
			INuiSensor *mSensor;

			//Funciones privadas
			godot::Dictionary ConvertSkeletonDataToDictionary(const NUI_SKELETON_DATA &pData);
			godot::Dictionary ConvertVector4ToDictionary(const Vector4 &pVector);
			godot::Vector3 SkeletonPointToScreen(const Vector4 &pVector);

		public:
			//Variables publicas
			int ScreenWidth;
			int ScreenHeight;

			//Funciones de godot
			static void _register_methods();
			void _init();

			//Funciones para controlar el sensor de Kinect
			bool InitializeSensor(int pIdxSensor);
			int GetSensorCount();
			void ReleaseSensor();
			bool IsSensorInitialized();
			int GetElevation();
			bool SetElevation(int pValue);
			godot::Array GetSkeletonFrame();
	};
}

#endif