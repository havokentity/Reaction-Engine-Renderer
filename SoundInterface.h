/*
==========================================================================
	$Description:
	Audio classes
	OpenALSound handles wave files for events, uses OpenAL
	OpenALStream handles ogg files for streamed audio, uses OpenAL
	FMODSource is a helper class for using sounds with FMOD
	class SoundSystem is a helper class for the engine sound system

	NOTE: OpenAL is NO LONGER SUPPORTED BY RXENGINE but the classes already there
		  will remain.

  classes contained are:
 
   class OpenALSound
   class OpenALStream
   class FMODSource
   class SoundSystem

   emitter classes are:

   class SoundEmitterPoint
				|
				================> class SoundEmitterSphere
				|
				================> class SoundEmitterLine
				|
				================> class SoundEmitterBox

==========================================================================
*/

#ifndef OPEN_AL_CLASS_H_
#define OPEN_AL_CLASS_H_

#define INCLUDE_FMOD_EX

#pragma comment(lib, "openAL32.lib")
#pragma comment(lib, "alut.lib")
#pragma comment(lib, "ogg_static.lib")
#pragma comment(lib, "vorbis_static.lib")
#pragma comment(lib, "vorbisfile_static.lib")

#include <al/al.h>
#include <al/alc.h>
#include <al/alut.h>

#ifdef INCLUDE_FMOD_EX
	#pragma comment(lib, "fmodex_vc.lib")
	#include <fmod.hpp>
	#include <fmod_errors.h>
#endif

//For vorbis
#include <vorbis/vorbisfile.h>

#include "utils/log.h"
#include "utils/3DMATH.H"
//#include "CoreEngine.h"
//#include "camera.h" 

namespace DifferentialArts
{

	#if defined(WIN32) || defined(__WATCOMC__) || defined(_WIN32) || defined(__WIN32__)
		#define __PACKED                         // dummy 
	#else
		#define __PACKED __attribute__((packed)) // gcc packed 
	#endif


	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5

		class FMODHelperClass

	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/


	/*!	\class FMODHelperClass
		\brief FMOD helper class
		\author Rajesh Peter Douglas D'Monte	

		This class encapsulates many useful FMOD and sound
		necessities
	*/
	class RX_API FMODHelperClass
	{
	public:
		void MasterStopAllSounds(void);//!<Stop all sounds
		void MasterResumeAllSounds(void);//!<Resumes all sounds
		void SetVolumeForFMOD(void); //!< Set FMOD volume to the value in myGlobalGame->getApp()->volume
		bool LoadSound(const char* fileName, FMOD::Sound** ppSoundObj); //!< Loads a 3D sound
		bool LoadSoundLoop(const char* fileName, FMOD::Sound** ppSoundObj);//!< Loads a 3D sound loop sound
		bool LoadSound2D(const char* fileName, FMOD::Sound** ppSoundObj);//!< Loads a 2D sound
		bool LoadSoundLoop2D(const char* fileName, FMOD::Sound** ppSoundObj);//!< Loads a 2D sound loop sound
		bool LoadStream(const char* fileName, FMOD::Sound** ppSoundObj);//!< Loads a 3D streaming sound
		bool LoadStreamLoop(const char* fileName, FMOD::Sound** ppSoundObj);//!< Loads a 3D streaming loop sound
		bool LoadStream2D(const char* fileName, FMOD::Sound** ppSoundObj);//!< Loads a 2D streaming sound
		bool LoadStreamLoop2D(const char* fileName, FMOD::Sound** ppSoundObj);//!< Loads a 2D streaming loop sound

		bool SaveSoundToWave(const char* fileName, FMOD::Sound* pSoundObj); //!< Saves a FMOD sound object to a wave file

		bool CreateSoundForRecording(FMOD::Sound** ppSoundObj, int iLengthInSeconds); //!< Creates a sound for recording

		bool StartRecording(FMOD::Sound* ppSoundObj); //!< Start recording audio
		bool StopRecording(void); //!< Stop recording audio

		bool PlayRecordingChannel(FMOD::Sound* pSoundObj); //!< Plays the recording channel
		bool StopRecordingChannel(void); //!< Stops playing the recording channel

		//
		//	Use the below load functions if you want to do some spectrum analysis on your sound
		//

		bool LoadSoftwareStream(const char* fileName, FMOD::Sound** ppSoundObj);//!<Loads a software streaming sound (For DSP effects)
		bool LoadSoftwareStreamLoop(const char* fileName, FMOD::Sound** ppSoundObj);//!<Loads a software streaming loop (For DSP effects)
		bool LoadSoftwareSound(const char* fileName, FMOD::Sound** ppSoundObj);//!<Loads a software Sound (For DSP effects)
		bool LoadSoftwareSoundLoop(const char* fileName, FMOD::Sound** ppSoundObj);//!<Loads a software Sound loop(For DSP effects)

	protected:
		FMOD::Channel* mRecordingPlaybackChannel;
	};

	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5

		class SoundSystem

	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/


	/*!	\class SoundSystem
		\brief Reaction Engine Sound System
		\author Rajesh Peter Douglas D'Monte	

		This class is for using the Reaction Engine
		Sound system features
	*/
	class RX_API SoundSystem: public BaseMemoryInterface
	{
	public:
		SoundSystem(void); //!<Constructor
		~SoundSystem(void); //!<Deconstructor

		bool InitSoundSystem(bool useFMOD = true, int iNumberOfSources = 32); //!< Initializes the sound system
		void DeInitSoundSystem(void); //!< Deinitializes the sound system

		void SetListenerPositionAndVelocity(const Math::Vector3 &pos, const Math::Vector3 &vel); //!< Sets the listener position and velocity
		void SetListenerOrientation(const Math::Vector3 &heading, const Math::Vector3 &up); //!< Sets the listener orientation
		void SetDoppler(float factor, float velocity); //!< Only in OpenAL, use FMOD properties instead
		bool FMOD_ERRCHECK(FMOD_RESULT result); //!< Interprets FMOD resutls

		Math::Vector3 GetListenerPosition(void) const; //!< Returns the listener position
		Math::Vector3 GetListenerVelocity(void) const; //!< Returns the listener velocity

		int getTotalNumberOfPlaybackDrivers(void); //!< Returns the total number of playback drivers
		void getPlaybackDriverName(int driverIndex, const char* driverNamePointer); //!< Returns the driver name of a given playback driver index, driverNamePointer size should be 256
		bool setPlaybackDriver(int driverIndex); //!< Sets the playback device driver

		int getTotalNumberOfRecordDrivers(void); //!< Returns the total number of recording drivers
		void getRecordDriverName(int driverIndex, const char* driverNamePointer); //!< Returns the driver name of a given recording driver index, driverNamePointer size should be 256
		bool setRecordDriver(int driverIndex); //!< Sets the recording device driver

		//!This function works for only channel group groupSounds1
		void StopAllSounds(void);
		//!This function works for only channel group groupSounds1
		void ResumeAllSounds(void);

		/* FMOD Audio channel groups */
		#ifdef INCLUDE_FMOD_EX
			FMOD::System *mFMODSystem; //!< FMOD core system
			FMOD::ChannelGroup *  groupSounds1; //!< groupSounds1 channel group
			FMOD::ChannelGroup *  groupSounds2; //!< groupSounds2 channel group
			FMOD::ChannelGroup *  groupMusic1; //!< groupMusic1 channel group
			FMOD::ChannelGroup *  groupMusic2; //!< groupMusic2 channel group
			FMOD::ChannelGroup *  groupExtra1; //!< groupExtra1 channel group
			FMOD::ChannelGroup *  groupExtra2; //!< groupExtra2 channel group
		#endif

		FMODHelperClass mFMODHelper; //!< Instance of FMODHelperClass. Don't declare any duplicates

	protected:
		ALuint* mALSources;
		float* mSDepthBuffer;
		int mCount; //Keeps track of current source index
		int mMaxCount;
		bool usingOpenAL;

		//Keeps track of listener position and velocity globally
		Math::Vector3 mListenerPosition;
		Math::Vector3 mListenerVelocity;

		ALCcontext *mContext;
		ALCdevice *mDevice;
	};

	/*
	void SetAudioCameraera(CCameraera &Camera, float dvelocity);
	void SetAudioCameraeraStrafe(CCameraera &Camera, float dvelocity);
	*/

	#define MAX_AUDIO_SOURCES 256
	#define BUFFER_SIZE (4096 * 16)

	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5

		For OpenAL ONLY

	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/

	/*!	\class OpenALSound
		\brief Sound object for OpenAL
		\author Rajesh Peter Douglas D'Monte	

		This class is for using sound objects with OpenAL

		\depcrecated
		Use FMOD interfaces provided with Reaction instead

		@see FMODSource
	*/
	class RX_API OpenALSound
	{
	public:
		void start(void);
		void stop(void);
		void pause(void);
		void set(const Math::Vector3 &p, const Math::Vector3 &v);
		bool Load(char* fileName, bool loop);
		void Free(void);
		void loop(void);
		void noLoop(void);
		void setPitch(float p);
		void setGain(float g);
		void setPitchAndGain(float p, float g);
		bool useBuffer(OpenALSound* sound);
		unsigned int getBuffer(void);
		void resetBuffer(void);
		bool isPlaying(void);
		bool isLoaded(void);
		void makeRelative(void);
		void makeNotRelative(void);
		void setRollOff(float v);
		bool isLooped(void);

		ALuint* getBufferAddress(void); 
		ALenum* getalFormatBufferAddress(void); 
		ALvoid* getalBufferAddress(void);
		ALsizei getalFreqBuffer(void); 
		ALsizei getalBufferLen(void);

		OpenALSound();
		~OpenALSound();

	private:
		ALvoid* alBuffer;            //data for the buffer
		ALenum alFormatBuffer;     //for the buffer format
		ALsizei alFreqBuffer;      //for the frequency of the buffer
		ALsizei alBufferLen;          //the bit depth
		bool alLoop;			   //looped
		ALuint alSource;     //buffer source
		ALuint Buffer; 
		float pitch, gain;
		char soundFileName[255];
		bool loaded;
		Math::Vector3 pos;
		Math::Vector3 vel;

		void applySound(void);
	};

	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5

		Streaming audio class, supports only *.ogg files (OpenAL only)

	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/
	/*!	\class OpenALStream
		\brief Streaming sound object for OpenAL
		\author Rajesh Peter Douglas D'Monte	

		This class is for using sound objects with OpenAL

		\depcrecated
		Use FMOD interfaces provided with Reaction instead

		@see FMODSource
	*/
	class RX_API OpenALStream
	{
		public:

			void open(char* path);
			void Free();
			void display();
			bool playback();
			bool playing(void);
			bool update(void);
			void play(void);
			void pauseit(void);
			void unPause(void);
			void switchPause(void);
			bool getPause(void);
			OpenALStream(void);
			~OpenALStream(void);

		protected:

			bool stream(ALuint buffer);
			void empty();
			void check();
			void errorString(int code);

		private:

			FILE*           oggFile;
			OggVorbis_File  oggStream;
			vorbis_info*    vorbisInfo;
			vorbis_comment* vorbisComment;
			char soundFileName[255];

			ALuint buffers[2];
			ALuint source;
			ALenum format;
			bool pause;
	};

	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5

		For FMOD Only

	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/

	/*!	\class FMODSource
		\brief FMOD sound source object
		\author Rajesh Peter Douglas D'Monte	

		This class is for using sound objects with FMOD
	*/
	class RX_API FMODSource
	{
	public:
		FMODSource(void); //!< Constructor
		FMODSource(FMOD::Sound* soundObj); //!< Constructor, binds the FMOD sound source with FMOD sound data object. This is required.
		~FMODSource(void);//!< Destructor
		void set(const Math::Vector3 &pos, const Math::Vector3 &vel); //!< Set sound position and velocity

		//Sets the variables in question
		//But doesn't update the channel data with the variables in question
		void setWithoutUpdatingChannel(const Math::Vector3 &pos, const Math::Vector3 &vel);  //!<Sets the the member data without updating FMOD channel data.
		void play(void);//!< Plays the sound source
		void pause(void);//!< Pauses the sound source
		void resume(void);//!< Resumes the sound source
		void stop(void);//!< Stops the sound source
		void setSound(FMOD::Sound* soundObj, FMOD::ChannelGroup* channelGroup);//!< Sets the pointer to the FMOD sound data object
		bool isPaused(void);//!< Returns true if paused
		bool isPlaying(void);//!< Returns true if playing

		FMOD::Sound* getSoundObject(void); //!< Returns the pointer to the source's FMOD sound data object

		FMOD::Channel* mChannel; //!< The sound sources channel. Every sound source has its own channel
		FMOD_VECTOR mPos; //!< Use set() to modify this value
		FMOD_VECTOR mVel; //!< Use set() to modify this value

		void Free(void); //!< Doesn't actually free anything. You'll have to free the FMOD sound data externally, since FMODSource objects are virtual.

	private:
		FMOD::Sound* mSoundObj;
		FMOD::ChannelGroup* channelGroup;

	};

	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5

		Various Audio source objects for gameplay (FOR FMOD sources only :D )

	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/

	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
		RX_FMOD_AUDIO_CHANNELS enumerations
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/

	/*! \enum RX_FMOD_AUDIO_CHANNELS
		\brief RX sound system channel groups enumeration
	*/
	typedef enum RX_API tRX_FMOD_AUDIO_CHANNELS
	{
		RX_SOUNDS_GROUP_1, //!< Primary sound channel group
		RX_SOUNDS_GROUP_2,//!< Secondary sound channel group
		RX_MUSIC_GROUP_1,//!< Primary music channel group
		RX_MUSIC_GROUP_2,//!< Secondary music channel group
		RX_EXTRA_GROUP_1,//!< Primary extra channel group
		RX_EXTRA_GROUP_2,//!< Secondary extra channel group
	} RX_FMOD_AUDIO_CHANNELS;

	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
		Audio Point emitter source
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/

	/*!	\class SoundEmitterPoint
		\brief A 3D sound emitter class
		\author Rajesh Peter Douglas D'Monte	

		This class is for using point source sound emitters
	*/
	class RX_API SoundEmitterPoint
	{
	public:
		SoundEmitterPoint(void);//!< Constructor
		~SoundEmitterPoint(void);//!< Destructor

		
		virtual bool Init(FMOD::Sound* pSoundObj, RX_FMOD_AUDIO_CHANNELS rxChannel/*use the RX_FMOD_AUDIO_CHANNELS for this param*/,
				  float _3dminStart = -1, float _3dmaxEnd = -1);
		//!< Initializes the Emitter. Read the detailed description for more information;	
		/*!<
			Note many FMODSource objects uses a common FMOD::Sound* object for sharing a common audio buffer
			If you set _3dminStart and _3dmaxEnd for any audio emitter, it changes the 3dsoundminmax settings for
			all the respective audio emitters using the same FMOD::Sound* mSoundObj;
			Leave _3dmaxEnd, or _3dminStart as -1 to leave the 3dsoundminmax settings unchanged in the FMOD::Sound* mSoundObj

			\param pSoundObj The FMOD sound object data
			\param rxChannel The channel group this source resides under
			\param _3dminStart 3D Min audio start (not exactly roll off)
			\param _3dmaxEnd 3D max audio end (not exactly roll off)

			@return true if the method succeeds
		*/
		
		virtual void UpdateEmitter(void); //!< Updates the emitter. Read the detailed description
		/*!<
			Call this function after you set the mFMODSource objects position, it will re-update the position
			based on the type of audio emitter call this function after calling FMODSource::setWithoutUpdatingChannel
			Only call this when the sound position changes
		*/

		FMODSource mFMODSource;
		//!< Reaction engine FMOD sound source object, use this for changing position or velocity

		FMOD_VECTOR pos; //!<only use pos for debugging purposes. Read detailed description.
		/*!< This is not to be used, UpdateEmitter() modifies these values as it see's fit.
			So its highly recommended you don't modify this value.
		*/
		FMOD_VECTOR vel; //!<only use vel for debugging purposes. Read detailed description.
		/*!< This is not to be used, UpdateEmitter() modifies these values as it see's fit.
			So its highly recommended you don't modify this value.
		*/

	protected:
	};

	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
		Audio Point emitter source
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/

	/*!	\class SoundEmitterSphere
		\brief A 3D sound sphere emitter class
		\author Rajesh Peter Douglas D'Monte	

		This class is for using spherical shaped sound emitters
	*/
	class RX_API SoundEmitterSphere: public SoundEmitterPoint
	{
	public:
		SoundEmitterSphere(void);//!< Constructor
		~SoundEmitterSphere(void);//!< Destructor
		
		virtual bool Init(FMOD::Sound* pSoundObj, RX_FMOD_AUDIO_CHANNELS rxChannel/*use the RX_FMOD_AUDIO_CHANNELS for this param*/,
						  float r, float _3dminStart = -1, float _3dmaxEnd = -1);
		//!< Initializes the Emitter. Read the detailed description for more information;	
		/*!<
			Note many FMODSource objects uses a common FMOD::Sound* object for sharing a common audio buffer
			If you set _3dminStart and _3dmaxEnd for any audio emitter, it changes the 3dsoundminmax settings for
			all the respective audio emitters using the same FMOD::Sound* mSoundObj;
			Leave _3dmaxEnd, or _3dminStart as -1 to leave the 3dsoundminmax settings unchanged in the FMOD::Sound* mSoundObj

			\param pSoundObj The FMOD sound object data
			\param rxChannel The channel group this source resides under
			\param r Radius of the emitting sphere
			\param _3dminStart 3D Min audio start (not exactly roll off)
			\param _3dmaxEnd 3D max audio end (not exactly roll off)

			@return true if the method succeeds
		*/
		
		virtual void UpdateEmitter(void); //!< Updates the emitter. Read the detailed description
		/*!<
			Call this function after you set the mFMODSource objects position, it re updates the position
			based on the type of audio emitter
			call this function after calling FMODSource::setWithoutUpdatingChannel
			Only call this.. when the sound position changes AND when user position changes
		*/

		void SetRadius(float r); //!< Sets the radius of the spherical sound emitter

	protected:
		float mfRadius;
	};

	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
		Audio Line emitter source
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/

	/*!	\class SoundEmitterLine
		\brief A 3D sound line emitter class
		\author Rajesh Peter Douglas D'Monte	

		This class is for using line sound emitters
	*/
	class RX_API SoundEmitterLine: public SoundEmitterPoint
	{
	public:
		SoundEmitterLine(void);//!< Constructor
		~SoundEmitterLine(void);//!< Destructor

		virtual bool Init(FMOD::Sound* pSoundObj, RX_FMOD_AUDIO_CHANNELS rxChannel/*use the RX_FMOD_AUDIO_CHANNELS for this param*/,
						  const Math::Vector3 &startPoint, const Math::Vector3 &endPoint, float _3dminStart = -1, float _3dmaxEnd = -1);	

		//!< Initializes the Emitter. Read the detailed description for more information;	
		/*!<
			Note many FMODSource objects uses a common FMOD::Sound* object for sharing a common audio buffer
			If you set _3dminStart and _3dmaxEnd for any audio emitter, it changes the 3dsoundminmax settings for
			all the respective audio emitters using the same FMOD::Sound* mSoundObj;
			Leave _3dmaxEnd, or _3dminStart as -1 to leave the 3dsoundminmax settings unchanged in the FMOD::Sound* mSoundObj

			\param pSoundObj The FMOD sound object data
			\param rxChannel The channel group this source resides under
			\param startPoint The starting point of the line
			\param endPoint The ending point of the line
			\param _3dminStart 3D Min audio start (not exactly roll off)
			\param _3dmaxEnd 3D max audio end (not exactly roll off)

			@return true if the method succeeds
		*/
		
		virtual void UpdateEmitter(void);//!< Updates the emitter. Read the detailed description
		/*!<
			Unlike SoundEmitterPoint, you need to call this not only when the sound source changes, 
			But also when the user position changes 
			do not call the any set methods of the mFMODSource object, it is not required.
		*/

		void SetEndPoints(const Math::Vector3 &startPoint, const Math::Vector3 &endPoint); //!< Sets the start and ending points of the line sound emitter

	protected:
		Math::Vector3 m_vStartPoint, m_vEndPoint;
	};

	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
		Audio Box emitter source
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/

	/*!	\class SoundEmitterBox
		\brief A 3D sound box emitter class
		\author Rajesh Peter Douglas D'Monte	

		This class is for using box sound emitters
	*/
	class RX_API SoundEmitterBox: public SoundEmitterPoint
	{
	public:
		SoundEmitterBox(void);//!< Constructor
		~SoundEmitterBox(void);//!< Destructor
		
		virtual bool Init(FMOD::Sound* pSoundObj, RX_FMOD_AUDIO_CHANNELS rxChannel/*use the RX_FMOD_AUDIO_CHANNELS for this param*/,
						  const Math::Vector3 &minSize, const Math::Vector3 &maxSize, float _3dminStart = -1, float _3dmaxEnd = -1);	
		//!< Initializes the Emitter. Read the detailed description for more information;	
		/*!<
			Note many FMODSource objects uses a common FMOD::Sound* object for sharing a common audio buffer
			If you set _3dminStart and _3dmaxEnd for any audio emitter, it changes the 3dsoundminmax settings for
			all the respective audio emitters using the same FMOD::Sound* mSoundObj;
			Leave _3dmaxEnd, or _3dminStart as -1 to leave the 3dsoundminmax settings unchanged in the FMOD::Sound* mSoundObj

			\param pSoundObj The FMOD sound object data
			\param rxChannel The channel group this source resides under
			\param minSize The negative extents of the box
			\param maxSize The positive extents of the box
			\param _3dminStart 3D Min audio start (not exactly roll off)
			\param _3dmaxEnd 3D max audio end (not exactly roll off)

			@return true if the method succeeds
		*/

		virtual void UpdateEmitter(void);//!<Call every time user position or dimensions change

		void SetDimensions(const Math::Vector3 &minSize, const Math::Vector3 &maxSize);//!< Sets the dimensions of the box	

	protected:
		Math::Vector3 m_vMinSize, m_vMaxSize;
	};
}

#endif