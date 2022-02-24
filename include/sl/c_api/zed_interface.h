#ifndef __INTERFACE_ZED_H
#define __INTERFACE_ZED_H

#include "types_c.h"
#include <stdbool.h>
/**
 * @file
 * */

//#define DEBUG
#ifdef _WIN32
#ifdef INTERFACE_NOEXPORT
#define INTERFACE_API
#else
#ifdef INTERFACE_EXPORT
#define INTERFACE_API __declspec(dllexport)
#else
#define INTERFACE_API __declspec(dllimport)
#endif
#endif
#else
#define INTERFACE_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

    /**
    \brief Forces unload of all instances.
    */
    INTERFACE_API void sl_unload_all_instances();

    /**
    \brief Forces unload of one instance.
    \param camera_id : id of the instance to unload.
    */
    INTERFACE_API void sl_unload_instance(int camera_id);

    /**
    \brief Checks usb devices connected.
    *param device : type of device to find.
    \return true if connected.
    */
    INTERFACE_API bool sl_find_usb_device(enum USB_DEVICE device);

    /**
    * \brief Creates a camera with resolution mode, fps and id for linux.
    * \param camera_id : id of the camera to be added.
    * \param verbose : Enable verbose mode.
    * \return true if the camera has been created successfully.
    */
    INTERFACE_API bool sl_create_camera(int camera_id);

	/**
	\brief Reports if the camera has been successfully opened.
	\param camera_id : id of the camera.
	\return true if the ZED is already setup, otherwise false.
	*/
	INTERFACE_API bool sl_is_opened(int camera_id);

    /**
    \brief Opens the camera depending on the init parameters.
    \param camera_id : id of the camera.
    \param init_parameters : structure containing all the initial parameters.
    \param path_svo : filename of the svo (for SVO input).
    \param ip : ip of the camera to open (for Stream input).
    \param stream_port : port of the camera to open (for Stream input).
    \param output_file : sdk verbose log file. Redirect the SDK verbose message to file.
    \param opt_settings_path : optional settings path. Equivalent to  \ref InitParameters::optional_settings_path.
    \param opencv_calib_path : optional openCV calibration file. Equivalent to  \ref InitParameters::optional_opencv_calibration_file.
    \return An error code giving information about the internal process. If SUCCESS (0) is returned, the camera is ready to use. Every other code indicates an error and the program should be stopped.
    */
    INTERFACE_API int sl_open_camera(int camera_id, struct SL_InitParameters *init_parameters, const char* path_svo, const char* ip, int stream_port, const char* output_file, const char* opt_settings_path, const char* opencv_calib_path);

    /**
    \brief Returns the initparameters used to open the ZED camera
    \param camera_id : id of the camera instance.
    */
    INTERFACE_API struct SL_InitParameters* sl_get_init_parameters(int camera_id);

    /**
    \brief Returns the Runtimeparameters used to open the ZED camera
    \param camera_id : id of the camera instance.
    \return a structure containing all the runtime parameters.
    */
    INTERFACE_API struct SL_RuntimeParameters* sl_get_runtime_parameters(int camera_id);

    /**
    \brief Returns the PositionalTrackingParameters
    \param camera_id : id of the camera instance.
    \return a structure containing all the positional tracking parameters
    */
    INTERFACE_API struct SL_PositionalTrackingParameters* sl_get_positional_tracking_parameters(int camera_id);

    /**
    \brief Destroys the camera and disable the textures.
    \param camera_id : id of the camera instance.
     */
    INTERFACE_API void sl_close_camera(int camera_id);
    /**
    \brief Grabs the lastest images from the camera.
    \param camera_id : id of the camera instance.
    \param runtime : structure containing all the runtime parameters.
    \return An error code giving information about the internal process."SUCCESS" if the method succeeded.
     */
    INTERFACE_API int sl_grab(int camera_id, struct SL_RuntimeParameters *runtime);

    /**
    \brief Lists all the connected devices with their associated information.

    This function lists all the cameras available and provides their serial number, models and other information.
    \param device_list [Out] : the devices properties for each connect camera.
    \param nb_devices : the number of cameras connected.
     */
    INTERFACE_API void sl_get_device_list(struct SL_DeviceProperties device_list[MAX_CAMERA_PLUGIN], int* nb_devices);

    /**
    \brief Performs an hardware reset of the ZED 2 / ZED 2i.
    \param sn : serial number of the camera to reset, or 0 to reset the first camera detected.
	\param fullReboot : Perform a full reboot (Sensors and Video modules)
     */
    INTERFACE_API int sl_reboot(int sn, bool full_reboot);

    /**
    \brief Creates a file for recording the ZED's output into a .SVO or .AVI video. An SVO is Stereolabs' own format designed for the ZED. It holds the video feed with timestamps as well as info about the camera used to record it.
    \param camera_id : id of the camera instance.
    \param filename : filename of the SVO file.
    \param compression_mode : compression mode. Can be one for the \ref SL_SVO_COMPRESSION_MODE enum.
    \param bitrate : overrides default bitrate of the SVO file, in KBits/s. Only works if \ref SVO_COMPRESSION_MODE is H264 or H265.
    \param target_fps : defines the target framerate for the recording module.
    \param transcode : in case of streaming input, if set to false, it will avoid decoding/re-encoding and convert directly streaming input to a SVO file.
               This saves a encoding session and can be especially useful on NVIDIA Geforce cards where the number of encoding session is limited.
    \return An ERROR_CODE that defines if SVO file was successfully created and can be filled with images.
     */
    INTERFACE_API int sl_enable_recording(int camera_id, const char* filename, enum SL_SVO_COMPRESSION_MODE compression_mode, unsigned int bitrate, int target_fps, bool transcode);

    /**
    \brief Disables the recording initiated by enableRecording() and closes the generated file.
    \param camera_id : id of the camera instance.
     */
    INTERFACE_API void sl_disable_recording(int camera_id);

	/**
	\brief Pauses or resumes the recording.
	\param camera_id : id of the camera instance.
	\param status : if true, the recording is paused. If false, the recording is resumed.
	 */
	INTERFACE_API void sl_pause_recording(int camera_id, bool status);

    /**
    \brief Initializes and starts the positional tracking processes.
    This function allows you to enable the position estimation of the SDK. It only has to be called once in the camera's lifetime.

    \param camera_id : id of the camera instance.
    \param area_file_path : area localization file that describes the surroundings, saved from a previous tracking session.
    \return \ref SL_ERROR_CODE::SUCCESS if everything went fine, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_enable_positional_tracking(int camera_id, struct SL_PositionalTrackingParameters * tracking_param, const char* area_file_path);

    /**
    \brief Initializes and starts the positional tracking processes.
    This function allows you to enable the position estimation of the SDK. It only has to be called once in the camera's lifetime.

    \param camera_id : id of the camera instance.
    \param initial_world_rotation : rotation of the camera in the world frame when the camera is started. By default, it should be identity.
    \param initial_world_position : position of the camera in the world frame when the camera is started. By default, it should be identity.
    \param enable_area_memory : this mode enables the camera to remember its surroundings. This helps correct positional tracking drift, and can be helpful for positioning different cameras relative to one other in space.
    \param enable_pose_smoothing : this mode enables smooth pose correction for small drift correction.
    \param set_floor_as_origin : this mode initializes the tracking to be aligned with the floor plane to better position the camera in space.
    \param set_as_static : this mode defines the camera as static. If true, it will not move in the environment. This allows you to set its position using initial_world_transform.
    \param enable_imu_fusion : this mode enables or disables IMU fusion. When set to false, only the optical odometry will be used.
    \param area_file_path : area localization file that describes the surroundings, saved from a previous tracking session.
    \return \ref SL_ERROR_CODE::SUCCESS if everything went fine, ERROR_CODE::FAILURE otherwise.
     */
    /**INTERFACE_API int enable_positional_tracking(int camera_id, struct SL_Quaternion *initial_world_rotation, struct SL_Vector3 *initial_world_position, bool enable_area_memory, bool enable_pose_smoothing, bool set_floor_as_origin, bool set_as_static,
            bool enable_imu_fusion, const char* area_file_path);*/
    /**
    \brief Disables the positional tracking.
    \param camera_id : id of the camera instance.
    \param area_file_path : if set, saves the spatial memory into an '.area' file.
     */
    INTERFACE_API void sl_disable_positional_tracking(int camera_id, const char *area_file_path);
	
    /**
    \brief Saves the current area learning file. The file will contain spatial memory data generated by the tracking.
    \param camera_id : id of the camera instance.
    \param area_file_path : save the spatial memory database in an '.area' file.
    \return \ref SL_ERROR_CODE::FAILURE if the area_file_path file wasn't found, SUCCESS otherwise.
     */
    INTERFACE_API int sl_save_area_map(int camera_id, const char* area_file_path);

    /**
    \brief Returns the state of the spatial memory export process.
    \param camera_id : id of the camera instance.
    \return The current state (SL_AREA_EXPORTING_STATE) of the spatial memory export process
     */
    INTERFACE_API int sl_get_area_export_state(int camera_id);

    /**
    \brief  Sets the playback cursor to the desired frame number in the SVO file.
    This function allows you to move around within a played-back SVO file. After calling, the next call to grab() will read the provided frame number.

    \param camera_id : id of the camera instance.
    \param frame_number : the number of the desired frame to be decoded.
     */
    INTERFACE_API void sl_set_svo_position(int camera_id, int frame_number);

    /**
    \brief Returns the camera FPS.
    \param camera_id : id of the camera instance.
    \return The current frame rate.
     */
    INTERFACE_API float sl_get_camera_fps(int camera_id);

	/**
	\brief Returns the current FPS.
	\param camera_id : id of the camera instance.
	\return The current frame rate.
		*/
	INTERFACE_API float sl_get_current_fps(int camera_id);

    /**
    \brief Returns the width of the current image.
    \param camera_id : id of the camera instance.
    \return Width of the image.
     */
    int INTERFACE_API sl_get_width(int camera_id);

    /**
    \brief Returns the height of the current image.
    \param camera_id : id of the camera instance.
    \return Height of the image.
     */
    int INTERFACE_API sl_get_height(int camera_id);

    /**
    \brief Gets the current confidence threshold value for the disparity map (and by extension the depth map).
    \param camera_id : id of the camera instance.
    \return The confidence threshold.
     */
    INTERFACE_API int sl_get_confidence_threshold(int camera_id);

    /**
    \brief Performs a new self calibration process.
    In some cases, due to temperature changes or strong vibrations, the stereo calibration becomes less accurate. Use this function to update the self-calibration data and get more reliable depth values.
    \param camera_id : id of the camera instance.
     */
    INTERFACE_API void sl_update_self_calibration(int camera_id);

    /**
    \brief Gets the Calibration Parameters.
    \param camera_id : id of the camera instance.
    \param raw_params : if true, returns Intrinsic and Extrinsic stereo parameters for original images (unrectified/distorded), else returns parameters for rectified/undistorded images.
    \return Structure containing Intrinsic and Extrinsic stereo parameters
     */
    INTERFACE_API struct SL_CalibrationParameters* sl_get_calibration_parameters(int camera_id, bool raw_params);

    /**
    \brief Gets the Sensors configuration.
    \param camera_id : id of the camera instance.
    \return Structure containing information about all the sensors available in the current device.
     */
    INTERFACE_API struct SL_SensorsConfiguration* sl_get_sensors_configuration(int camera_id);

    /**
    \brief Gets the IMU to Left camera transform matrix.
    \param camera_id : id of the camera instance.
    \param translation : translation between IMU frame and camera frame.
    \param rotation : rotation between IMU frame and camera frame.
     */
    INTERFACE_API void sl_get_camera_imu_transform(int camera_id, struct SL_Vector3 *translation, struct SL_Quaternion *rotation);

    /**
    \brief Gets the input type (see \ref SL_INPUT_TYPE).
    \param camera_id : id of the camera instance.
    \return The input type.
     */
    INTERFACE_API int sl_get_input_type(int camera_id);

    /**
    \brief Gets the ZED Serial Number.
    \param camera_id : id of the camera instance.
    \return The serial number of the camera.
     */
    INTERFACE_API int sl_get_zed_serial(int camera_id);

    /**
    \brief Gets the ZED camera Current Firmware version.
    \param camera_id : id of the camera instance.
    \return The firmware of the camera.
     */
    INTERFACE_API int sl_get_camera_firmware(int camera_id);

    /**
    \brief Gets the ZED mcu Current Firmware version.
    \param camera_id : id of the camera instance.
    \return The firmware version of the sensor module, 0 if no sensors are available.
     */
    INTERFACE_API int sl_get_sensors_firmware(int camera_id);

    /**
    \brief Gets the ZED Camera model (see \ref SL_MODEL).
    \param camera_id : id of the camera instance.
    \return The ZED Camera model.
     */
    INTERFACE_API int sl_get_camera_model(int camera_id);

    /**
    \brief Get the Timestamp at the time the frame has been extracted from USB stream. (should be called after a grab).
    \param camera_id : id of the camera instance.
    \return The Camera timestamp.
     */
    INTERFACE_API unsigned long long sl_get_image_timestamp(int camera_id);

    /**
    \brief Get the current Timestamp at the time the function is called. Can be compared to the camera Timestamp for synchronization.
    \param camera_id : id of the camera instance.
    \return The current timestamp.
     */
    INTERFACE_API unsigned long long sl_get_current_timestamp(int camera_id);

    /**
    \brief Gets the total number of frames in the loaded SVO file.
    \param camera_id : id of the camera instance.
    \return The total number of frames in the SVO file (-1 if the SDK is not reading a SVO).
     */
    INTERFACE_API int sl_get_svo_number_of_frames(int camera_id);

    /**
    \brief Sets a value in the ZED's camera settings.
    \param camera_id : id of the camera instance.
    \param mode : Setting to be changed
    \param value : new value
     */
    INTERFACE_API void sl_set_camera_settings(int camera_id, enum SL_VIDEO_SETTINGS mode, int value);

    /**
    \brief Sets the region of interest for automatic exposure/gain computation
    \param camera_id : id of the camera instance.
    \param side : defines left,right,both to distinguish between left and right or both sides (see \ref SL_SIDE).
    \param roi : Region of interest.
    \param reset : reset aestruct C_agc if true.
    \return SL_ERROR_CODE::SUCCESS if ROI has been applied. Other ERROR_CODE otherwise.
     */
    INTERFACE_API int sl_set_roi_for_aec_agc(int camera_id, enum SL_SIDE side, struct SL_Rect* roi, bool reset);

    /**
    \brief Gets the value of a given setting from the ZED camera.
    \param camera_id : id of the camera instance.
    \param mode : Setting to be retrieved (see \ref SL_VIDEO_SETTINGS).
    \return The current value for the corresponding setting. Returns -1 if encounters an error.
     */
    INTERFACE_API int sl_get_camera_settings(int camera_id, enum SL_VIDEO_SETTINGS mode);

    /**
    \brief Gets the region of interest for automatic exposure/gain computation
    \param camera_id : id of the camera instance.
    \param side : defines left,right,both to distinguish between left and right or both sides (see \ref SL_SIDE).
    \param roi [Out] : Region of interest.
    \return SL_ERROR_CODE::SUCCESS if ROI has been applied. Other ERROR_CODE otherwise.
     */
    INTERFACE_API int sl_get_roi_for_aec_agc(int id, enum SL_SIDE side, struct SL_Rect* roi);

    /**
    \brief Gets the depth min value from InitParameters (see \ref SL_InitParameters::depth_minimum_distance).
    \param camera_id : id of the camera instance.
    \return The min depth value available.
     */
    INTERFACE_API float sl_get_depth_min_range_value(int camera_id);

    /**
    \brief Gets the depth max value from InitParameters (see \ref SL_InitParameters::depth_maximum_distance).
    \param camera_id : id of the camera instance.
    \return The max depth value available.
     */
    INTERFACE_API float sl_get_depth_max_range_value(int camera_id);

    /**
    \brief Gets the current range of perceived depth.
    \param camera_id : id of the camera instance.
    \param min : \b [out] Minimum depth detected (in selected sl::UNIT)
    \param max : \b [out] Maximum depth detected (in selected sl::UNIT)
    \return SL_ERROR_CODE::SUCCESS if values have been extracted. Other ERROR_CODE otherwise.
     */
    INTERFACE_API int sl_get_current_min_max_depth(int camera_id,float& min, float& max);

    /**
    \brief Gets the number of zed connected.
    \return The number of Zed cameras connected.
     */
    INTERFACE_API int sl_get_number_zed_connected();

    /**
    \brief Returns the version of the currently installed ZED SDK.
    \return The ZED SDK version installed.
     */
    INTERFACE_API char* sl_get_sdk_version();

    /**
    \brief Gets the current position of the SVO being recorded to.
    \param camera_id : id of the camera instance.
    \return The current SVO position;
     */
    INTERFACE_API int sl_get_svo_position(int camera_id);

    /**
    \brief Gets the number of frames dropped since Grab() was called for the first time. Based on camera timestamps and an FPS comparison.
    \param camera_id : id of the camera instance.
    \return The number of frames dropped since the first Grab() call.
     */
    INTERFACE_API unsigned int sl_get_frame_dropped_count(int camera_id);


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////// Motion tracking ///////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	\brief Gets the current position of the camera and state of the tracking, with an optional offset to the tracking frame.
	\param camera_id : id of the camera instance.
	\return true if the tracking module is enabled
	*/
	INTERFACE_API bool sl_is_positional_tracking_enabled(int camera_id);

    /**
    \brief Gets the current position of the camera and state of the tracking, with an optional offset to the tracking frame.
    \param camera_id : id of the camera instance.
    \param rotation : Quaternion filled with the current rotation of the camera depending on its reference frame.
    \param position : Vector filled with the current position of the camera depending on its reference frame.
    \param target_quaternion : Rotational offset applied to the tracking frame.
    \param target_translation : Positional offset applied to the tracking frame.
    \param reference_frame : Reference frame for setting the rotation/position.
    \return The current state of the tracking process (see \ref SL_POSITIONAL_TRACKING_STATE).
     */
    INTERFACE_API int sl_get_position_at_target_frame(int camera_id, struct SL_Quaternion *rotation, struct SL_Vector3 *position, struct SL_Quaternion *target_quaternion, struct SL_Vector3 *target_translation, enum SL_REFERENCE_FRAME reference_frame);
    /**
    \brief Gets the current position of the camera and state of the tracking, filling a PoseData struck useful for AR pass-though.
    \param camera_id : id of the camera instance.
    \param poseData : Current Pose.
    \param reference_frame : Reference frame sor setting the rotation/position.
    \return The current state of the tracking process (see \ref SL_POSITIONAL_TRACKING_STATE).
     */
    INTERFACE_API int sl_get_position_data(int camera_id, struct SL_PoseData *poseData, enum SL_REFERENCE_FRAME reference_frame);
    /**
    \brief Retrieves the estimated position and orientation of the camera in the specified \ref REFERENCE_FRAME "reference frame".
    \param camera_id : id of the camera instance.
    \param rotation : Quaternion filled with the current rotation of the camera depending on its reference frame.
    \param position : Vector filled with the current position of the camera depending on its reference frame.
    \param reference_frame : Reference frame for setting the rotation/position.
    \return The current state of the tracking process (see \ref SL_POSITIONAL_TRACKING_STATE).
     */
    INTERFACE_API int sl_get_position(int camera_id, struct SL_Quaternion *rotation, struct SL_Vector3 *position, enum SL_REFERENCE_FRAME reference_frame);
    /**
    \brief Gets the position of the camera and the current state of the ZED Tracking as a float array (4x4).
    \param camera_id : id of the camera instance.
    \param pose : pose of the camera as a float array (float[16]).
    \param reference_frame : the reference from which you want the pose to be expressed (see SL_REFERENCE_FRAME::WORLD).
    \return The Positional tracking state.
     */
    INTERFACE_API int sl_get_position_array(int camera_id, float* pose, enum SL_REFERENCE_FRAME reference_frame);

    /**
    \brief Resets the tracking, and re-initializes the position with the given pose
    \param camera_id : id of the camera instance.
    \param rotation : rotation of the camera in the world frame when the function is called.
    \param translation : position of the camera in the world frame when the function is called.
    \return SL_ERROR_CODE::SUCCESS if the tracking has been reset, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_reset_positional_tracking(int camera_id, struct SL_Quaternion rotation, struct SL_Vector3 translation);
    /**
    \brief Resets the tracking with an offset.
    \param camera_id : id of the camera instance.
    \param rotation : rotation of the camera in the world frame when the function is called.
    \param translation : position of the camera in the world frame when the function is called.
    \param target_quaternion : rotation offset to apply.
    \param target_translation : translation offset to apply.
    \return SL_ERROR_CODE::SUCCESS if the tracking has been reset, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_reset_positional_tracking_with_offset(int camera_id, struct SL_Quaternion rotation, struct SL_Vector3 translation, struct SL_Quaternion target_quaternion, struct SL_Vector3 target_translation);
    /**
    \brief Sets a prior to the IMU orientation (using a ZED-mini, ZED2 or ZED2i).
     Prior must come from a external IMU, such as the HMD orientation and should be in a time frame
     that's as close as possible to the camera.
     \param rotation : Prior rotation.
     \SUCCESS if the transform has been passed, ERROR_CODE::INVALID_FUNCTION_CALL otherwise
     */
    INTERFACE_API int sl_set_imu_prior_orientation(int camera_id, struct SL_Quaternion rotation);
    /**
    \brief Gets the rotation given by the ZED-M/ZED2 IMU. Returns an error if using ZED (v1) which does not contains internal sensors.
    \param camera_id : id of the camera instance.
    \param quat [Out]: Rotation from the IMU.
    \param time_reference : time reference.
    \return ERROR_CODE::SUCCESS if sensors data have been extracted.
     */
    INTERFACE_API int sl_get_imu_orientation(int camera_id, struct SL_Quaternion *quat, enum SL_TIME_REFERENCE time_reference);
    /**
    \brief Gets the full Sensor data from the ZED-M/ZED2/ZED2i. Returns an error is using ZED (v1) which does not contains internal sensors.
    \param camera_id : id of the camera instance.
    \param data : sensor data.
    \param time_reference : time reference.
    \return ERROR_CODE::SUCCESS if sensors data have been extracted.
     */
    INTERFACE_API int sl_get_sensors_data(int camera_id, struct SL_SensorData *data, enum SL_TIME_REFERENCE time_reference);


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////// Spatial Mapping ///////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    \brief Consolidates the chucks from a scan. This is used to turn a lots of small meshes (which are efficient for the scanning process)
    into several large meshes (which are more convenient to work with).
    \param camera_id : id of the camera instance.
    \param nb_faces : define the new number of faces per chunk (useful for Unity that doesn't handle chunks over 65K vertices).
    \param nb_vertices : Array of the number of vertices in each submesh.
    \param nb_triangles : Array of the number of triangles in each submesh.
    \param nb_updated_submeshes : Number of submeshes.
    \param updates_indices : List of all submeshes updated since the last update.
    \param nb_vertices_tot : Total number of updated vertices in all submeshes.
    \param nb_triangles_tot : Total number of updated triangles in all submeshes.
    \param max_sub_mesh : Maximum number of submeshes.
     */
    INTERFACE_API void sl_spatial_mapping_merge_chunks(int camera_id, int nb_faces, int* nb_vertices, int* nb_triangles, int* nb_updated_submeshes, int* updated_indices, int* nb_vertices_tot, int* nb_triangles_tot, const int max_submesh);

    /**
    \brief Initializes and begins the spatial mapping processes.
    \param camera_id : id of the camera instance.
    \param type : Spatial mapping type (see \ref SL_SPATIAL_MAP_TYPE).
    \param resolution_meter : Spatial mapping resolution in meters.
    \param max_range_meter : Maximum scanning range in meters.
    \param save_texture : True to scan surface textures in addition to geometry.
    \param max_memory_usage : The maximum CPU memory (in megabytes) allocated for the meshing process.
    \return SUCCESS if everything went fine, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_enable_spatial_mapping(int camera_id, struct SL_SpatialMappingParameters* mapping_param);
    /**
    \brief Disables the Spatial Mapping process.
    \param camera_id : id of the camera instance.
     */
    INTERFACE_API void sl_disable_spatial_mapping(int camera_id);
    /**
     Sets the pause state of the data integration mechanism for the ZED's spatial mapping.
     \param camera_id : id of the camera instance.
     \param status : If true, the integration is paused. If false, the spatial mapping is resumed.
     */
    INTERFACE_API void sl_pause_spatial_mapping(int camera_id, bool status);
    /**
    \brief Starts the mesh generation process in a thread that doesn't block the spatial mapping process.
    \param camera_id : id of the camera instance.
     */
    INTERFACE_API void sl_request_mesh_async(int camera_id);
    /**
    \brief Returns the mesh generation status. Useful for knowing when to update and retrieve the mesh.
    \param camera_id : id of the camera instance.
    \return SUCCESS if the mesh is ready and not yet retrieved, otherwise ERROR_CODE::FAILURE.
     */
    INTERFACE_API int sl_get_mesh_request_status_async(int camera_id);
    /**
    \brief Gets the current state of spatial mapping.
    \param camera_id : id of the camera instance.
    \return The current state ( \ref SL_SPATIAL_MAPPING_STATE) of the spatial mapping process (
     */
    INTERFACE_API enum SL_SPATIAL_MAPPING_STATE sl_get_spatial_mapping_state(int camera_id);
    /**
    \brief Updates the internal version of the mesh and returns the sizes of the meshes.
    \param camera_id : id of the camera instance.
    \param nb_vertices : Array of the number of vertices in each submesh.
    \param nb_triangles : Array of the number of triangles in each submesh.
    \param nb_submeshes : Number of submeshes.
    \param updates_indices : List of all submeshes updated since the last update.
    \param num_vertices_tot : Total number of updated vertices in all submeshes.
    \param num_triangles_tot : Total number of updated triangles in all submeshes.
    \param max_sub_mesh : Maximum number of submeshes.
    \return SUCCESS if the mesh is updated.
     */
    INTERFACE_API int sl_update_mesh(int camera_id, int* nb_vertices, int* nb_triangles, int* nb_submeshes, int* updated_indices, int* nb_vertices_tot, int* nb_triangles_tot, const int max_submesh);
    /**
    \brief Retrieves all chunks of the generated mesh. Call update_mesh before calling this.
    Vertex and triangles arrays must be at least of the sizes returned by update_mesh (nb_vertices and nb_triangles).
    \param camera_id : id of the camera instance.
    \param vertices : Vertices of the mesh
    \param triangles : Triangles of the mesh.
    \param max_submeshes : Maximum number of submesh that can be handled.
    \param uvs : uvs of the texture.
    \param texture_ptr : Texture of the mesh (if enabled).
    \return SUCCESS if the mesh is retrieved.
     */
    INTERFACE_API int sl_retrieve_mesh(int camera_id, float* vertices, int* triangles, float* uvs, unsigned char* texture_ptr, const int max_submeshes);
    /**
    \brief Updates the internal version of the mesh and returns the sizes of the meshes.
    \param camera_id : id of the camera instance.
    \param nb_vertices : Array of the number of vertices in each submesh.
    \param nb_triangles : Array of the number of triangles in each submesh.
    \param nb_submeshes : Number of submeshes.
    \param updates_indices : List of all submeshes updated since the last update.
    \param num_vertices_tot : Total number of updated vertices in all submeshes.
    \param num_triangles_tot : Total number of updated triangles in all submeshes.
    \param max_sub_mesh : Maximum number of submeshes.
    \return SUCCESS if the chunks are updated.
     */
    INTERFACE_API int sl_update_chunks(int camera_id, int* nb_vertices, int* nb_triangles, int* nb_submeshes, int* updated_indices, int* nb_vertices_tot, int* nb_triangles_tot, const int max_submesh);
    /**
    \brief Retrieves one chunk data. Call update_chunks before calling this.
    Vertex and triangles arrays must be at least of the sizes returned by update_mesh (nbVertices and nbTriangles).
    \param camera_id : id of the camera instance.
    \param max_submesh : Maximum number of submesh that can be handled.
    \param vertices : Vertices of the chunk
    \param triangles : Triangles of the chunk.
    \return SUCCESS if the chunk is retrieved.
     */
    INTERFACE_API int sl_retrieve_chunks(int camera_id, float* vertices, int* triangles, const int max_submesh);
    /**
    \brief Updates the fused point cloud (if spatial map type was FUSED_POINT_CLOUD).
    \param camera_id : id of the camera instance.
    \param num_vertices_tot : returns the total number of vertices.
    \return SUCCESS if the fused point cloud is updated.
     */
    INTERFACE_API int sl_update_fused_point_cloud(int camera_id, int* nb_vertices_tot);
    /**
    \brief Retrieves all points of the fused point cloud. Call update_fused_point_cloud before calling this.
    \param camera_id : id of the camera instance.
    \param vertices : Points of the fused point cloud.
    \return SUCCESS if the fused point cloud is retrieved.
     */
    INTERFACE_API int sl_retrieve_fused_point_cloud(int camera_id, float* vertices);
    /**
    \brief Extracts the current spatial map from the spatial mapping process.
    \param camera_id : id of the camera instance.
    SUCCESS if the mesh is filled and available, otherwise FAILURE.
     */
    INTERFACE_API int sl_extract_whole_spatial_map(int camera_id);
    /**
    \brief Saves the scanned mesh in a specific file format.
    \param camera_id : id of the camera instance.
    \param filename : Path and filename of the mesh.
    \param format : File format (extension). Can be .obj, .ply or .bin.
    \return True if the file was successfully saved, false otherwise.
     */
    INTERFACE_API bool sl_save_mesh(int camera_id, const char* filename, enum SL_MESH_FILE_FORMAT format);
	/**
	\brief Saves the scanned point cloud in a specific file format.
	\param camera_id : id of the camera instance.
	\param filename : Path and filename of the point cloud.
	\param format : File format (extension). Can be .obj, .ply or .bin.
	\return True if the file was successfully saved, false otherwise.
	 */
	INTERFACE_API bool sl_save_point_cloud(int c_id, const char* filename, enum SL_MESH_FILE_FORMAT format);
    /**
    \brief Loads a saved mesh file.
    \param camera_id : id of the camera instance.
    \param filename : Path and filename of the mesh. Should incluse the extension (.obj, .ply or .bin).
    \param nb_ertices : Array of the number of vertices in each submesh.
    \param nb_triangles : Array of the number of triangles in each submesh.
    \param nb_sub_meshes : Number of submeshes.
    \param updated_indices : List of all submeshes updated since the last update.
    \param nb_vertices_tot :  Total number of updated vertices in all submeshes.
    \param nb_triangles_tot : Array of the number of triangles in each submesh.
    \param max_submesh : Maximum number of submeshes taht can be handled.
    \param texture_size : Array containing the sizes of all the textures (width ,height) if applicable.
    \return True if the file was successfully loaded, false otherwise.
     */
    INTERFACE_API bool sl_load_mesh(int camera_id, const char* filename, int* nb_vertices, int* nb_triangles, int* num_submeshes, int* updated_indices, int* nb_vertices_tot, int* nb_triangles_tot, int* textures_size,const int max_submesh);
    /**
    \brief Applies the scanned texture onto the internal scanned mesh.
    \param camera_id : id of the camera instance.
    \param nb_vertices : Array of the number of vertices in each submesh.
    \param nb_triangles : Array of the number of triangles in each submesh.
    \param nb_sub_meshes : Number of submeshes.
    \param updated_indices : List of all submeshes updated since the last update.
    \param nb_vertices_tot :  Total number of updated vertices in all submeshes.
    \param nb_triangles_tot : Array of the number of triangles in each submesh.
    \param max_submesh : Maximum number of submeshes taht can be handled.
    \param texture_size : Array containing the sizes of all the textures (width ,height) if applicable.
    \return True if the texturing was successful, false otherwise.
     */
    INTERFACE_API bool sl_apply_texture(int camera_id, int* nb_vertices, int* nb_triangles, int* nb_updated_submeshes, int* updated_indices, int* nb_vertices_tot, int* nb_triangles_tot, int* textures_size, const int max_submesh);
    /**
    \brief Filters a mesh to removes triangles while still preserving its overall shaper (though less accurate).
    \param camera_id : id of the camera instance.
    \param filter_params : Filter level. Higher settings remore more triangles (SL_MeshFilterParameters::MESH_FILTER).
    \param nb_ vertices : Array of the number of vertices in each submesh.
    \param nb_triangles : Array of the number of triangles in each submesh.
    \param nb_sub_meshes : Number of submeshes.
    \param updated_indices : List of all submeshes updated since the last update.
    \param nb_vertices_tot :  Total number of updated vertices in all submeshes.
    \param nb_triangles_tot : Array of the number of triangles in each submesh.
    \param max_submesh : Maximum number of submeshes that can be handled.
    \return True if the filtering was successful, false otherwise.
     */
    INTERFACE_API bool sl_filter_mesh(int camera_id, enum SL_MESH_FILTER filter_params, int* nb_vertices, int* nb_triangles, int* nb_updated_submeshes, int* updated_indices, int* nb_vertices_tot, int* nb_triangles_tot, const int max_submesh);
    /**
    \brief Gets a vector pointing toward the direction of gravity. This is estimated from a 3D scan of the environment,
    and such, a scan must be started and finished for this value to be calculated.
    If using the ZED_M  /ZED2, this isn't required thanks to its IMU.
    \param camera_id : id of the camera instance.
    \param gravity [Out] : vector of gravity.
     */
    INTERFACE_API void sl_spatial_mapping_get_gravity_estimation(int camera_id, struct SL_Vector3 *gravity);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////// Plane Detection ///////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    \brief Looks for a plane in the visible area that is likely to represent the floor.
    \param camera_id : id of the camera instance.
    \param reset_quaternion : The rotation to align the axis with the gravity.
    \param reset_translation : The translation to align the tracking with the floor plane. The initial position will then be at ground height.
    \param prior_rotation : Prior rotation.
    \param prior_translation : Prior translation.
    \return The detected floor plane if the function succeeded.
     */
    INTERFACE_API struct SL_PlaneData* sl_find_floor_plane(int camera_id, struct SL_Quaternion *reset_quaternion, struct SL_Vector3* reset_translation, struct SL_Quaternion prior_rotation, struct SL_Vector3 prior_translation);
    /**
    \brief Check for a plane in hte real world at given screen-space coordinates.
    \param camera_id : id of the camera instance.
    \param pixel : Point on the ZED image to check for a plane.
    \param thres : Check if area is enough for Unity. If true, removes smaller planes.
    \return Data of the detected plane.
     */
    INTERFACE_API struct SL_PlaneData* sl_find_plane_at_hit(int camera_id, struct SL_Vector2 pixel, bool thres);
    /**
    \brief Using data from a detected floor plane, updates supplied vertex and triangles arrays with data needed to make a mesh that represents it.
    \param camera_id : id of the camera instance.
    \param vertices : Array to be filled with mesh vertices.
    \param triangles : Array to be filled with mesh triangles, stored as indexes of each triangle's points.
    \param nb_vertices_tot : Total number of vertices in the mesh.
    \param nb_triangles_tot : Total triangles indexes (3x number of triangles).
    \return SUCCESS if the mesh conversion was successful.
     */
    INTERFACE_API int sl_convert_floorplane_to_mesh(int camera_id, float* vertices, int* triangles, int* nb_vertices_tot, int* nb_triangles_tot);
    /**
    \brief Using data from a detected hit plane, updates supplied vertex and triangles arrays with data needed to make a mesh that represents it.
    \param camera_id : id of the camera instance.
    \param vertices : Array to be filled with mesh vertices.
    \param triangles : Array to be filled with mesh triangles, stored as indexes of each triangle's points.
    \param nb_vertices_tot : Total number of vertices in the mesh.
    \param nb_triangles_tot : Total triangles indexes (3x number of triangles).
    \return SUCCESS if the mesh conversion was successful.
     */
    INTERFACE_API int sl_convert_hitplane_to_mesh(int camera_id, float* vertices, int* triangles, int* nb_vertices_tot, int* nb_triangles_tot);


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////// Retrieve /////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    \brief Retrieves a measure texture from the ZED SDK. Use this to get an individual texture from the last grabbed frame with measurements in every pixel -
    such as depth map, confidence map etc.
    Measure textures are not human-viewable but don't lose accuracy, unlike image textures.
    \param camera_id : id of the camera instance.
    \param measure_ptr : pointer to the measure texture.
    \param type : Measure type (depth, confidence, xyz, etc). See \ref SL_MEASURE.
    \param  mem : Whether the measure should be on CPU or GPU memory. See \ref SL_MEM.
    \param width : width of the texture in pixel.
    \param height : height of the texture in pixel.
    \return "SUCCESS" if the retrieve succeeded.
     */
    INTERFACE_API int sl_retrieve_measure(int camera_id, int* measure_ptr, enum SL_MEASURE type, enum SL_MEM mem, int width, int height);
    /**
    \brief Retrieves an image texture from the ZED SDK in a human-viewable format. Image textures work for when you want the result to be visible,
    such as the direct RGB image from the camera, or a greyscale image of the depth. However it will lose accuracy if used to show measurements
    like depth or confidence, unlike measure textures.
    \param camera_id : id of the camera instance.
    \param image_ptr : pointer to the image texture.
    \param type : Image type (left RGB, right depth map, etc). See \ref SL_VIEW.
    \param  mem : Whether the image should be on CPU or GPU memory (SL_MEM).
    \param width : width of the texture in pixel.
    \param height : height of the texture in pixel.
    \return "SUCCESS" if the retrieve succeeded.
     */
    INTERFACE_API int sl_retrieve_image(int camera_id, int* image_ptr, enum SL_VIEW type, enum SL_MEM mem, int width, int height);


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////// Streaming Sender //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    \brief Creates a streaming pipeline.
    \param camera_id : id of the camera instance.
    \param codec : defines the codec used for streaming (see \ref SL_STREAMING_CODE).
    \param bitrate : defines the streaming bitrate in Kbits/s.
            | STREAMING_CODEC  | Resolution   | FPS   | bitrate (kbps) |
            |------------------|--------------|-------|----------------|
            | H264             |  HD2K        |   15  |     8500       |
            | H264             |  HD1080      |   30  |    12500       |
            | H264             |  HD720       |   60  |     7000       |
            | H265             |  HD2K        |   15  |     7000       |
            | H265             |  HD1080      |   30  |    11000       |
            | H265             |  HD720       |   60  |     6000       |

    \note Available range : [1000 - 60000]
    \param port : defines the port used for streaming.
    \param gop_size : defines the gop size in number of frames.
    \param adaptative_bitrate : enable/disable adaptive bitrate.
    \param chunk_size : defines a single chunk size.
    \param target_framrate : defines the target framerate for the streaming output.
    \return An ERROR_CODE that defines if the stream was started.
     */
    INTERFACE_API int sl_enable_streaming(int camera_id, enum SL_STREAMING_CODEC codec, unsigned int bitrate, unsigned short port, int gop_size, int adaptative_bitrate, int chunk_size, int target_framerate);
    /**
    \brief Tells if the streaming is running (true) or still initializing (false).
    \param camera_id : id of the camera instance.
    \return True is the streaming is running, False if still initializing.
     */
    INTERFACE_API int sl_is_streaming_enabled(int camera_id);
    /**
    \brief Disables the streaming initiated by enable_streaming().
    \param camera_id : id of the camera instance.
     */
    INTERFACE_API void sl_disable_streaming(int camera_id);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////// Save to File Utils ////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    \brief Writes the image into a file defined by its extension.
    \param camera_id : id of the camera instance.
    \param view : View of the image (SL_VIEW).
    \param file_name : file path including the name and extension.
    \return SUCCESS if the image was successfuly saved.
     */
    INTERFACE_API int sl_save_current_image(int camera_id, enum SL_VIEW view, const char* file_name);
    /**
    \brief Writes the depth map into a file defined by its extension.
    \param camera_id : id of the camera instance.
    \param side : side of the image (0 = left, 1 = right).
    \param file_name : file path including the name and extension.
    \return SUCCESS if the depth was successfuly saved.
     */
    INTERFACE_API int sl_save_current_depth(int camera_id, enum SL_SIDE side, const char* file_name);
    /**
    \brief Writes the point cloud into a file defined by its extension.
    \param camera_id : id of the camera instance.
    \param side : side of the pc (0 = left, 1 = right).
    \param file_name : file path including the name and extension.
    \return SUCCESS if the point cloud was successfuly saved.
     */
    INTERFACE_API int sl_save_current_point_cloud(int camera_id, enum SL_SIDE side, const char* file_name);



#if WITH_OBJECT_DETECTION

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////// Object Detection //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 /**
		\brief Check if a corresponding optimized engine is found for the requested Model based on your rig configuration.
		\param model : AI model to check.
		\param gpu_id : ID of the gpu.
		\return The status of the given model for the specified GPU.
	*/
	INTERFACE_API SL_AI_Model_status* sl_check_AI_model_status(SL_AI_MODELS model, int gpu_id);

	/**
	\brief Optimize the requested model, possible download if the model is not present on the host.
	\param model : AI model to optimize.
	\param gpu_id : ID of the gpu to optimize on.
	\return SUCCESS if the model is well optimized.
	*/
	INTERFACE_API SL_ERROR_CODE sl_optimize_AI_model(SL_AI_MODELS model, int gpu_id);

    /**
    \brief Initializes and starts the Deep Learning detection module.
    \n The object detection module currently supports two types of detection :
    - Multiple class of objects with the \ref DETECTION_MODEL::MULTI_CLASS_BOX, \ref DETECTION_MODEL::MULTI_CLASS_BOX_MEDIUM or \ref DETECTION_MODEL::MULTI_CLASS_BOX_ACCURATE.
    The full list of detectable objects is available through \ref OBJECT_CLASS and \ref OBJECT_SUBCLASS.
    - Human skeleton detection with the \ref DETECTION_MODEL::HUMAN_BODY_FAST,\ref DETECTION_MODEL::HUMAN_BODY_MEDIUM or \ref DETECTION_MODEL::HUMAN_BODY_ACCURATE.
    This model only detects humans but also provides a full skeleton map for each person.

    Detected objects can be retrieved using the \ref retrieve_objects() function.
    \param camera_id : id of the camera instance.
    \param object_detection_parameters : structure containing all specific parameters for object detection (see \ref SL_ObjectDetectionParameters).
    \return
             - \ref ERROR_CODE::SUCCESS : if everything went fine.\n
             - \ref ERROR_CODE::CORRUPTED_SDK_INSTALLATION : if the AI model is missing or corrupted. In this case, the SDK needs to be reinstalled.\n
             - \ref ERROR_CODE::MODULE_NOT_COMPATIBLE_WITH_CAMERA : if the camera used does not have a IMU (ZED Camera). the IMU gives the gravity vector that helps in the 3D box localization.\n
             - \ref ERROR_CODE::MOTION_SENSORS_REQUIRED : if the camera model is correct (ZED2/ZED2i) but the IMU is missing. It probably happens because InitParameters::disable_sensors was set to true.\n
             - \ref ERROR_CODE::INVALID_FUNCTION_CALL : if one of the ObjectDetection parameter is not compatible with other modules parameters (For example, depth mode has been set to NONE).\n
             - \ref ERROR_CODE::FAILURE : otherwise.\n
     */
    INTERFACE_API int sl_enable_objects_detection(int camera_id, struct SL_ObjectDetectionParameters* object_detection_parameters);
    /**
    \brief Pauses or resumes the object detection processes.

    If the object detection has been enabled with  \ref ObjectDetectionParameters::image_sync set to false (running asynchronously), this function will pause processing.

    While in pause, calling this function with <i>status = false</i> will resume the object detection.
    The \ref retrieveObjects function will keep on returning the last objects detected while in pause.

    \param status : If true, object detection is paused. If false, object detection is resumed.
     */
    INTERFACE_API void sl_pause_objects_detection(int camera_id, bool status);
    /**
    \brief Disables the Object Detection process.

    The object detection module immediately stops and frees its memory allocations.
    \param camera_id : id of the camera instance.
     */
    INTERFACE_API void sl_disable_objects_detection(int camera_id);

	/**
	\brief Generate a UUID like unique ID to help identify and track AI detections
	\param uuid : Unique ID generated.
	\return : Size of the unique ID generated.
	 */
	INTERFACE_API int sl_generate_unique_id(char* uuid);

	/**
	\brief Feed the 3D Object tracking function with your own 2D bounding boxes from your own detection algorithm.
	\param camera_id : id of the camera instance.
	\param objects_in : 2D detections from custom detection algorithm.
	\param nb_objects : number of custom objects (size of the object_in array).
	\note The detection should be done on the current grabbed left image as the internal process will use all current available data to extract 3D informations and perform object tracking.
	\return \ref SUCCESS if everything went fine, \ref ERROR_CODE::FAILURE otherwise
   */
	INTERFACE_API int sl_ingest_custom_box_objects(int camera_id, int nb_objects, struct SL_CustomBoxObjectData* objects_in);

    /**
    \brief Retrieve objects detected by the object detection module.
    \param camera_id : id of the camera instance.
    \param objects : The detected objects will be saved into this object. If the object already contains data from a previous detection, it will be updated, keeping a unique ID for the same person.
    \param object_detection_runtime_parameters : Object detection runtime settings, can be changed at each detection. In async mode, the parameters update is applied on the next iteration.
    \return \ref SUCCESS if everything went fine, \ref ERROR_CODE::FAILURE otherwise
     */
    INTERFACE_API int sl_retrieve_objects(int camera_id, struct SL_ObjectDetectionRuntimeParameters* object_detection_runtime_parameters, struct SL_Objects* objects);

    /**
    \brief Updates the internal batch of detected objects.
    \param camera_id : id of the camera instance.
    \param [Out] : number of batches.
    \return \ref SUCCESS if everything went fine, \ref ERROR_CODE::FAILURE otherwise
     */
    INTERFACE_API int sl_update_objects_batch(int camera_id, int* nb_batches);

    /**
    \brief Gets a batch of detected objects. Need to be called after update_objects_batch().
    \param camera_id : id of the camera instance.
    \param index : index of the batch to retrieve ([0, nb_batches]).
    \param nb_data : number of detected objects in the batch.
    \param id : The trajectory ID.
    \param label : Object Category. Identify the object type (SL_OBJECT_CLASS).
    \param sublabel : Object subclass (SL_OBJECT_SUBCLASS).
    \param tracking_state : defines the object tracking state (SL_OBJECT_TRACKING_STATE).
    \param positions : a sample of 3D positions.
    \param position_covariances : a sample of the associated position covariances.
    \param velocities : a sample of 3D velocity.
    \param timestamps : The associated position timestamp.
    \param bounding_boxes_2d : 2D bounding box of the person represented as four 2D points starting at the top left corner and rotation clockwise.
     Expressed in pixels on the original image resolution, [0,0] is the top left corner.
                     A ------ B
                     | Object |
                     D ------ C
    \param bounding_box : a sample of 3d bounding boxes.
    \param confidences : a sample of object detection confidence.
    \param action_states : a sample of object action state (SL_OBJECT_ACTION_STATE).
    \param keypoints_2d : a sample of 2d position keypoints.
    Not available with DETECTION_MODE::MULTI_CLASS_BOX.
    \param keypoints : a sample of 3d position keypoints.
    Not available with DETECTION_MODE::MULTI_CLASS_BOX.
    \param head_bounding_boxes_2d : bounds the head with four 2D points.
    Not available with DETECTION_MODE::MULTI_CLASS_BOX.
    \param head_bounding_boxes :  bounds the head with eight 3D points.
    Not available with DETECTION_MODE::MULTI_CLASS_BOX.
    \param head_positions : 3D head centroids.
    \param keypoint_confidences : Per keypoint detection confidence, can not be lower than the \ref ObjectDetectionRuntimeParameters::detection_confidence_threshold.
    Not available with DETECTION_MODEL::MULTI_CLASS_BOX.
    \return \ref SUCCESS if everything went fine, \ref ERROR_CODE::FAILURE otherwise
     */
	INTERFACE_API int sl_get_objects_batch(int camera_id, int index, struct SL_ObjectsBatch* objs_batch);


    INTERFACE_API int sl_get_objects_batch_csharp(int camera_id, int index, int* nb_data, int* id, int* label, int* sublabel, int* tracking_state,
		struct SL_Vector3 positions[MAX_TRAJECTORY_SIZE], float position_covariances[MAX_TRAJECTORY_SIZE][6], struct SL_Vector3 velocities[MAX_TRAJECTORY_SIZE], unsigned long long timestamps[MAX_TRAJECTORY_SIZE],
		struct SL_Vector2 bounding_boxes_2d[MAX_TRAJECTORY_SIZE][4], struct SL_Vector3 bounding_boxes[MAX_TRAJECTORY_SIZE][8], float confidences[MAX_TRAJECTORY_SIZE], int action_states[MAX_TRAJECTORY_SIZE],
		struct SL_Vector2 keypoints_2d[MAX_TRAJECTORY_SIZE][18], struct SL_Vector3 keypoints[MAX_TRAJECTORY_SIZE][18], struct SL_Vector2 head_bounding_boxes_2d[MAX_TRAJECTORY_SIZE][4], struct SL_Vector3 head_bounding_boxes[MAX_TRAJECTORY_SIZE][8],
		struct SL_Vector3 head_positions[MAX_TRAJECTORY_SIZE], float keypoints_confidences[MAX_TRAJECTORY_SIZE][18]);
#endif
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////// Mat ///////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    \brief Creates a Mat with the given resolution.
    \param width : width of the new mat.
    \param height : height of the new mat.
    \param type : Data type and number of channels the Mat will hold (see \ref SL_MAT_TYPE).
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return Ptr of the Mat.
     */
    INTERFACE_API int* sl_mat_create_new(int width, int height, enum SL_MAT_TYPE type, enum SL_MEM mem);
    /**
    \brief Creates an empty Mat with the given resolution.
    \return Ptr of the Mat.
     */
    INTERFACE_API int* sl_mat_create_new_empty();
    /**
    \brief Tells if the Mat has been initialized.
    \param ptr : Ptr to the Mat.
    \return True if the Mat has been initialized.
     */
    INTERFACE_API bool sl_mat_is_init(int* ptr);
    /**
    \brief Frees the memory of the Mat.
    \param ptr : Ptr to the Mat.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return True if the Mat has been initialized.
     */
    INTERFACE_API void sl_mat_free(int* ptr, enum SL_MEM mem);
    /**
    \brief Returns information about the Mat.
    \param ptr : Ptr to the Mat.
    \param buffer : buffer providing Mat information.
     */
    INTERFACE_API void sl_mat_get_infos(int* ptr, char* buffer);

    // GET values
    /**
    \brief Returns the value of a specific point in the matrix.
    \param ptr : Ptr to the Mat.
    \param col : specifies the column.
    \param raw : specifices the row.
    \param value [Out] : the value to get.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_get_value_uchar(int* ptr, int col, int raw, unsigned char* value, enum SL_MEM mem);
    /**
    \brief Returns the value of a specific point in the matrix.
    \param ptr : Ptr to the Mat.
    \param col : specifies the column.
    \param raw : specifices the row.
    \param value [Out] : the value to get.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_get_value_uchar2(int* ptr, int col, int raw, struct SL_Uchar2* value, enum SL_MEM mem);
    /**
    \brief Returns the value of a specific point in the matrix.
    \param ptr : Ptr to the Mat.
    \param col : specifies the column.
    \param raw : specifices the row.
    \param value [Out] : the value to get.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_get_value_uchar3(int* ptr, int col, int raw, struct SL_Uchar3 * value, enum SL_MEM mem);
    /**
    \brief Returns the value of a specific point in the matrix.
    \param ptr : Ptr to the Mat.
    \param col : specifies the column.
    \param raw : specifices the row.
    \param value [Out] : the value to get.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int mat_get_value_uchar4(int* ptr, int col, int raw, struct SL_Uchar4* value, enum SL_MEM mem);

    /**
    \brief Returns the value of a specific point in the matrix.
    \param ptr : Ptr to the Mat.
    \param col : specifies the column.
    \param raw : specifices the row.
    \param value [Out] : the value to get.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_get_value_float(int* ptr, int col, int raw, float* value, enum SL_MEM mem);
    /**
    \brief Returns the value of a specific point in the matrix.
    \param ptr : Ptr to the Mat.
    \param col : specifies the column.
    \param raw : specifices the row.
    \param value [Out] : the value to get.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_get_value_float2(int* ptr, int col, int raw, struct SL_Vector2 * value, enum SL_MEM mem);
    /**
    \brief Returns the value of a specific point in the matrix.
    \param ptr : Ptr to the Mat.
    \param col : specifies the column.
    \param raw : specifices the row.
    \param value [Out] : the value to get.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_get_value_float3(int* ptr, int col, int raw, struct SL_Vector3 * value, enum SL_MEM mem);
    /**
    \brief Returns the value of a specific point in the matrix.
    \param ptr : Ptr to the Mat.
    \param col : specifies the column.
    \param raw : specifices the row.
    \param value [Out] : the value to get.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_get_value_float4(int* ptr, int col, int raw, struct SL_Vector4 * value, enum SL_MEM mem);

    // SET VALUE
    /**
    \brief Sets a value to a specific point in the matrix.
    \param ptr : Ptr to the Mat.
    \param col : specifies the column.
    \param raw : specifices the row.
    \param value : the value to be set.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_set_value_uchar(int* ptr, int col, int raw, unsigned char value, enum SL_MEM mem);
    /**
    \brief Sets a value to a specific point in the matrix.
    \param ptr : Ptr to the Mat.
    \param col : specifies the column.
    \param raw : specifices the row.
    \param value [Out] : the value to get.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int mat_set_value_uchar2(int* ptr, int col, int raw, struct SL_Uchar2 value, enum SL_MEM mem);
    /**
    \brief Sets a value to a specific point in the matrix.
    \param ptr : Ptr to the Mat.
    \param col : specifies the column.
    \param raw : specifices the row.
    \param value [Out] : the value to get.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_set_value_uchar3(int* ptr, int col, int raw, struct SL_Uchar3 value, enum SL_MEM mem);
    /**
    \brief Sets a value to a specific point in the matrix.
    \param ptr : Ptr to the Mat.
    \param col : specifies the column.
    \param raw : specifices the row.
    \param value : the value to be set.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_set_value_uchar4(int* ptr, int col, int raw, struct SL_Uchar4 value, enum SL_MEM mem);
    /**
    \brief Sets a value to a specific point in the matrix.
    \param ptr : Ptr to the Mat.
    \param col : specifies the column.
    \param raw : specifices the row.
    \param value : the value to be set.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_set_value_float(int* ptr, int col, int raw, float value, enum SL_MEM mem);
    /**
    \brief Sets a value to a specific point in the matrix.
    \param ptr : Ptr to the Mat.
    \param col : specifies the column.
    \param raw : specifices the row.
    \param value : the value to be set.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_set_value_float2(int* ptr, int col, int raw, struct SL_Vector2 value, enum SL_MEM mem);
    /**
    \brief Sets a value to a specific point in the matrix.
    \param ptr : Ptr to the Mat.
    \param col : specifies the column.
    \param raw : specifices the row.
    \param value : the value to be set.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_set_value_float3(int* ptr, int col, int raw, struct SL_Vector3 value, enum SL_MEM mem);
    /**
    \brief Sets a value to a specific point in the matrix.
    \param ptr : Ptr to the Mat.
    \param col : specifies the column.
    \param raw : specifices the row.
    \param value : the value to be set.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_set_value_float4(int* ptr, int col, int raw, struct SL_Vector4 value, enum SL_MEM mem);

    //SET TO
    /**
    \brief Fills the entire Mat with the given value.
    \param ptr : Ptr to the Mat.
    \param value : the value with which to fill the Mat.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_set_to_uchar(int* ptr, unsigned char value, enum SL_MEM mem);
    /**
    \brief Fills the entire Mat with the given value.
    \param ptr : Ptr to the Mat.
    \param value : the value with which to fill the Mat.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_set_to_uchar2(int* ptr, struct SL_Uchar2 value, enum SL_MEM mem);
    /**
    \brief Fills the entire Mat with the given value.
    \param ptr : Ptr to the Mat.
    \param value : the value with which to fill the Mat.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_set_to_uchar3(int* ptr, struct SL_Uchar3 value, enum SL_MEM mem);
    /**
    \brief Fills the entire Mat with the given value.
    \param ptr : Ptr to the Mat.
    \param value : the value with which to fill the Mat.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_set_to_uchar4(int* ptr, struct SL_Uchar4 value, enum SL_MEM mem);
    /**
    \brief Fills the entire Mat with the given value.
    \param ptr : Ptr to the Mat.
    \param value : the value with which to fill the Mat.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_set_to_float(int* ptr, float value, enum SL_MEM mem);
    /**
    \brief Fills the entire Mat with the given value.
    \param ptr : Ptr to the Mat.
    \param value : the value with which to fill the Mat.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_set_to_float2(int* ptr, struct SL_Vector2 value, enum SL_MEM mem);
    /**
    \brief Fills the entire Mat with the given value.
    \param ptr : Ptr to the Mat.
    \param value : the value with which to fill the Mat.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_set_to_float3(int* ptr, struct SL_Vector3 value, enum SL_MEM mem);
    /**
    \brief Fills the entire Mat with the given value.
    \param ptr : Ptr to the Mat.
    \param value : the value with which to fill the Mat.
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_set_to_float4(int* ptr, struct SL_Vector4 value, enum SL_MEM mem);

    /**
    \brief Copies data from the GPU to the CPU, if possible.
    \param ptr : Ptr to the Mat.
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_update_cpu_from_gpu(int* ptr);
    /**
    \brief Copies data from the CPU to the GPU, if possible.
    \param ptr : Ptr to the Mat.
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_update_gpu_from_cpu(int* ptr);
    /**
    \brief Copies data from this Mat to another Mat (deep copy).
    \param ptr : Ptr to the Source Mat.
    \param ptr_dest : Ptr to the Destination Mat.
    \param cpy_type : The To and From memory types (see \ref SL_COPY_TYPE).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_copy_to(int* ptr, int* ptr_dest, enum SL_COPY_TYPE cpy_type);

    /**
    \brief Reads an image from a file. Supports .png and .jpeg. Only works if Mat has access to MEM_CPU.
    \param ptr : Ptr to the Mat.
    \param file_path : File path, including file name and extension.
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_read(int* ptr, const char* file_path);
    /**
    \brief Writes the Mat into a file as an image. Only works if Mat has access to MEM_CPU.
    \param ptr : Ptr to the Mat.
    \param file_path : File path, including file name and extension.
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_write(int* ptr, const char* file_path);
    /**
    \brief Gets the Width of the matrix.
    \param ptr : Ptr to the Mat.
    \return The width of the matrix.
     */
    INTERFACE_API int sl_mat_get_width(int* ptr);
    /**
    \brief Gets the Height of the matrix.
    \param ptr : Ptr to the Mat.
    \return The height of the matrix.
     */
    INTERFACE_API int sl_mat_get_height(int* ptr);
    /**
    \brief Gets the number of channels stored in each pixel.
    \param ptr : Ptr to the Mat.
    \return Number of values/channels.
     */
    INTERFACE_API int sl_mat_get_channels(int* ptr);
    /**
    \brief Gets the type of memory (CPU and/or GPU).
    \param ptr : Ptr to the Mat.
    \return The memory type (SL_MEM).
     */
    INTERFACE_API int sl_mat_get_memory_type(int* ptr);
    /**
    \brief Gets the size in bytes of one pixel.
    \param ptr : Ptr to the Mat.
    \return The size in bytes of one pixel.
     */
    INTERFACE_API int sl_mat_get_pixel_bytes(int* ptr);
    /**
    \brief Gets the memory 'step' in number/length of elements - how many values make up each row of pixels.
    \param ptr : Ptr to the Mat.
    \return The Step length.
     */
    INTERFACE_API int sl_mat_get_step(int* ptr);
    /**
    \brief Gets the memory 'step' in bytes - how many bytes make up each row of pixels.
    \param ptr : Ptr to the Mat.
    \return The Step length in bytes.
     */
    INTERFACE_API int sl_mat_get_step_bytes(int* ptr);
    /**
    \brief Gets the size of each row in bytes.
    \param ptr : Ptr to the Mat.
    \return The size of each row in bytes.
     */
    INTERFACE_API int sl_mat_get_width_bytes(int* ptr);
    /**
    \brief Returns whether the Mat is the owner of the memory it's accessing.
    \return True if the Mat is the owner of the memory it's accessing.
     */
    INTERFACE_API bool sl_mat_is_memory_owner(int* ptr);
    /**
    \brief Returns the resolution of the image that this Mat holds.
    \param ptr : Ptr to the Mat.
    \return The resolution of the mat.
     */
    INTERFACE_API struct SL_Resolution sl_mat_get_resolution(int* ptr);
    /**
    \brief Allocates memory for the Mat.
    \param ptr : Ptr to the Mat.
    \param width : Width of the image/matrix in pixels.
    \param height : Height of the image/matrix in pixels.
    \param type : Data type and number of channels the Mat will hold (see \ref SL_MAT_TYPE).
    \param mem : Whether Mat should exist on CPU or GPU memory ( \ref SL_MEM).
     */
    INTERFACE_API void sl_mat_alloc(int* ptr, int width, int height, enum SL_MAT_TYPE type, enum SL_MEM mem);
    /**
    \brief Copies data from another Mat into this one(deep copy).
    \param ptr : Ptr to the Mat.
    \param ptr_srouce : Source Mat from which to copy.
    \param copy_type : The To and From memory types (SL_COPY_TYPE).
    \param mem : Whether Mat should exist on CPU or GPU memory (SL_MEM).
    \return ERROR_CODE::SUCCESS if everything went well, ERROR_CODE::FAILURE otherwise.
     */
    INTERFACE_API int sl_mat_set_from(int* ptr, int* ptr_source, enum SL_COPY_TYPE copy_type);
    /**
    \brief Gets a pointer to the Mat.
    \param ptr : Ptr to the Mat.
    \param mem : Whether Mat should exist on CPU or GPU memory ( \ref SL_MEM).
    \return A pointer to the Mat.
     */
    INTERFACE_API int* sl_mat_get_ptr(int* ptr, enum SL_MEM mem);
    /**
    \brief Duplicates a Mat by copying all its data into a new one (deep copy).
    \param ptr : Ptr to the Mat.
    \param ptr_srouce : Source Mat from which to copy.
     */
    INTERFACE_API void sl_mat_clone(int* ptr, int* ptr_source);

#ifdef __cplusplus
}
#endif

#endif
