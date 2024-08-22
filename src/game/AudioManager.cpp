#include "AudioManager.h"

#define OpenAL_ErrorCheck(message)\
{\
    ALenum error = alGetError();\
    if (error != AL_NO_ERROR)\
	{\
		std::cout << "OpenAL Error: " << error << std::endl;\
		std::cout << message << std::endl;\
	}\
}

#define alec(FUNCTION_CALL)\
FUNCTION_CALL;\
OpenAL_ErrorCheck(#FUNCTION_CALL)

void AudioManager::playSound(std::string filename, glm::vec3 location)
{
	const ALCchar* defaultDeviceString = alcGetString(/*device*/nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
	ALCdevice* device = alcOpenDevice(defaultDeviceString);
	if (!device)
	{
		std::cerr << "failed to get the default device for OpenAL" << std::endl;
	}
	std::cout << "OpenAL Device: " << alcGetString(device, ALC_DEVICE_SPECIFIER) << std::endl;
	//OpenAL_ErrorCheck(device);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Create an OpenAL audio context from the device
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ALCcontext* context = alcCreateContext(device, /*attrlist*/ nullptr);
	//OpenAL_ErrorCheck(context);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Activate this context so that OpenAL state modifications are applied to the context
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!alcMakeContextCurrent(context))
	{
		std::cerr << "failed to make the OpenAL context the current context" << std::endl;
	}
	//OpenAL_ErrorCheck("Make context current");

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Create a listener in 3d space (ie the player); (there always exists as listener, you just configure data on it)
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	alec(alListener3f(AL_POSITION, 0.f, 0.f, 0.f));
	alec(alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f));
	ALfloat forwardAndUpVectors[] = {
		/*forward = */ 1.f, 0.f, 0.f,
		/* up = */ 0.f, 1.f, 0.f
	};
	alec(alListenerfv(AL_ORIENTATION, forwardAndUpVectors));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Create buffers that hold our sound data; these are shared between contexts and are defined at a device level
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct ReadWavData
	{
		unsigned int channels = 0;
		unsigned int sampleRate = 0;
		drmp3_uint64 totalPCMFrameCount = 0;
		std::vector<uint16_t> pcmData;
		drmp3_uint64 getTotalSamples() { return totalPCMFrameCount * channels; }
	};
	ReadWavData monoData;
	{
		drmp3_config config;
		drmp3_uint64 totalPCMFrameCount;
		std::string path = "res/audio/" + filename;
		drmp3_int16* pSampleData = drmp3_open_file_and_read_pcm_frames_s16(path.c_str(), &config, &totalPCMFrameCount, nullptr);
		if (pSampleData == NULL) {
			std::cerr << "failed to load audio file" << std::endl;
			drmp3_free(pSampleData, nullptr); // Make sure to clean up memory
		}

		monoData.channels = config.channels;
		monoData.sampleRate = config.sampleRate;
		monoData.totalPCMFrameCount = totalPCMFrameCount;
		monoData.pcmData.resize(static_cast<size_t>(totalPCMFrameCount * config.channels));
		std::memcpy(monoData.pcmData.data(), pSampleData, monoData.pcmData.size() * sizeof(drmp3_int16));

		drmp3_free(pSampleData, nullptr);  // Free the allocated memory after use

		// Create buffer and fill with audio data
		ALuint monoBuffer;
		alec(alGenBuffers(1, &monoBuffer));
		alec(alBufferData(monoBuffer, AL_FORMAT_MONO16, monoData.pcmData.data(), static_cast<ALsizei>(monoData.pcmData.size() * sizeof(uint16_t)), monoData.sampleRate));

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Create a source that emits the sound
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ALuint source;
		alec(alGenSources(1, &source));
		alec(alSourcef(source, AL_PITCH, 1.f));
		alec(alSourcef(source, AL_GAIN, 1.f));
		alec(alSource3f(source, AL_POSITION, location.x, location.y, location.z));
		alec(alSource3f(source, AL_VELOCITY, 0.f, 0.f, 0.f));
		alec(alSourcei(source, AL_LOOPING, AL_FALSE));
		alec(alSourcei(source, AL_BUFFER, monoBuffer));

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Play sound
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		alec(alSourcePlay(source));
	}

	ReadWavData stereoData;
	{
		drmp3_config config;
		drmp3_uint64 totalPCMFrameCount;
		std::string path = "res/audio/" + filename;
		drmp3_int16* pSampleData = drmp3_open_file_and_read_pcm_frames_s16(path.c_str(), &config, &totalPCMFrameCount, nullptr);
		if (pSampleData == NULL) {
			std::cerr << "failed to load audio file" << std::endl;
			drmp3_free(pSampleData, nullptr); // Make sure to clean up memory
		}

		stereoData.channels = config.channels;
		stereoData.sampleRate = config.sampleRate;
		stereoData.totalPCMFrameCount = totalPCMFrameCount;
		stereoData.pcmData.resize(static_cast<size_t>(totalPCMFrameCount * config.channels));
		std::memcpy(stereoData.pcmData.data(), pSampleData, stereoData.pcmData.size() * sizeof(drmp3_int16));

		drmp3_free(pSampleData, nullptr);  // Free the allocated memory after use

		// Create buffer and fill with audio data
		ALuint stereoBuffer;
		alec(alGenBuffers(1, &stereoBuffer));
		alec(alBufferData(stereoBuffer, AL_FORMAT_STEREO16, stereoData.pcmData.data(), static_cast<ALsizei>(stereoData.pcmData.size() * sizeof(uint16_t)), stereoData.sampleRate));

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Create a source that emits the sound
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ALuint source;
		alec(alGenSources(1, &source));
		alec(alSourcef(source, AL_PITCH, 1.f));
		alec(alSourcef(source, AL_GAIN, 1.f));
		alec(alSource3f(source, AL_POSITION, location.x, location.y, location.z));
		alec(alSource3f(source, AL_VELOCITY, 0.f, 0.f, 0.f));
		alec(alSourcei(source, AL_LOOPING, AL_FALSE));
		alec(alSourcei(source, AL_BUFFER, stereoBuffer));

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Play sound
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		alec(alSourcePlay(source));
	}
}