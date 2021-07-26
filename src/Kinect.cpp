#include "Kinect.hpp"

using namespace godot;

//******************************************************************
//Convierte una estructura NUI_SKELETON_DATA a un diccionario de godot
//******************************************************************
Dictionary Kinect::ConvertSkeletonDataToDictionary(const NUI_SKELETON_DATA &pData)
{
    Dictionary mInfo;

    try
    {
        mInfo["user_index"] = static_cast<long>(pData.dwUserIndex);
        mInfo["tracking_state"] = static_cast<int>(pData.eTrackingState);
        mInfo["tracking_id"] = static_cast<long>(pData.dwTrackingID);
        mInfo["enrollment_index"] = static_cast<long>(pData.dwEnrollmentIndex);
        mInfo["position"] = ConvertVector4ToDictionary(pData.Position);
        mInfo["quality_flags"] = static_cast<long>(pData.dwQualityFlags);

        Dictionary mBody;

        //Recorremos todas las partes del cuerpo
        for (int i = 0; i < 20; i++)
        {
            Dictionary mPart = ConvertVector4ToDictionary(pData.SkeletonPositions[i]);
            mPart["tracking_state"] = static_cast<int>(pData.eSkeletonPositionTrackingState[i]);
            mBody[i] = mPart;
        }

        mInfo["body"] = mBody;
    }
    catch (std::exception e)
    {
        Godot::print_error(e.what(), "ConvertSkeletonDataToDictionary", __FILE__, __LINE__);
    }

    return mInfo;
}

//******************************************************************
//Convierte un vector en un diccionario
//******************************************************************
Dictionary Kinect::ConvertVector4ToDictionary(const Vector4 &pVector)
{
    Dictionary mInfo;

    try
    {
        Vector3 mRealPosition = SkeletonPointToScreen(pVector);

        mInfo["x"] = mRealPosition.x;
        mInfo["y"] = mRealPosition.y;
        mInfo["z"] = mRealPosition.z;
        mInfo["w"] = pVector.w;
    }
    catch (std::exception e)
    {
        Godot::print_error(e.what(), "ConvertVector4ToDictionary", __FILE__, __LINE__);
    }

    return mInfo;
}

//******************************************************************
//Convierte las coordenadas del punto del esqueleto al espacio de la ventana
//******************************************************************
godot::Vector3 Kinect::SkeletonPointToScreen(const Vector4 &pVector)
{
    Vector3 vResult;

    try
    {
        //Comprobamos que haya un tamaño de pantalla definido
        if ((ScreenWidth > 0) && (ScreenHeight > 0))
        {
            long x = 0;
            long y = 0;
            unsigned short depth;

            //Calculamos la posicion del esqueleto en pantalla
            //NuiTransformSkeletonToDepthImage retorna las coordenadas en NUI_IMAGE_RESOLUTION_320x240
            NuiTransformSkeletonToDepthImage(pVector, &x, &y, &depth);

            vResult.x = static_cast<float>(x * ScreenWidth) / cScreenWidth;
            vResult.y = static_cast<float>(y * ScreenHeight) / cScreenHeight;
        }
        else
        {
            //Copiamos los mismos valores
            vResult.x = pVector.x;
            vResult.y = pVector.y;
        }

        vResult.z = pVector.z;
    }
    catch (std::exception e)
    {
        Godot::print_error(e.what(), "SkeletonPointToScreen", __FILE__, __LINE__);
    }

    return vResult;
}

//******************************************************************
//Constructor
//******************************************************************
void Kinect::_init()
{
    try
    {
        //Inicializamos las variables
        mSensor = nullptr;
        ScreenWidth = -1;
        ScreenHeight = -1;
    }
    catch (std::exception e)
    {
        Godot::print_error(e.what(), "_init", __FILE__, __LINE__);
    }
}

//******************************************************************
//Registramos los metodos de la clase
//******************************************************************
void Kinect::_register_methods()
{
    try
    {
        //Registramos las variables que seran visibles desde Godot
        register_property<Kinect, int>("screen_height", &Kinect::ScreenHeight, -1);
        register_property<Kinect, int>("screen_width", &Kinect::ScreenWidth, -1);

        //Registramos los metodos que seran visibles desde Godot
        register_method("initialize_sensor", &Kinect::InitializeSensor);
        register_method("get_sensor_count", &Kinect::GetSensorCount);
        register_method("release_sensor", &Kinect::ReleaseSensor);
        register_method("is_sensor_initialized", &Kinect::IsSensorInitialized);
        register_method("get_elevation", &Kinect::GetElevation);
        register_method("set_elevation", &Kinect::SetElevation);
        register_method("get_skeleton_frame", &Kinect::GetSkeletonFrame);
    }
    catch (std::exception e)
    {
        Godot::print_error(e.what(), "_register_methods", __FILE__, __LINE__);
    }
}

//******************************************************************
//Inicializa el sensor indicado
//******************************************************************
bool Kinect::InitializeSensor(int pIdxSensor)
{
    bool bResult = false;

    try
    {
        //Comprobamos que el indice del sensor no sea un valor negativo
        if (pIdxSensor >= 0)
        {
            //Obtenemos el total de sensores detectados
            int mTotalSensor = GetSensorCount();

            //Comprobamos que el indice del sensor que se quiere inicializar no este fuera de rango
            if (mTotalSensor > pIdxSensor)
            {
                //Liberamos el sensor que hubiera abierto antes
                ReleaseSensor();

                //Creamos la instancia del sensor y comprobamos el resultado de la operacion
                if (SUCCEEDED(NuiCreateSensorByIndex(pIdxSensor, &mSensor)))
                {
                    //Obtenemos el estado del sensor
                    if (S_OK == mSensor->NuiStatus())
                    {
                        //Inicializamos el sensor y establecemos el resultado de la operacion
                        bResult = SUCCEEDED(mSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON));
                    }
                    else
                    {
                        mSensor->Release();
                        mSensor = nullptr;
                    }
                }
            }
        }
    }
    catch (std::exception e)
    {
        Godot::print_error(e.what(), "InitializeSensor", __FILE__, __LINE__);
    }
    
    return bResult;
}

//******************************************************************
//Obtiene el total de sensores detectados
//******************************************************************
int Kinect::GetSensorCount()
{
    int iResult = -1;

    try
    {
        //Obtenemos el total de sensores detectados
        HRESULT hr = NuiGetSensorCount(&iResult);

        //Comprobamos el resultado de la operacion
        if (FAILED(hr))
        {
            iResult = -1;
        }
    }
    catch (std::exception e)
    {
        Godot::print_error(e.what(), "GetSensorCount", __FILE__, __LINE__);
    }

    return iResult;
}

//******************************************************************
//Libera el sensor que este activo
//******************************************************************
void Kinect::ReleaseSensor()
{
    try
    {
        //Comprobamos que haya una instancia del sensor activa
        if (IsSensorInitialized())
        {
            //Liberamos el sensor
            mSensor->NuiShutdown();

            //Apuntamos la variable a null
            mSensor = nullptr;
        }
    }
    catch (std::exception e)
    {
        Godot::print_error(e.what(), "ReleaseSensor", __FILE__, __LINE__);
    }
}

//******************************************************************
//Obtiene si hay un sensor inicializado o no
//******************************************************************
bool Kinect::IsSensorInitialized()
{
    return (mSensor != nullptr);
}

//******************************************************************
//Obtiene el angulo de elevacion del sensor
//******************************************************************
int Kinect::GetElevation()
{
    long iResult = 0xFF;

    try
    {
        //Comprobamos que haya un sensor inicializado
        if (IsSensorInitialized())
        {
            //Obtenemos el angulo del sensor y comprobamos el resultado de la operacion
            if (FAILED(mSensor->NuiCameraElevationGetAngle(&iResult)))
            {
                //Indicamos que ha habido un error
                iResult = 0xFF;
            }
        }
    }
    catch (std::exception e)
    {
        Godot::print_error(e.what(), "GetElevation", __FILE__, __LINE__);
    }

    return static_cast<int>(iResult);
}

//******************************************************************
//Establece el angulo de elevacion del sensor
//******************************************************************
bool Kinect::SetElevation(int pValue) 
{
    bool bResult = false;

    try
    {
        //Comprobamos que haya un sensor inicializado
        if (IsSensorInitialized())
        {
            //Comprobamos que el valor del angulo este dentro del rango permitido
            if (pValue > 27) { pValue = 27; }
            if (pValue < -27) { pValue = -27; }

            //Establecemos el angulo del sensor y establecemos el resultado de la operacion
            bResult = SUCCEEDED(mSensor->NuiCameraElevationSetAngle(pValue));
        }
    }
    catch (std::exception e)
    {
        Godot::print_error(e.what(), "SetElevation", __FILE__, __LINE__);
    }

    return bResult;
}

//******************************************************************
//Obtiene un frame con todos los esqueletos capturados
//******************************************************************
Array Kinect::GetSkeletonFrame()
{
    Array mFrame;

    try
    {
        //Comprobamos que haya un sensor inicializado
        if (IsSensorInitialized())
        {
            NUI_SKELETON_FRAME mSkeletonFrame;

            //Inicializamos la estructura
            memset(&mSkeletonFrame, 0, sizeof(NUI_SKELETON_FRAME));

            //Obtenemos la informacion con todos los esqueletos que se han detectado
            if (SUCCEEDED(mSensor->NuiSkeletonGetNextFrame(0, &mSkeletonFrame)))
            {
                //Suavizamos los movimientos, ya que la deteccion puede tener mucho "ruido"
                mSensor->NuiTransformSmooth(&mSkeletonFrame, NULL);

                //Recorremos todos los esqueletos
                for (int z = 0; z < NUI_SKELETON_COUNT; ++z)
                {
                    //Obtenemos la informacion del esqueleto
                    const NUI_SKELETON_DATA& mSkeleton = mSkeletonFrame.SkeletonData[z];

                    //Comprobamos que sea un esqueleto valido
                    if (mSkeleton.eTrackingState == NUI_SKELETON_TRACKED)
                    {
                        Dictionary mBody = ConvertSkeletonDataToDictionary(mSkeleton);
                        
                        //Insertamos el cuerpo en la lista
                        mFrame.push_back(mBody);
                    }
                }
            }
        }
    }
    catch (std::exception e)
    {
        Godot::print_error(e.what(), "GetSkeletonFrame", __FILE__, __LINE__);
    }

    return mFrame;
}