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

   class SoundEmitterPoint
				|
				================> class SoundEmitterSphere
				|
				================> class SoundEmitterLine
				|
				================> class SoundEmitterBox

==========================================================================
*/

#include "CoreEngine.h" //Ignore the fact that it doesn't include openALClass.h directly

namespace DifferentialArts
{
	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5

		class SoundSystem

	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/

	SoundSystem::SoundSystem(void)
	{
		mContext = NULL;
		mDevice = NULL;

		mCount = 0;
	}

	//This function works for only channel set 1
	void SoundSystem::StopAllSounds(void)
	{
		if(!myGlobalGame->usingEngineSound())
		{
			return;
		}

		FMOD_RESULT result;
		result = this->groupSounds1->setPaused(true);
		//this->groupSounds1->setPaused(

		if(result != FMOD_OK)
		{
			myGlobalGame->mlog.LogError(LString("Error accessing sound channel group using FMOD"));
			FMOD_ERRCHECK(result);
			return;
		}
	}

	//This function works for only channel set 1
	void SoundSystem::ResumeAllSounds(void)
	{
		if(!myGlobalGame->usingEngineSound())
		{
			return;
		}

		FMOD_RESULT result;
		result = this->groupSounds1->setPaused(false);

		if(result != FMOD_OK)
		{
			myGlobalGame->mlog.LogError(LString("Error accessing sound channel group using FMOD"));
			FMOD_ERRCHECK(result);
			return;
		}
	}

	Math::Vector3 SoundSystem::GetListenerPosition(void) const
	{
		return this->mListenerPosition;
	}

	Math::Vector3 SoundSystem::GetListenerVelocity(void) const
	{
		return this->mListenerVelocity;
	}

	/*
	=================================================================
		PLAYBACK DRIVER STUFF
	=================================================================
	*/

	int SoundSystem::getTotalNumberOfPlaybackDrivers(void)
	{
		int num;

		FMOD_RESULT result;

		result = this->mFMODSystem->getNumDrivers(&num);

		FMOD_ERRCHECK(result);

		return num;
	}

	void SoundSystem::getPlaybackDriverName(int driverIndex, const char* driverNamePointer)
	{
		FMOD_RESULT result;
		char name[256];

		
		result = this->mFMODSystem->getDriverInfo(driverIndex, name, 256, 0);   
		FMOD_ERRCHECK(result);
	}

	bool SoundSystem::setPlaybackDriver(int driverIndex)
	{
		int num;
		FMOD_RESULT result;

		result = this->mFMODSystem->getNumDrivers(&num);

		FMOD_ERRCHECK(result);

		if(driverIndex < 0 || driverIndex >= num)
		{
			return false;
		}

		result = mFMODSystem->setDriver(driverIndex);

		if(!FMOD_ERRCHECK(result))
		{
			return false;
		}

		return  true;
	}

	/*
	=================================================================
		RECORDING DRIVER STUFF
	=================================================================
	*/

	int SoundSystem::getTotalNumberOfRecordDrivers(void)
	{
		int num;

		FMOD_RESULT result;

		result = this->mFMODSystem->getRecordNumDrivers(&num);

		FMOD_ERRCHECK(result);

		return num;
	}

	void SoundSystem::getRecordDriverName(int driverIndex, const char* driverNamePointer)
	{
		FMOD_RESULT result;
		char name[256];

		result = this->mFMODSystem->getRecordDriverInfo(driverIndex, name, 256, 0);   
		FMOD_ERRCHECK(result);
	}

	bool SoundSystem::setRecordDriver(int driverIndex)
	{
		int num;
		FMOD_RESULT result;

		result = this->mFMODSystem->getRecordNumDrivers(&num);

		FMOD_ERRCHECK(result);

		if(driverIndex < 0 || driverIndex >= num)
		{
			return false;
		}

		result = mFMODSystem->setRecordDriver(driverIndex);

		if(!FMOD_ERRCHECK(result))
		{
			return false;
		}

		return  true;
	}

	bool SoundSystem::FMOD_ERRCHECK(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			char    errstring[1024];

			sprintf(errstring, "FMOD error! (%d) %s", result, FMOD_ErrorString(result));

			myGlobalGame->mlog.LogError(LString("%s", errstring));
			return false;
		}

		return true;
	}

	void SoundSystem::SetDoppler(float factor, float velocity)
	{
		if(myGlobalGame->isUsingOpenAL())
		{
			alDopplerFactor(factor);
			alDopplerVelocity(velocity);
		} else {
			myGlobalGame->mlog.LogError(LString("When using fmod, please set doppler from this->mFMODSystem"));
		}
	}

	void SoundSystem::SetListenerPositionAndVelocity(const Math::Vector3 &pos, const Math::Vector3 &vel)
	{
		mListenerPosition = pos;
		mListenerVelocity = vel;

		if(myGlobalGame->isUsingOpenAL())
		{	
			alListener3f(AL_POSITION, pos.x,pos.y,pos.z);
			alListener3f(AL_VELOCITY, vel.x,vel.y,vel.z);
		} else {
			FMOD_VECTOR mPos, mVel;

			mPos.x = pos.x;
			mPos.y = pos.y;
			mPos.z = pos.z;

			mVel.x = vel.x;
			mVel.y = vel.y;
			mVel.z = vel.z;

			this->mFMODSystem->set3DListenerAttributes(0, &mPos, &mVel, 0, 0);
		}
	}

	void SoundSystem::SetListenerOrientation(const Math::Vector3 &heading, const Math::Vector3 &up)
	{
		if(myGlobalGame->isUsingOpenAL())
		{
			//set the orientation using an array of floats
			float vector_orientation[6];
			vector_orientation[0] = heading.x;
			vector_orientation[1] = heading.y;
			vector_orientation[2] = heading.z;
			vector_orientation[3] = up.x;
			vector_orientation[4] = up.y;
			vector_orientation[5] = up.z;
			alListenerfv(AL_ORIENTATION, vector_orientation);
		} else {
			FMOD_VECTOR mUp, mForward;

			mUp.x = up.x;
			mUp.y = up.y;
			mUp.z = up.z;

			mForward.x = heading.x;
			mForward.y = heading.y;
			mForward.z = heading.z;

			this->mFMODSystem->set3DListenerAttributes(0, 0, 0, &mForward, &mUp);
		}
		
	}

	bool SoundSystem::InitSoundSystem(bool useFMOD, int iNumberOfSources)
	{	
		Console* myLog = &myGlobalGame->mlog;

		if(!myGlobalGame->bUseEngineSound)
		{
			myLog->LogInfo(LString("Application not using Engine Sound System!"));
			return true;
		}	

		if(useFMOD)
		{
			myLog->LogInfo(LString("Init FMOD started..."));

			FMOD_RESULT result;
			unsigned int version;

			result = FMOD::System_Create(&this->mFMODSystem);		// Create the main system object.
			FMOD_ERRCHECK(result);

			result = this->mFMODSystem->getVersion(&version);
			FMOD_ERRCHECK(result);

			if (version < FMOD_VERSION)
			{
				myLog->LogError(LString("Error!  You are using an old version of FMOD %08x.  This program requires %08x", version, FMOD_VERSION));
				myGlobalGame->getApp()->end();
			} else {
				myLog->LogInfo(LString("You are using FMOD %08x.  Program Compiled using FMOD %08x. Its OK!", version, FMOD_VERSION));
			}

			result = this->mFMODSystem->setOutput(FMOD_OUTPUTTYPE_DSOUND);	
			FMOD_ERRCHECK(result);

			result = this->mFMODSystem->init(iNumberOfSources, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);	// Initialize FMOD.
			FMOD_ERRCHECK(result);

			int _2dc, _3dc, tchannels;
			result = this->mFMODSystem->getHardwareChannels(&_2dc, &_3dc, &tchannels);
			FMOD_ERRCHECK(result);

			myLog->LogInfo(LString("Total 2D Audio Channels: %d", _2dc));
			myLog->LogInfo(LString("Total 3D Audio Channels: %d", _3dc));
			myLog->LogInfo(LString("Total Audio Channels: %d", tchannels));

			/* Create channel groups */
			myLog->LogInfo(LString("Creating channel groups for myGlobalGame object (class CoreEngine)..."));

			//Sound Group 1
			myLog->LogInfo(LString("Creating Sound Group 1 channel group..."));
			result = this->mFMODSystem->createChannelGroup("Sound Group 1", &this->groupSounds1);

			if(result != FMOD_OK)
			{
				myLog->LogError(LString("Failed to created Sound Group 1 channel group"));
				FMOD_ERRCHECK(result);
			} else {
				myLog->LogSuccessText(LString("Successfully created Sound Group 1 channel group!"));
			}

			//Sound Group 2
			myLog->LogInfo(LString("Creating Sound Group 2 channel group..."));
			result = this->mFMODSystem->createChannelGroup("Sound Group 2", &this->groupSounds2);

			if(result != FMOD_OK)
			{
				myLog->LogError(LString("Failed to created Sound Group 2 channel group"));
				FMOD_ERRCHECK(result);
			} else {
				myLog->LogSuccessText(LString("Successfully created Sound Group 2 channel group!"));
			}

			//Music Group 1
			myLog->LogInfo(LString("Creating Music Group 1 channel group..."));
			result = this->mFMODSystem->createChannelGroup("Music Group 1", &this->groupMusic1);

			if(result != FMOD_OK)
			{
				myLog->LogError(LString("Failed to created Music Group 1 channel group"));
				FMOD_ERRCHECK(result);
			} else {
				myLog->LogSuccessText(LString("Successfully created Music Group 1 channel group!"));
			}

			//Music Group 2
			myLog->LogInfo(LString("Creating Music Group 2 channel group..."));
			result = this->mFMODSystem->createChannelGroup("Music Group 2", &this->groupMusic2);

			if(result != FMOD_OK)
			{
				myLog->LogError(LString("Failed to created Music Group 2 channel group"));
				FMOD_ERRCHECK(result);
			} else {
				myLog->LogSuccessText(LString("Successfully created Music Group 2 channel group!"));
			}

			//Extra Sound Group 1
			myLog->LogInfo(LString("Creating Extra Sound Group 1 channel group..."));
			result = this->mFMODSystem->createChannelGroup("Extra Sound Group 1", &this->groupExtra1);

			if(result != FMOD_OK)
			{
				myLog->LogError(LString("Failed to created Extra Sound Group 1 channel group"));
				FMOD_ERRCHECK(result);
			} else {
				myLog->LogSuccessText(LString("Successfully created Extra Sound Group 1 channel group!"));
			}

			//Extra Sound Group 2
			myLog->LogInfo(LString("Creating Extra Sound Group 2 channel group..."));
			result = this->mFMODSystem->createChannelGroup("Extra Sound Group 2", &this->groupExtra2);

			if(result != FMOD_OK)
			{
				myLog->LogError(LString("Failed to created Extra Sound Group 2 channel group"));
				FMOD_ERRCHECK(result);
			} else {
				myLog->LogSuccessText(LString("Successfully created Extra Sound Group 2 channel group!"));
			}  

			FMOD::ChannelGroup* channelgroup;
			result = this->mFMODSystem->getMasterChannelGroup(&channelgroup);	

			if(result != FMOD_OK)
			{
				myGlobalGame->mlog.LogError(LString("Error accessing master channel group using FMOD"));
				FMOD_ERRCHECK(result);
				return false;
			}

			channelgroup->addGroup(this->groupSounds1);
			channelgroup->addGroup(this->groupSounds2);
			channelgroup->addGroup(this->groupExtra1);
			channelgroup->addGroup(this->groupExtra2);
			channelgroup->addGroup(this->groupMusic1);
			channelgroup->addGroup(this->groupMusic2);

			myLog->LogSuccessText(LString("Succesfull Init of FMOD!"));
		} else {
			myLog->LogInfo(LString("Init of DirectSound3D using OpenAL has started..."));

			mDevice = alcOpenDevice("DirectSound3D");

			if(NULL == mDevice)
			{
				myLog->LogError(LString("Failed to initialize DirectSound3D using OpenAL"));
				return false;
			};
			myLog->LogSuccessText(LString("Succesfull openAL DeviceOpen"));

			mContext = alcCreateContext(mDevice, NULL);
			//Set active context to myContext
			alcMakeContextCurrent(mContext);	
			// Clear Error Code
			if (alcGetError(0) != ALC_NO_ERROR)
			{
				myLog->LogError(LString("Failed to initialize DirectSound3D using OpenAL"));
				return false;
			}


			/*
				Code to get total free audio channels on system
			*/
			int error = AL_NO_ERROR;
			int srcCount = 0;
			ALuint alSource[MAX_AUDIO_SOURCES];

			/* Generate sources until you can generate no more, keep track of the count */
			while(error == AL_NO_ERROR)
			{
				alGenSources(1, &alSource[srcCount]);	

				error = alGetError();

				srcCount++;
			}


			/* Delete those sources */
			//alDeleteSources(srcCount, alSource);
			for(int i = 0;i<srcCount;i++)
			{
				alDeleteSources(1, &alSource[i]);
			}

			mMaxCount = srcCount;

			/* Create source arrays and sound depth buffer array */
			mALSources = new ALuint[mMaxCount];
			mSDepthBuffer = new float[mMaxCount];

			if(!mALSources)
			{
				myLog->LogError(LString("globalALSources was not initialized.. Insufficient memory"));
				return false;
			} else {
				myLog->LogSuccessText(LString("globalALSources was initialized successfully!"));
			}

			if(!mSDepthBuffer)
			{
				myLog->LogError(LString("gSDepthBuffer was not initialized.. Insufficient memory"));
				return false;
			} else {
				myLog->LogSuccessText(LString("gSDepthBuffer was initialized successfully!"));
			}

			myLog->LogInfo(LString("Free Audio Channels on system: %d", srcCount));

			/*
				End of Code to get total free audio channels on system
			*/

			myGlobalGame->bUseOpenAL = true;

			myLog->LogSuccessText(LString("Succesfull Init of DirectSound3D using OpenAL!"));
		}

		return true;
	}

	void SoundSystem::DeInitSoundSystem(void)
	{
		Console* myLog = &myGlobalGame->mlog;

		if(!myGlobalGame->bUseEngineSound)
		{
			myLog->LogInfo(LString("No audio capabilities were used"));
			return;
		}

		if(myGlobalGame->isUsingOpenAL())
		{
			/* Delete Create source arrays and sound depth buffer array */
			if(mALSources)
			{
				delete[] mALSources;
				mALSources = 0;
				myLog->LogSuccessText(LString("globalALSources succesfully released"));
			}

			if(mSDepthBuffer)
			{
				delete[] mSDepthBuffer;
				mSDepthBuffer = 0;
				myLog->LogSuccessText(LString("gSDepthBuffer succesfully released"));
			}

			myLog->LogInfo(LString("Shutdown of DirectSound3D using OpenAL has started..."));
			//Get currently active context
			mContext=alcGetCurrentContext();
			//Get the device for currently active context
			mDevice=alcGetContextsDevice(mContext);
			//Disable the present context
			alcMakeContextCurrent(NULL);
			alcDestroyContext(mContext);
			alcCloseDevice(mDevice);

			myGlobalGame->bUseOpenAL = false;

			myLog->LogSuccessText(LString("Succesfull shutdown of DirectSound3D using OpenAL!"));
		} else {
			if(this->mFMODSystem)
			{	
				FMOD_RESULT result;
				myLog->LogInfo(LString("Freeing channel groups for myGlobalGame object (class CoreEngine)..."));

				result = this->groupExtra1->release();
				FMOD_ERRCHECK(result);

				result = this->groupExtra2->release();
				FMOD_ERRCHECK(result);

				result = this->groupSounds1->release();
				FMOD_ERRCHECK(result);

				result = this->groupSounds2->release();
				FMOD_ERRCHECK(result);

				result = this->groupMusic1->release();
				FMOD_ERRCHECK(result);

				result = this->groupMusic2->release();
				FMOD_ERRCHECK(result);

				myLog->LogSuccessText(LString("Succesfull freeing of FMOD Sound Channel groups!"));

				result = this->mFMODSystem->release();
				FMOD_ERRCHECK(result);
				myLog->LogSuccessText(LString("Succesfull shutdown of FMOD!"));
			}
		}
	}

	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5

		class FMODHelperClass

	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/

	void FMODHelperClass::MasterResumeAllSounds(void)
	{
		FMOD::ChannelGroup* channelgroup;
		FMOD_RESULT result;
		result = myGlobalGame->mSoundSystem->mFMODSystem->getMasterChannelGroup(&channelgroup);	

		if(result != FMOD_OK)
		{
			myGlobalGame->mlog.LogError(LString("Error accessing master channel group using FMOD"));
			//FMOD_ERRCHECK(result);
			return;
		}

		channelgroup->setPaused(false);
	}

	void FMODHelperClass::MasterStopAllSounds(void)
	{
		FMOD::ChannelGroup* channelgroup;
		FMOD_RESULT result;
		result = myGlobalGame->mSoundSystem->mFMODSystem->getMasterChannelGroup(&channelgroup);	

		if(result != FMOD_OK)
		{
			myGlobalGame->mlog.LogError(LString("Error accessing master channel group using FMOD"));
			//FMOD_ERRCHECK(result);
			return;
		}

		channelgroup->setPaused(true);
	}

	bool FMODHelperClass::LoadSoundLoop(const char* fileName, FMOD::Sound** ppSoundObj)
	{
		FMOD_RESULT result; 

		result = myGlobalGame->mSoundSystem->mFMODSystem->createSound(fileName,
																	  FMOD_3D | //| FMOD_CREATESAMPLE |
																	  FMOD_3D_IGNOREGEOMETRY |
																	  FMOD_LOOP_NORMAL,
																	  0, ppSoundObj);

		return myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);
	}

	bool FMODHelperClass::LoadSound(const char* fileName, FMOD::Sound** ppSoundObj)
	{
		FMOD_RESULT result; 

		result = myGlobalGame->mSoundSystem->mFMODSystem->createSound(fileName,
																	  FMOD_3D |// FMOD_CREATESAMPLE |
																	  FMOD_3D_IGNOREGEOMETRY,
																	  0, ppSoundObj);
		return myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);
	}

	bool FMODHelperClass::LoadSoundLoop2D(const char* fileName, FMOD::Sound** ppSoundObj)
	{
		FMOD_RESULT result; 

		result = myGlobalGame->mSoundSystem->mFMODSystem->createSound(fileName,
																	  FMOD_2D|FMOD_LOOP_NORMAL,
																	  0, ppSoundObj);
		return myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);
	}

	bool FMODHelperClass::LoadSound2D(const char* fileName, FMOD::Sound** ppSoundObj)
	{
		FMOD_RESULT result; 

		result = myGlobalGame->mSoundSystem->mFMODSystem->createSound(fileName,
																	  FMOD_2D,
																	  0, ppSoundObj);
		return myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);
	}

	bool FMODHelperClass::LoadStreamLoop(const char* fileName, FMOD::Sound** ppSoundObj)
	{
		FMOD_RESULT result; 

		result = myGlobalGame->mSoundSystem->mFMODSystem->createStream(fileName,
																	   FMOD_3D |// FMOD_CREATESAMPLE
																	   FMOD_3D_IGNOREGEOMETRY |
																	   FMOD_LOOP_NORMAL,
																	   0, ppSoundObj);
		return myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);
	}

	bool FMODHelperClass::LoadStream(const char* fileName, FMOD::Sound** ppSoundObj)
	{
		FMOD_RESULT result; 

		result = myGlobalGame->mSoundSystem->mFMODSystem->createStream(fileName,
																	   FMOD_3D | // FMOD_CREATESAMPLE
																	   FMOD_3D_IGNOREGEOMETRY,
																	   0, ppSoundObj);
		return myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);
	}

	bool FMODHelperClass::LoadStreamLoop2D(const char* fileName, FMOD::Sound** ppSoundObj)
	{
		FMOD_RESULT result; 

		result = myGlobalGame->mSoundSystem->mFMODSystem->createStream(fileName,
																	   FMOD_2D | // FMOD_CREATESAMPLE													
																	   FMOD_LOOP_NORMAL,
																	   0, ppSoundObj);
		return myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);
	}

	bool FMODHelperClass::SaveSoundToWave(const char* fileName, FMOD::Sound* pSoundObj)
	{
		FILE *fp;
		int             channels, bits;
		float           rate;
		void           *ptr1, *ptr2;
		unsigned int    lenbytes, len1, len2;

		if (!pSoundObj)
		{
			return false;
		}

		pSoundObj->getFormat  (0, 0, &channels, &bits);
		pSoundObj->getDefaults(&rate, 0, 0, 0);
		pSoundObj->getLength  (&lenbytes, FMOD_TIMEUNIT_PCMBYTES);

		{		
			#if defined(WIN32) || defined(_WIN64) || defined(__WATCOMC__) || defined(_WIN32) || defined(__WIN32__)
			#pragma pack(1)
			#endif	
	        
	        
			//    WAV Structures        
			typedef struct
			{
				signed char id[4];
				int 		size;
			} RiffChunk;
	    
			struct
			{
				RiffChunk       chunk           __PACKED;
				unsigned short	wFormatTag      __PACKED;    // format type  
				unsigned short	nChannels       __PACKED;    // number of channels (i.e. mono, stereo...)  
				unsigned int	nSamplesPerSec  __PACKED;    // sample rate  
				unsigned int	nAvgBytesPerSec __PACKED;    // for buffer estimation  
				unsigned short	nBlockAlign     __PACKED;    // block size of data  
				unsigned short	wBitsPerSample  __PACKED;    // number of bits per sample of mono data 
			} FmtChunk  = { {{'f','m','t',' '}, sizeof(FmtChunk) - sizeof(RiffChunk) }, 1, channels, (int)rate, (int)rate * channels * bits / 8, 1 * channels * bits / 8, bits } __PACKED;

			struct
			{
				RiffChunk   chunk;
			} DataChunk = { {{'d','a','t','a'}, lenbytes } };

			struct
			{
				RiffChunk   chunk;
				signed char rifftype[4];
			} WavHeader = { {{'R','I','F','F'}, sizeof(FmtChunk) + sizeof(RiffChunk) + lenbytes }, {'W','A','V','E'} };

			
			#if defined(WIN32) || defined(_WIN64) || defined(__WATCOMC__) || defined(_WIN32) || defined(__WIN32__)
			#pragma pack()
			#endif				

			fp = fopen(fileName, "wb");
	       
	        
			//   Write out the WAV header.        
			fwrite(&WavHeader, sizeof(WavHeader), 1, fp);
			fwrite(&FmtChunk, sizeof(FmtChunk), 1, fp);
			fwrite(&DataChunk, sizeof(DataChunk), 1, fp);

	        
			//   Lock the sound to get access to the raw data.        
			pSoundObj->lock(0, lenbytes, &ptr1, &ptr2, &len1, &len2);

	        
			//    Write it to disk.        
			fwrite(ptr1, len1, 1, fp);

	        
			//    Unlock the sound to allow FMOD to use it again.        
			pSoundObj->unlock(ptr1, ptr2, len1, len2);

			fclose(fp);
		}

		return true;
	}

	bool FMODHelperClass::LoadStream2D(const char* fileName, FMOD::Sound** ppSoundObj)
	{
		FMOD_RESULT result; 

		result = myGlobalGame->mSoundSystem->mFMODSystem->createStream(fileName,
														FMOD_2D,
														0, ppSoundObj);
		return myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);
	}

	bool FMODHelperClass::LoadSoftwareSoundLoop(const char* fileName, FMOD::Sound** ppSoundObj)
	{
		FMOD_RESULT result; 

		result = myGlobalGame->mSoundSystem->mFMODSystem->createSound(fileName,
														FMOD_SOFTWARE |// FMOD_CREATESAMPLE
														FMOD_3D_IGNOREGEOMETRY |
														FMOD_LOOP_NORMAL,
														0, ppSoundObj);
		return myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);
	}

	bool FMODHelperClass::LoadSoftwareSound(const char* fileName, FMOD::Sound** ppSoundObj)
	{
		FMOD_RESULT result; 

		result = myGlobalGame->mSoundSystem->mFMODSystem->createSound(fileName,
														FMOD_SOFTWARE |// FMOD_CREATESAMPLE|
														FMOD_3D_IGNOREGEOMETRY,
														0, ppSoundObj);
		return myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);
	}

	bool FMODHelperClass::LoadSoftwareStreamLoop(const char* fileName, FMOD::Sound** ppSoundObj)
	{
		FMOD_RESULT result; 

		result = myGlobalGame->mSoundSystem->mFMODSystem->createStream(fileName,
														FMOD_SOFTWARE |// FMOD_CREATESAMPL													
														FMOD_LOOP_NORMAL,
														0, ppSoundObj);
		return myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);
	}

	bool FMODHelperClass::LoadSoftwareStream(const char* fileName, FMOD::Sound** ppSoundObj)
	{
		FMOD_RESULT result; 

		result = myGlobalGame->mSoundSystem->mFMODSystem->createStream(fileName,
														FMOD_SOFTWARE,
														0, ppSoundObj);
		return myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);
	}

	bool FMODHelperClass::CreateSoundForRecording(FMOD::Sound** ppSoundObj, int iLengthInSeconds)
	{
		FMOD_CREATESOUNDEXINFO exinfo;
		FMOD_RESULT result; 

		memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));

		exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.numchannels      = 1;
		exinfo.format           = FMOD_SOUND_FORMAT_PCM16;
		exinfo.defaultfrequency = 44100;
		exinfo.length           = exinfo.defaultfrequency * sizeof(short) * exinfo.numchannels * iLengthInSeconds;
	    
		result = myGlobalGame->mSoundSystem->mFMODSystem->createSound(0, FMOD_2D | FMOD_SOFTWARE | FMOD_OPENUSER, &exinfo, ppSoundObj);

		return myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);
	}

	bool FMODHelperClass::StartRecording(FMOD::Sound* pSoundObj)
	{
		FMOD_RESULT result;

		result = myGlobalGame->mSoundSystem->mFMODSystem->recordStart(pSoundObj, false);
		return myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);
	}

	bool FMODHelperClass::StopRecording(void)
	{
		FMOD_RESULT result;

		result = myGlobalGame->mSoundSystem->mFMODSystem->recordStop();
		return myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);
	}

	bool FMODHelperClass::PlayRecordingChannel(FMOD::Sound* pSoundObj)
	{
		FMOD_RESULT result;

		mRecordingPlaybackChannel = 0;

		result = myGlobalGame->mSoundSystem->mFMODSystem->playSound(FMOD_CHANNEL_REUSE,
																	pSoundObj, false, &mRecordingPlaybackChannel);

		return myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);
	}

	bool FMODHelperClass::StopRecordingChannel(void)
	{
		FMOD_RESULT result;

		result = myGlobalGame->mSoundSystem->mFMODSystem->recordStop();

		if (mRecordingPlaybackChannel)
		{
			mRecordingPlaybackChannel->stop();
			mRecordingPlaybackChannel = 0;
		}

		return myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);
	}

	void FMODHelperClass::SetVolumeForFMOD(void)
	{
		FMOD::ChannelGroup *  channelgroup;
		FMOD_RESULT result;
		result = myGlobalGame->mSoundSystem->mFMODSystem->getMasterChannelGroup(&channelgroup);

		if(result != FMOD_OK)
		{
			myGlobalGame->mlog.LogError(LString("Error setting volume using FMOD"));
		}

		channelgroup->setVolume(myGlobalGame->getApp()->volume);
	}

	/*
	void SetAudioCameraera(CCameraera &Camera, float dvelocity)
	{
		heading = -Camera.Position() + Camera.View();

		SetListenerOrientation(heading, Camera.UpVector());

		SetListenerPositionAndVelocity(myCameraera.Position(), -heading*dvelocity);
	}

	void SetAudioCameraeraStrafe(CCameraera &Camera, float dvelocity)
	{
		heading = -Camera.Position() + Camera.View();

		SetListenerOrientation(heading, Camera.UpVector());

		SetListenerPositionAndVelocity(myCameraera.Position(), myCameraera.Strafe()*dvelocity);
	}
	*/

	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5

		For OpenAL ONLY

	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/

	void OpenALSound::start(void)
	{
		alSourcePlay(this->alSource);
	}

	void OpenALSound::stop(void)
	{
		alSourceStop(this->alSource);
	}

	void OpenALSound::set(const Math::Vector3 &p, const Math::Vector3 &v)
	{
		//set source position
		this->pos = p;
		alSource3f(alSource,AL_POSITION, pos.x, pos.y, pos.z);
		//set source velocity
		this->vel = v;
		alSource3f(alSource,AL_VELOCITY, vel.x, vel.y, vel.z);
	}

	bool OpenALSound::Load(char* fileName, bool loop)
	{
		if(this->loaded)
			return true;

		Console* myLog = &myGlobalGame->mlog;

		myLog->LogInfo(LString("Attempting to load sound file: \"%s\"", fileName));

		strcpy(this->soundFileName, fileName);

		strcpy(this->soundFileName, fileName);
		this->alLoop = loop;

		myLog->LogInfo(LString("Sound file name bytes copied to local soundFileName for \"%s\"", fileName));

		ALboolean tloop;
		
		int error = -1;	

		alGetError();
		// Load wav data into a buffer.
		alGenBuffers(1, &Buffer);
		myLog->LogInfo(LString("Sound buffer created for \"%s\"", fileName));

		if ((error = alGetError()) != AL_NO_ERROR)
		{
			myLog->LogError(LString("Could not load sound file, alGenBuffers in &Buffer failure: \"%s\"", fileName));
			this->Free();
			return false;
		}

		alutLoadWAVFile(fileName, &alFormatBuffer, &alBuffer, &alBufferLen, &alFreqBuffer, &tloop);

		if (alGetError() != AL_NO_ERROR)
		{
			myLog->LogError(LString("Could not load wav file, alutLoadWAVFile failure: \"%s\"", fileName));
			this->Free();
			return false;
		}

		alBufferData(Buffer, alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);

		if(alBuffer == NULL)
		{
			myLog->LogError(LString("Severe: Audio data is A NULL POINTER"));
		}
		
		if ((error = alGetError())!= AL_NO_ERROR)
		{
			myLog->LogError(LString("Could not load wav file, alBufferData failure: \"%s\", error no: %d", fileName, error));
			this->Free();
			return false;
		}	

		alutUnloadWAV(alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
		
		if (alGetError() != AL_NO_ERROR)
		{
			myLog->LogError(LString("Could not load wav file: \"%s\"", fileName));	
			this->Free();
			return false;
		}

		// Bind buffer with a source.

		alGenSources(1, &alSource);	

		if ((error = alGetError())!= AL_NO_ERROR)
		{
			myLog->LogError(LString("Could not load wav file, alGenSources failure: \"%s\", error no: %d", fileName, error));
			this->Free();
			return false;
		}

		this->pitch = 1.0f;
		this->gain = 1.0f;

		/* Binding buffer to source */
		alSourcei (alSource, AL_BUFFER,   Buffer   );
		alSourcei (alSource, AL_LOOPING,  alLoop );	
		alSourcef(alSource, AL_PITCH, this->pitch);
		alSourcef(alSource, AL_GAIN, this->gain);
		/* End of Binding buffer to source */

		// Do an error check and return.

		if (alGetError() != AL_NO_ERROR)
		{
			myLog->LogError(LString("Could not load wav file: \"%s\"", fileName));	
			this->Free();
			return false;
		}

		if(loop)
			this->loop();
		else
			this->noLoop();

		myLog->LogSuccessText(LString("Succesfull load of wav: \"%s\"", fileName));

		this->loaded = true;

		return true;
	}

	OpenALSound::OpenALSound()
	{
		this->loaded = false;
	}

	OpenALSound::~OpenALSound()
	{
		this->Free();
	}

	bool OpenALSound::isLoaded(void)
	{
		return this->loaded;
	}

	void OpenALSound::Free()
	{
		this->stop();
		//delete our buffer
		alDeleteBuffers(1,&Buffer);
		//delete our source
		alDeleteSources(1,&alSource);	
		if(this->loaded)
		{
			myGlobalGame->mlog.LogSuccessText(LString("%s has been succesfully released!", this->soundFileName));
			this->loaded = false;
		}
	}

	void OpenALSound::loop(void)
	{
		this->alLoop = AL_TRUE;
		//tell the sound to loop continuously
		alSourcei(alSource,AL_LOOPING,AL_TRUE);
	}

	void OpenALSound::noLoop(void)
	{
		this->alLoop = AL_FALSE;
		//tell the sound to not loop continuously
		alSourcei(alSource,AL_LOOPING,AL_FALSE);
	}

	void OpenALSound::setPitch(float p)
	{
		this->pitch = p;
		alSourcef(alSource, AL_PITCH, this->pitch);
	}

	void OpenALSound::setGain(float g)
	{
		this->gain = g;
		alSourcef(alSource, AL_GAIN, this->gain);
	}

	void OpenALSound::setPitchAndGain(float p, float g)
	{
		this->pitch = p;
		this->gain = g;
		alSourcef(alSource, AL_PITCH, this->pitch);
		alSourcef(alSource, AL_GAIN, this->gain);
	}

	unsigned int OpenALSound::getBuffer(void)
	{
		return this->Buffer;
	}

	bool OpenALSound::isLooped(void)
	{
		return this->alLoop;
	}

	bool OpenALSound::useBuffer(OpenALSound* sound)
	{
		Console* myLog = &myGlobalGame->mlog;

		this->loaded = false;

		strcpy(this->soundFileName, sound->soundFileName);

		myLog->LogInfo(LString("Attempting to useBuffer sound file: \"%s\"", sound->soundFileName));
		
		this->alLoop = sound->isLooped();

		myLog->LogInfo(LString("Sound file name bytes copied to local soundFileName for \"%s\" as useBuffer", sound->soundFileName));
		
		int error = -1;	

		alGetError();
		// Load wav data into a buffer.
		alGenBuffers(1, &Buffer);
		myLog->LogInfo(LString("Sound buffer created for \"%s\" as a useBuffer", sound->soundFileName));

		if ((error = alGetError()) != AL_NO_ERROR)
		{
			myLog->LogError(LString("As a useBuffer, alGenBuffers in &Buffer failure: \"%s\", error: %d", sound->soundFileName, error));
			this->Free();
			return false;
		}

		//alutLoadWAVFile(fileName, &alFormatBuffer, &alBuffer, &alBufferLen, &alFreqBuffer, &tloop);

		this->Buffer = (*sound->getBufferAddress());
		this->alFormatBuffer = (*sound->getalFormatBufferAddress());
		this->alBuffer = sound->getalBufferAddress();
		this->alBufferLen = sound->getalBufferLen();
		this->alFreqBuffer = sound->getalFreqBuffer();

		alBufferData(Buffer, alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);

		if(alBuffer == NULL)
		{
			myLog->LogError(LString("Severe: Audio data is A NULL POINTER (useBuffer)"));
		}
		
		
		if ((error = alGetError())!= AL_NO_ERROR)
		{
			if(error != 40963)
			{
				myLog->LogError(LString("Could not load wav file, alBufferData failure: \"%s\", error no: %d (useBuffer)", this->soundFileName, error));
				this->Free();
				return false;
			}
		}

		//alutUnloadWAV(alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
		
		if ((error = alGetError())!= AL_NO_ERROR)
		{
			if(error != 40963)
			{
				myLog->LogError(LString("Could not load wav file: \"%s\" (useBuffer) error: %d", soundFileName, error));	
				this->Free();
				return false;
			}
		}

		// Bind buffer with a source.

		alGenSources(1, &alSource);	

		if ((error = alGetError())!= AL_NO_ERROR)
		{
			myLog->LogError(LString("Could not load wav file, alGenSources failure: \"%s\", error no: %d (useBuffer)", soundFileName, error));
			this->Free();
			return false;
		}

		this->pitch = 1.0f;
		this->gain = 1.0f;

		alSourcei (alSource, AL_BUFFER,   Buffer   );
		alSourcei (alSource, AL_LOOPING,  alLoop );	
		alSourcef(alSource, AL_PITCH, this->pitch);
		alSourcef(alSource, AL_GAIN, this->gain);

		// Do an error check and return.

		if ((error = alGetError())!= AL_NO_ERROR)
		{
			if(error != 40963)
			{
				myLog->LogError(LString("Could not load wav file: \"%s\" (useBuffer), error: %d", soundFileName, error));	
				this->Free();
				return false;
			}
		}

		/*
		if(loop)
			this->loop();
		else
			this->noLoop();
		*/

		myLog->LogSuccessText(LString("Succesfull use buffer of wav: \"%s\" (useBuffer)", soundFileName));

		this->loaded = true;

		return true;

		/**BREAK**/
		//alSourcei(alSource, AL_BUFFER, sound->getBuffer());
	}

	void OpenALSound::setRollOff(float v)
	{
		alSourcef (alSource, AL_ROLLOFF_FACTOR,  v);
	}

	void OpenALSound::resetBuffer(void)
	{
		alSourcei(alSource, AL_BUFFER, this->getBuffer());
	}

	void OpenALSound::pause(void)
	{
		alSourcePause(this->alSource);
	}

	bool OpenALSound::isPlaying(void)
	{
		ALenum state;
	    
		alGetSourcei(this->alSource, AL_SOURCE_STATE, &state);
	    
		return (state == AL_PLAYING);
	}

	void OpenALSound::makeRelative(void)
	{
		alSourcei (this->alSource, AL_SOURCE_RELATIVE, AL_TRUE);
	}

	void OpenALSound::makeNotRelative(void)
	{
		alSourcei (this->alSource, AL_SOURCE_RELATIVE, AL_FALSE);
	}

	ALuint* OpenALSound::getBufferAddress(void)
	{
		return &Buffer;
	}

	ALenum* OpenALSound::getalFormatBufferAddress(void)
	{
		return &alFormatBuffer;
	}

	ALvoid* OpenALSound::getalBufferAddress(void)
	{
		return &alBuffer;
	}

	ALsizei OpenALSound::getalFreqBuffer(void)
	{
		return alFreqBuffer;
	}

	ALsizei OpenALSound::getalBufferLen(void)
	{
		return alBufferLen;
	}

	/*
		From here on, the OpenALStream class that handles *.ogg files begins
	*/

	void OpenALStream::open(char* path)
	{
		Console* myLog = &myGlobalGame->mlog;

		int result;

		this->pause = false;

		strcpy(this->soundFileName, path);
		
		myLog->LogInfo(LString("Streaming ogg file: %s", this->soundFileName));
	    
		if(!(oggFile = fopen(path, "rb")))
			myLog->LogError(LString("Could not open Ogg file: %s", this->soundFileName));

		if((result = ov_open(oggFile, &oggStream, NULL, 0)) < 0)
		{
			fclose(oggFile);
	        
			myLog->LogError(LString("Could not open Ogg stream. "));
			errorString(result);
		}

		vorbisInfo = ov_info(&oggStream, -1);
		vorbisComment = ov_comment(&oggStream, -1);

		if(vorbisInfo->channels == 1)
			format = AL_FORMAT_MONO16;
		else
			format = AL_FORMAT_STEREO16;
	        
	        
		alGenBuffers(2, buffers);
		check();
		alGenSources(1, &source);
		check();
	    
		alSource3f(source, AL_POSITION,        0.0, 0.0, 0.0);
		alSource3f(source, AL_VELOCITY,        0.0, 0.0, 0.0);
		alSource3f(source, AL_DIRECTION,       0.0, 0.0, 0.0);
		alSourcef (source, AL_ROLLOFF_FACTOR,  0.0          );
		alSourcei (source, AL_SOURCE_RELATIVE, AL_TRUE      );
	}

	void OpenALStream::Free()
	{
		alSourceStop(source);
		empty();
		alDeleteSources(1, &source);
		check();
		alDeleteBuffers(2, buffers);
		check();

		ov_clear(&oggStream);

		myGlobalGame->mlog.LogInfo(LString("Ogg file realeased: %s", this->soundFileName));
	}

	OpenALStream::~OpenALStream(void)
	{
		this->Free();
	}

	void OpenALStream::display()
	{
		Console* myLog = &myGlobalGame->mlog;

		myLog->LogInfo(LString("Details of ogg file: %s\n", this->soundFileName));
		myLog->LogInfo(LString("Version %d", vorbisInfo->version));
		myLog->LogInfo(LString("channels %d", vorbisInfo->channels));
		myLog->LogInfo(LString("rate (hz) ", vorbisInfo->rate));
		myLog->LogInfo(LString("bitrate upper %d", vorbisInfo->bitrate_upper));
		myLog->LogInfo(LString("bitrate nominal %d", vorbisInfo->bitrate_nominal));
		myLog->LogInfo(LString("bitrate lower %d", vorbisInfo->bitrate_lower));
		myLog->LogInfo(LString("bitrate window %d\n", vorbisInfo->bitrate_window));

		myLog->LogInfo(LString("vendor %s\n", vorbisComment->vendor));

		for(int i = 0; i < vorbisComment->comments; i++)
			myLog->LogInfo(LString("%s", vorbisComment->user_comments[i]));

		myLog->LogInfo(LString("End of ogg file comments: %s\n", this->soundFileName));
	}

	bool OpenALStream::playback()
	{
		if(playing())
			return true;
	        
		if(!stream(buffers[0]))
			return false;
	        
		if(!stream(buffers[1]))
			return false;
	    
		alSourceQueueBuffers(source, 2, buffers);
		alSourcePlay(source);
	    
		return true;
	}

	bool OpenALStream::playing(void)
	{
		ALenum state;
	    
		alGetSourcei(source, AL_SOURCE_STATE, &state);
	    
		return (state == AL_PLAYING);
	}

	bool OpenALStream::update(void)
	{
		int processed;
		bool active = true;

		alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);

		while(processed--)
		{
			ALuint buffer;
	        
			alSourceUnqueueBuffers(source, 1, &buffer);
			check();

			active = stream(buffer);

			alSourceQueueBuffers(source, 1, &buffer);
			check();
		}

		return active;
	}

	bool OpenALStream::stream(ALuint buffer)
	{
		char pcm[BUFFER_SIZE];
		int  size = 0;
		int  section;
		int  result;

		while(size < BUFFER_SIZE)
		{
			result = ov_read(&oggStream, pcm + size, BUFFER_SIZE - size, 0, 2, 1, &section);
	    
			if(result > 0)
				size += result;
			else
				if(result < 0)
					errorString(result);
				else
					break;
		}
	    
		if(size == 0)
			return false;
	        
		alBufferData(buffer, format, pcm, size, vorbisInfo->rate);
		check();
	    
		return true;
	}

	void OpenALStream::empty()
	{
		int queued;
	    
		alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
	    
		while(queued--)
		{
			ALuint buffer;
	    
			alSourceUnqueueBuffers(source, 1, &buffer);
			check();
		}
	}

	void OpenALStream::check()
	{
		int error = alGetError();

		if(error == AL_INVALID_VALUE)
			myGlobalGame->mlog.LogError(LString("OpenAL error: AL_INVALID_VALUE"));

		else if(error != AL_NO_ERROR)
			myGlobalGame->mlog.LogError(LString("OpenAL error was raised. Error is: %d", error));
	}

	void OpenALStream::errorString(int code)
	{
		switch(code)
		{
			case OV_EREAD:
				myGlobalGame->mlog.LogError(LString("Read from media."));
				break;
			case OV_ENOTVORBIS:
				myGlobalGame->mlog.LogError(LString("Not Vorbis data."));
				break;
			case OV_EVERSION:
			   myGlobalGame->mlog.LogError(LString("Vorbis version mismatch."));
				break;
			case OV_EBADHEADER:
				myGlobalGame->mlog.LogError(LString("Invalid Vorbis header."));
				break;
			case OV_EFAULT:
				myGlobalGame->mlog.LogError(LString("Internal logic fault (bug or heap/stack corruption."));
				break;
			default:
				myGlobalGame->mlog.LogError(LString("Unknown Ogg error."));
		}
	}

	void OpenALStream::pauseit(void)
	{
		this->pause = true;
	}

	void OpenALStream::unPause(void)
	{
		this->pause = false;
	}

	void OpenALStream::switchPause(void)
	{
		this->pause = !this->pause;
	}

	bool OpenALStream::getPause(void)
	{
		return this->pause;
	}

	void OpenALStream::play(void)
	{

		if(!this->pause)
		{
			if(this->playing())
				this->update();
	    
			if(!this->playing())
			{
				if(!this->playback())
					myGlobalGame->mlog.LogError(LString("Ogg abruptly stopped for %s", this->soundFileName));
				else
					myGlobalGame->mlog.LogError(LString("Ogg stream was interrupted for %s", this->soundFileName));
			}
		} else {
			alSourcePause(this->source);
		}
	}

	OpenALStream::OpenALStream(void)
	{
		this->pause = true;
	}

	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5

		For FMOD Only

	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/

	FMODSource::FMODSource(void)
	{
		mPos.x = 0;
		mPos.y = 0;
		mPos.z = 0;

		mVel.x = 0;
		mVel.y = 0;
		mVel.z = 0;

		this->mSoundObj = 0;
	}

	FMODSource::FMODSource(FMOD::Sound* soundObj)
	{
		mPos.x = 0;
		mPos.y = 0;
		mPos.z = 0;

		mVel.x = 0;
		mVel.y = 0;
		mVel.z = 0;

		FMOD_RESULT result;

		this->mSoundObj = soundObj;

		result = myGlobalGame->mSoundSystem->mFMODSystem->playSound(FMOD_CHANNEL_FREE, soundObj, true, &mChannel);
		myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);
	}

	FMODSource::~FMODSource(void)
	{
		//this->Free();
	}

	void FMODSource::set(const Math::Vector3 &pos, const Math::Vector3 &vel)
	{
		this->mPos.x = pos.x;
		this->mPos.y = pos.y;
		this->mPos.z = pos.z;

		this->mVel.x = vel.x;
		this->mVel.y = vel.y;
		this->mVel.z = vel.z;

		/*FMOD_RESULT result = */mChannel->set3DAttributes(&mPos, &mVel);
		//FMOD_ERRCHECK(result);
	}

	void FMODSource::setWithoutUpdatingChannel(const Math::Vector3 &pos, const Math::Vector3 &vel)
	{
		this->mPos.x = pos.x;
		this->mPos.y = pos.y;
		this->mPos.z = pos.z;

		this->mVel.x = vel.x;
		this->mVel.y = vel.y;
		this->mVel.z = vel.z;
	}

	void FMODSource::play(void)
	{
		FMOD_RESULT result = myGlobalGame->mSoundSystem->mFMODSystem->playSound(FMOD_CHANNEL_FREE, this->mSoundObj, false, &mChannel);	
		myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);

		//Apply channel group to self sound channel
		result = this->mChannel->setChannelGroup(this->channelGroup);
		myGlobalGame->mSoundSystem->FMOD_ERRCHECK(result);

		//this->mChannel->setPaused(false);
	}

	void FMODSource::pause(void)
	{
		this->mChannel->setPaused(true);
	}

	bool FMODSource::isPaused(void)
	{
		bool paused;
		/*FMOD_RESULT result = */this->mChannel->getPaused(&paused);
		//FMOD_ERRCHECK(result);

		return paused;
	}


	void FMODSource::resume(void)
	{
		this->mChannel->setPaused(false);
	}

	void FMODSource::stop(void)
	{
		this->mChannel->stop();
	}

	void FMODSource::setSound(FMOD::Sound* soundObj, FMOD::ChannelGroup* channelGroup)
	{
		this->mChannel->stop();
		this->mSoundObj = soundObj;

		if(!channelGroup)
		{
			return;
		}

		if(channelGroup)
		{
			this->channelGroup = channelGroup;		
		}
	}

	void FMODSource::Free(void)
	{
		//this->mChannel->
	}

	FMOD::Sound* FMODSource::getSoundObject(void)
	{
		return this->mSoundObj;
	}

	bool FMODSource::isPlaying(void)
	{
		bool play;
		/*FMOD_RESULT result = */this->mChannel->isPlaying(&play);
		//FMOD_ERRCHECK(result);

		return play;
	}

	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5

		Various Audio source objects for gameplay (FOR FMOD sources only :D )

	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/

	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5

		Audio Point emitter source

	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/

	SoundEmitterPoint::SoundEmitterPoint(void)
	{
	}

	SoundEmitterPoint::~SoundEmitterPoint(void)
	{
	}

	bool SoundEmitterPoint::Init(FMOD::Sound* pSoundObj,
									RX_FMOD_AUDIO_CHANNELS rxChannel, float _3dminStart, float _3dmaxEnd)
	{
		FMOD::ChannelGroup* tChannelGroup;

		this->mFMODSource.set(Math::Vector3(0, 0, 0), Math::Vector3(0, 0, 0));

		switch(rxChannel)
		{
		case RX_SOUNDS_GROUP_1:
			tChannelGroup = myGlobalGame->mSoundSystem->groupSounds1;
			break;
		case RX_SOUNDS_GROUP_2:
			tChannelGroup = myGlobalGame->mSoundSystem->groupSounds2;
			break;
		case RX_MUSIC_GROUP_1:
			tChannelGroup = myGlobalGame->mSoundSystem->groupMusic1;
			break;
		case RX_MUSIC_GROUP_2:
			tChannelGroup = myGlobalGame->mSoundSystem->groupMusic2;
			break;
		case RX_EXTRA_GROUP_1:
			tChannelGroup = myGlobalGame->mSoundSystem->groupExtra1;
			break;
		case RX_EXTRA_GROUP_2:
			tChannelGroup = myGlobalGame->mSoundSystem->groupExtra2;
			break;
		default:
			tChannelGroup = myGlobalGame->mSoundSystem->groupSounds1;
			break;
		}

		this->mFMODSource.setSound(pSoundObj, tChannelGroup);

		if(_3dmaxEnd > 0 && _3dmaxEnd > 0)
		{
			pSoundObj->set3DMinMaxDistance(_3dminStart, _3dmaxEnd);
		}

		return true;
	}

	void SoundEmitterPoint::UpdateEmitter(void)
	{
		//Perform audio shaping operations
		pos = this->mFMODSource.mPos;
		vel = this->mFMODSource.mVel;

		this->mFMODSource.mChannel->set3DAttributes(&pos, &vel);
	}

	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5

		Audio Point emitter source

	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/
	SoundEmitterSphere::SoundEmitterSphere(void)
	{
	}

	SoundEmitterSphere::~SoundEmitterSphere(void)
	{
	}

	void SoundEmitterSphere::SetRadius(float r)
	{
		this->mfRadius = r;
	}

	bool SoundEmitterSphere::Init(FMOD::Sound* pSoundObj,
									RX_FMOD_AUDIO_CHANNELS rxChannel, float r, float _3dminStart, float _3dmaxEnd)
	{
		FMOD::ChannelGroup* tChannelGroup;

		this->mFMODSource.set(Math::Vector3(0, 0, 0), Math::Vector3(0, 0, 0));

		this->mfRadius = r;

		switch(rxChannel)
		{
		case RX_SOUNDS_GROUP_1:
			tChannelGroup = myGlobalGame->mSoundSystem->groupSounds1;
			break;
		case RX_SOUNDS_GROUP_2:
			tChannelGroup = myGlobalGame->mSoundSystem->groupSounds2;
			break;
		case RX_MUSIC_GROUP_1:
			tChannelGroup = myGlobalGame->mSoundSystem->groupMusic1;
			break;
		case RX_MUSIC_GROUP_2:
			tChannelGroup = myGlobalGame->mSoundSystem->groupMusic2;
			break;
		case RX_EXTRA_GROUP_1:
			tChannelGroup = myGlobalGame->mSoundSystem->groupExtra1;
			break;
		case RX_EXTRA_GROUP_2:
			tChannelGroup = myGlobalGame->mSoundSystem->groupExtra2;
			break;
		default:
			tChannelGroup = myGlobalGame->mSoundSystem->groupSounds1;
			break;
		}

		this->mFMODSource.setSound(pSoundObj, tChannelGroup);

		if(_3dmaxEnd > 0 && _3dmaxEnd > 0)
		{
			pSoundObj->set3DMinMaxDistance(_3dminStart, _3dmaxEnd);
		}

		return true;
	}

	void SoundEmitterSphere::UpdateEmitter(void)
	{
		Math::Vector3 userPosition = myGlobalGame->mSoundSystem->GetListenerPosition();
		Math::Vector3 soundPosition = Math::Vector3(this->mFMODSource.mPos.x, this->mFMODSource.mPos.y, this->mFMODSource.mPos.z);

		Math::Vector3 dir = userPosition - soundPosition;

		float dist = dir.InverseMagnitude();

		dir *= dist;

		dist = 1/dist;

		float final_distance = Math::min(dist, this->mfRadius);

		FMOD_VECTOR new_pos;

		new_pos.x = this->mFMODSource.mPos.x + dir.x * final_distance;
		new_pos.y = this->mFMODSource.mPos.y + dir.y * final_distance;
		new_pos.z = this->mFMODSource.mPos.z + dir.z * final_distance;

		//Perform audio shaping operations
		pos = new_pos;
		vel = this->mFMODSource.mVel;

		this->mFMODSource.mChannel->set3DAttributes(&pos, &vel);
	}

	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5

		Audio Line emitter source

	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/

	SoundEmitterLine::SoundEmitterLine(void)
	{
	}

	SoundEmitterLine::~SoundEmitterLine(void)
	{
	}

	bool SoundEmitterLine::Init(FMOD::Sound* pSoundObj,
									RX_FMOD_AUDIO_CHANNELS rxChannel,
									const Math::Vector3 &startPoint, const Math::Vector3 &endPoint,
									float _3dminStart, float _3dmaxEnd)
	{
		FMOD::ChannelGroup* tChannelGroup;

		this->mFMODSource.set(Math::Vector3(0, 0, 0), Math::Vector3(0, 0, 0));

		this->m_vStartPoint = startPoint;
		this->m_vEndPoint = endPoint;

		switch(rxChannel)
		{
		case RX_SOUNDS_GROUP_1:
			tChannelGroup = myGlobalGame->mSoundSystem->groupSounds1;
			break;
		case RX_SOUNDS_GROUP_2:
			tChannelGroup = myGlobalGame->mSoundSystem->groupSounds2;
			break;
		case RX_MUSIC_GROUP_1:
			tChannelGroup = myGlobalGame->mSoundSystem->groupMusic1;
			break;
		case RX_MUSIC_GROUP_2:
			tChannelGroup = myGlobalGame->mSoundSystem->groupMusic2;
			break;
		case RX_EXTRA_GROUP_1:
			tChannelGroup = myGlobalGame->mSoundSystem->groupExtra1;
			break;
		case RX_EXTRA_GROUP_2:
			tChannelGroup = myGlobalGame->mSoundSystem->groupExtra2;
			break;
		default:
			tChannelGroup = myGlobalGame->mSoundSystem->groupSounds1;
			break;
		}

		this->mFMODSource.setSound(pSoundObj, tChannelGroup);

		if(_3dmaxEnd > 0 && _3dmaxEnd > 0)
		{
			pSoundObj->set3DMinMaxDistance(_3dminStart, _3dmaxEnd);
		}

		return true;
	}

	void SoundEmitterLine::UpdateEmitter(void)
	{
		Math::Vector3 closestPoint = ClosestPointOnLine(this->m_vStartPoint, 
											   this->m_vEndPoint,
											   myGlobalGame->mSoundSystem->GetListenerPosition());

		//Perform audio shaping operations
		pos.x = closestPoint.x;
		pos.y = closestPoint.y;
		pos.z = closestPoint.z;

		vel = this->mFMODSource.mVel;

		this->mFMODSource.mChannel->set3DAttributes(&pos, &vel);
	}

	void SoundEmitterLine::SetEndPoints(const Math::Vector3 &startPoint, const Math::Vector3 &endPoint)
	{
		this->m_vStartPoint = startPoint;
		this->m_vEndPoint = endPoint;
	}

	/*
	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5

		Audio Box emitter source

	5%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
	*/

	SoundEmitterBox::SoundEmitterBox(void)
	{
	}

	SoundEmitterBox::~SoundEmitterBox(void)
	{
	}

	bool SoundEmitterBox::Init(FMOD::Sound* pSoundObj,
									RX_FMOD_AUDIO_CHANNELS rxChannel,
									const Math::Vector3 &minSize, const Math::Vector3 &maxSize,
									float _3dminStart, float _3dmaxEnd)
	{
		FMOD::ChannelGroup* tChannelGroup;

		this->mFMODSource.set(Math::Vector3(0, 0, 0), Math::Vector3(0, 0, 0));

		this->m_vMinSize = minSize;
		this->m_vMaxSize = maxSize;

		switch(rxChannel)
		{
		case RX_SOUNDS_GROUP_1:
			tChannelGroup = myGlobalGame->mSoundSystem->groupSounds1;
			break;
		case RX_SOUNDS_GROUP_2:
			tChannelGroup = myGlobalGame->mSoundSystem->groupSounds2;
			break;
		case RX_MUSIC_GROUP_1:
			tChannelGroup = myGlobalGame->mSoundSystem->groupMusic1;
			break;
		case RX_MUSIC_GROUP_2:
			tChannelGroup = myGlobalGame->mSoundSystem->groupMusic2;
			break;
		case RX_EXTRA_GROUP_1:
			tChannelGroup = myGlobalGame->mSoundSystem->groupExtra1;
			break;
		case RX_EXTRA_GROUP_2:
			tChannelGroup = myGlobalGame->mSoundSystem->groupExtra2;
			break;
		default:
			tChannelGroup = myGlobalGame->mSoundSystem->groupSounds1;
			break;
		}	

		this->mFMODSource.setSound(pSoundObj, tChannelGroup);

		if(_3dmaxEnd > 0 && _3dmaxEnd > 0)
		{
			pSoundObj->set3DMinMaxDistance(_3dminStart, _3dmaxEnd);
		}

		return true;
	}

	void SoundEmitterBox::UpdateEmitter(void)
	{
		Math::Vector3 final_point;

		Math::Vector3 user_pos = myGlobalGame->mSoundSystem->GetListenerPosition();

		final_point.x = Math::clamp(user_pos.x, this->m_vMinSize.x, m_vMaxSize.x);
		final_point.y = Math::clamp(user_pos.y, this->m_vMinSize.y, m_vMaxSize.y);
		final_point.z = Math::clamp(user_pos.z, this->m_vMinSize.z, m_vMaxSize.z);

		//Perform audio shaping operations
		pos.x = final_point.x;
		pos.y = final_point.y;
		pos.z = final_point.z;

		vel = this->mFMODSource.mVel;

		this->mFMODSource.mChannel->set3DAttributes(&pos, &vel);
	}

	void SoundEmitterBox::SetDimensions(const Math::Vector3 &minSize, const Math::Vector3 &maxSize)
	{
		this->m_vMinSize = minSize;
		this->m_vMaxSize = maxSize;
	}
}