#include "general.hpp"
#include "AL/al.h"
#include "AL/alc.h"
#define DR_FLAC_IMPLEMENTATION
#include "dr_flac.h"
#define DR_MP3_IMPLEMENTATION
#include "dr_mp3.h"
#define STB_DEFINE
#define STB_ONLY
#include "stb_vorbis.c"
#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"
#include <vector>
#include <cstddef>


size_t BUFFER_SIZE = 2048;
	size_t BUFFER_NUM = 3;

ALenum getFormat(ALuint  channel)
{
	ALenum format = 0;
	if(channel == 1)
		format = AL_FORMAT_MONO16;
	else if (channel == 2)
		format = AL_FORMAT_STEREO16;
	else if(channel > 2)
	{
		// currently not implemented.
		return 0;
	}
	return format;
}

// setBufferSize sets the size of memory to be allocated for the buffer.
void setBufferSize(size_t size)
{
	if(size < 2048 || size > 65536)
	{
		BUFFER_SIZE = 2048;
		return;
	}
			BUFFER_SIZE = size;
}

void setBufferCount(size_t number)
{
	if(number < 3)
	{
		BUFFER_NUM = 3;
		return;
	}
	BUFFER_NUM = number;
}

size_t getBufferCount()
{
	return BUFFER_NUM;
}

size_t getBufferSize()
{
	return BUFFER_SIZE;
}

namespace stream
{
enum DecoderType {
	FLAC = 1,
	MP3,
	OGG,
	WAV,
};

typedef struct StreamInfo
{
	ALuint channels;
	ALuint sampleRate;
	//stream->info.bitsPerSample;
} StreamInfo;

typedef struct StreamDecoders
{
	DecoderType decoder;
		drflac* flacStream;
		drmp3 mp3Stream;
	stb_vorbis *oggStream;
	stb_vorbis_info oggInfo;
	drwav wavStream;
} StreamDecoders;

typedef struct  AudioStream
{
	ALuint source;
	ALuint *buffers = new ALuint[getBufferCount()];
	ALenum format;
	StreamInfo info;
	StreamDecoders audioDecoder;
	std::vector<short> memBuf;
	bool loop = false;
	bool inited = false;

	bool init();
	bool uninit();
	bool openFromFile(const char *filePath);
	bool openFromMemory(const void *data, size_t size);
	size_t fillBuffers();
	void setLoop(bool  loop);
	bool getLoop();
	bool stop();
	bool seek(size_t millisecond);
	bool play();
	bool pause();
	bool update();

} AudioStream;

bool AudioStream::init()
{
	std::size_t successBufferNumber = 0;
	alSourceRewind(source);
	for(; successBufferNumber < getBufferCount(); ++successBufferNumber) {
		size_t readCount = fillBuffers() * info.channels;
		if(readCount < 1)
			break;
		alBufferData(buffers[successBufferNumber], format, memBuf.data(), readCount * sizeof(short), info.sampleRate);
	}
	alSourceQueueBuffers(source, successBufferNumber, &buffers[0]);
	return true;
}

bool AudioStream::uninit()
{
	if(audioDecoder.decoder == 0)
		return false;
	switch(audioDecoder.decoder)
	{
	case DecoderType::FLAC:
		drflac_close(audioDecoder.flacStream);
		return true;
	case DecoderType::MP3:
		drmp3_uninit(&audioDecoder.mp3Stream);
		return true;
	case DecoderType::OGG:
		stb_vorbis_close(audioDecoder.oggStream);
		audioDecoder.oggStream = NULL;
		return true;
	case DecoderType::WAV:
		drwav_uninit(&audioDecoder.wavStream);
		return true;
	default: return false;
	}
}

bool AudioStream::openFromFile(const char *filePath)
{
	if (filePath == NULL) {
		return false;
	}
	switch (audioDecoder.decoder)
	{
	case DecoderType::FLAC:
		audioDecoder.flacStream = drflac_open_file(filePath, NULL);
		if(audioDecoder.flacStream == NULL)
			return false;
		info.channels = audioDecoder.flacStream->channels;
		info.sampleRate = audioDecoder.flacStream->sampleRate;
		format = getFormat(info.channels);
		break;
	case DecoderType::MP3:
		if(!drmp3_init_file(&audioDecoder.mp3Stream, filePath, NULL))
			return false;
		info.channels = audioDecoder.mp3Stream.channels;
		info.sampleRate = audioDecoder.mp3Stream.sampleRate;
		format = getFormat(info.channels);
		break;
		case DecoderType::OGG:
		audioDecoder.oggStream = stb_vorbis_open_filename(filePath, NULL, NULL);
		if(!audioDecoder.oggStream)
			return false;
		audioDecoder.oggInfo = stb_vorbis_get_info(audioDecoder.oggStream);
		info.channels = audioDecoder.oggStream->channels;
		info.sampleRate = audioDecoder.oggStream->sample_rate;
		format = getFormat(info.channels);
		break;
	case DecoderType::WAV:
		if (!drwav_init_file(&audioDecoder.wavStream, filePath, NULL))
			return false;
		info.channels = audioDecoder.wavStream.channels;
		info.sampleRate = audioDecoder.wavStream.sampleRate;
		format = getFormat(info.channels);
		break;
		default: return false;
	}
	if(format == 0  || info.channels == 0 || info.sampleRate == 0) {
		uninit();
		return false;
	}
	memBuf.resize(getBufferSize() * info.channels);
	return true;
}

bool AudioStream::openFromMemory(const void *data, size_t size)
{
	if(size < 1 || data == NULL)
		return false;
	switch (audioDecoder.decoder) {
	case DecoderType::FLAC:
		audioDecoder.flacStream = drflac_open_memory(data, size, NULL);
		if(audioDecoder.flacStream == NULL)
			return false;
		info.channels = audioDecoder.flacStream->channels;
		info.sampleRate = audioDecoder.flacStream->sampleRate;
		format = getFormat(info.channels);
		break;
	case DecoderType::MP3:
		if(!drmp3_init_memory(&audioDecoder.mp3Stream, data, size, NULL))
			return false;
		info.channels = audioDecoder.mp3Stream.channels;
		info.sampleRate = audioDecoder.mp3Stream.sampleRate;
		format = getFormat(info.channels);
		break;
		case DecoderType::OGG:
		audioDecoder.oggStream = stb_vorbis_open_memory(static_cast<const unsigned char *>(data), size, NULL, NULL);
		if(!audioDecoder.oggStream)
			return false;
		audioDecoder.oggInfo = stb_vorbis_get_info(audioDecoder.oggStream);
		info.channels = audioDecoder.oggStream->channels;
		info.sampleRate = audioDecoder.oggStream->sample_rate;
		format = getFormat(info.channels);
		break;
	case DecoderType::WAV:
		if (!drwav_init_memory(&audioDecoder.wavStream, data, size, NULL))
			return false;
		info.channels = audioDecoder.wavStream.channels;
		info.sampleRate = audioDecoder.wavStream.sampleRate;
		format = getFormat(info.channels);
		break;
		default: return false;
	}
	if(format == 0  || info.channels == 0 || info.sampleRate == 0) {
		uninit();
		return false;
	}
	memBuf.resize(getBufferSize() * info.channels);
	return true;
}

size_t AudioStream::fillBuffers()
{
	size_t readCount = 0;
	switch(audioDecoder.decoder) {
	case DecoderType::FLAC:
		readCount  = drflac_read_pcm_frames_s16(audioDecoder.flacStream, getBufferSize(), memBuf.data());
		return readCount;
	case DecoderType::MP3:
		readCount = drmp3_read_pcm_frames_s16(&audioDecoder.mp3Stream, getBufferSize(), memBuf.data());
		return readCount;
	case DecoderType::OGG:
		readCount = stb_vorbis_get_samples_short_interleaved(audioDecoder.oggStream, info.channels, memBuf.data(), getBufferSize());
		return readCount;
	case DecoderType::WAV:
		readCount = drwav_read_pcm_frames_s16(&audioDecoder.wavStream, getBufferSize(), memBuf.data());
		return readCount;
	default: return 0;
	}
}

bool AudioStream::getLoop()
{
	return loop;
}


void AudioStream::setLoop(bool loop)
{
	this->loop = loop;
}

bool AudioStream::seek(size_t millisecond)
{
	size_t timePos = 		(millisecond * info.sampleRate) / 1000;
	switch(audioDecoder.decoder) {
	case DecoderType::FLAC:
		if(!drflac_seek_to_pcm_frame(audioDecoder.flacStream, timePos))
			return false;
		return true;
	case DecoderType::MP3:
		if(!drmp3_seek_to_pcm_frame(&audioDecoder.mp3Stream, timePos))
			return false;
		return true;
	case DecoderType::OGG:
		if(!stb_vorbis_seek_frame(audioDecoder.oggStream, timePos))
			return false;
		return true;
	case DecoderType::WAV:
		if(!drwav_seek_to_pcm_frame(&audioDecoder.wavStream, timePos))
			return false;
		return true;
	default: return false;
	}
}

bool AudioStream::play()
{
	if(!inited) {
		init();
		inited = true;
	}
	if(alIsSource(source)) {
		alSourcePlay(source);
		return true;
	}
	return false;
}

bool AudioStream::pause()
{
	if(alIsSource(source)) {
		alSourcePause(source);
		return true;
	}
	return false;
}

bool AudioStream::stop()
{
	if(alIsSource(source)) {
		alSourceStop(source);
		return seek(0);
	}
	return false;
}

bool AudioStream::update()
{
	ALint processed = 0;
	alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
	bool end = false;
	size_t size = 0;
	while(processed--) {
		ALuint bufferID = 0;
		alSourceUnqueueBuffers(source, 1, &bufferID);
		while(size < getBufferSize()) {
			auto readCount = fillBuffers() * info.channels;
			if(readCount < getBufferSize()) {
				if(loop) {
					seek(0);
					continue;
				} else {
					end = true;
				inited = false;
				break;
				}
			}
			size += readCount;
		}
		if(end) {
			break;
		}
		alBufferData(bufferID, format, memBuf.data(), size * sizeof(short), info.sampleRate);
		alSourceQueueBuffers(source, 1, &bufferID);
		size = 0;
	}
	if(end) {
		return false;
	}
	ALint  state = 0;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	if(state != AL_PLAYING && state != AL_PAUSED) { // && state != AL_STOPPED
		ALint queued = 0;
		alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
		if(queued == 0)
			return false;
	}
	return true;
}


}




stream::AudioStream* newAudioStream(stream::DecoderType dec)
{
	stream::AudioStream *aStream = new(stream::AudioStream);
	aStream->audioDecoder.decoder = dec;
	alGenBuffers(getBufferCount(), &aStream->buffers[0]);
	alGenSources(1, &aStream->source);
	return aStream;
}


bool destroyStream(stream::AudioStream* aStream)
{
	if(alIsSource(aStream->source)) {
		alSourceStop(aStream->source);
		alDeleteSources(1, const_cast<const ALuint*>(&aStream->source));
	}
	for(auto bufferID = 0; bufferID < int(getBufferCount()); ++bufferID) {
		if(alIsBuffer(aStream->buffers[bufferID]))
			alDeleteBuffers(1, &aStream->buffers[bufferID]);
	}
	if(!aStream->memBuf.empty()) {
		aStream->memBuf.clear();
		aStream->memBuf.resize(0);
	}
		delete []aStream->buffers;
	aStream->uninit();
	delete aStream;
	return true;
}


// flac files

expo
ALuint loadFlacFromFile(const char* fileName)
{
		ALuint format = 0;
	ALuint buffer = 0;
		alGenBuffers(1, &buffer);
	if(!alIsBuffer(buffer))
		return 0;
	unsigned int channels = 0;
	unsigned int sampleRate= 0;
	drflac_uint64 totalPCMFrameCount = 0;
	drflac_int16* pcmData = drflac_open_file_and_read_pcm_frames_s16(fileName, &channels, &sampleRate, &totalPCMFrameCount, NULL);
	if(pcmData == NULL || channels == 0 || sampleRate == 0) {
		alDeleteBuffers(1, &buffer);
		return 0;
	}
	format = getFormat(channels);
	if(format == 0) {
		alDeleteBuffers(1, &buffer);
		drflac_free(pcmData, NULL);
		return 0;
	}
	alBufferData(buffer, format, pcmData, totalPCMFrameCount * channels * sizeof(short), sampleRate);
	drflac_free(pcmData, NULL);
	return buffer;
}

expo
ALuint loadFlacFromMemory(size_t dataSize, const void* data)
{
		ALuint format = 0;
	ALuint buffer = 0;
		alGenBuffers(1, &buffer);
	if(!alIsBuffer(buffer))
		return 0;
	unsigned int channels = 0;
	unsigned int sampleRate = 0;
	drflac_uint64 totalPCMFrameCount;
	drflac_int16* pcmData = drflac_open_memory_and_read_pcm_frames_s16(data, dataSize, &channels, &sampleRate, &totalPCMFrameCount, NULL);
	if(pcmData == NULL || channels == 0 || sampleRate == 0) {
		alDeleteBuffers(1, &buffer);
		return 0;
	}
format = getFormat(channels);
if(format == 0) {
		alDeleteBuffers(1, &buffer);
		drflac_free(pcmData, NULL);
		return 0;
	}
	alBufferData(buffer, format, pcmData, totalPCMFrameCount * channels * sizeof(short), sampleRate);
	drflac_free(pcmData, NULL);
	return buffer;
}


// mp3 files

expo
ALuint loadMp3FromFile(const char* filePath)
{
	ALuint format = 0;
	ALuint buffer = 0;
	alGenBuffers(1, &buffer);
	if(!alIsBuffer(buffer))
		return 0;
	drmp3_config mp3Info;
	drmp3_uint64 totalPcm;
drmp3_int16* pcmData = drmp3_open_file_and_read_pcm_frames_s16(filePath,& mp3Info, &totalPcm, NULL);
	if(pcmData == NULL || mp3Info.channels == 0 || mp3Info.sampleRate == 0) {
		alDeleteBuffers(1, &buffer);
		return 0;
	}
	format = getFormat(mp3Info.channels);
	if(format == 0) {
		alDeleteBuffers(1, &buffer);
		free(pcmData);
		return 0;
	}
	alBufferData(buffer, format, pcmData, totalPcm * mp3Info.channels * sizeof(short), mp3Info.sampleRate);
	free(pcmData);
	return buffer;
}

expo
ALuint loadMp3FromMemory(size_t dataSize, const void* data)
{
	ALuint format = 0;
	ALuint buffer = 0;
	alGenBuffers(1, &buffer);
	if(!alIsBuffer(buffer))
		return 0;
	drmp3_config mp3Info;
	drmp3_uint64 totalPcm;
drmp3_int16* pcmData = drmp3_open_memory_and_read_pcm_frames_s16(data, dataSize, &mp3Info, &totalPcm, NULL);
	if(pcmData == NULL || mp3Info.sampleRate == 0 || mp3Info.channels == 0) {
		alDeleteBuffers(1, &buffer);
		return 0;
	}
	format = getFormat(mp3Info.channels);
	if(format == 0) {
		alDeleteBuffers(1, &buffer);
		free(pcmData);
		return 0;
	}
	alBufferData(buffer, format, pcmData, totalPcm * mp3Info.channels * sizeof(short), mp3Info.sampleRate);
	free(pcmData);
	return buffer;
}


// ogg file

expo
ALuint loadOggFromFile(const char* filePath)
{
	int channels = 0;
	int sampleRate = 0;
	ALuint format = 0;
	ALuint buffer = 0;
	short* pcmData = NULL;
	alGenBuffers(1, &buffer);
	if(!alIsBuffer(buffer)) {
		return 0;
	}
	int samples  =  stb_vorbis_decode_filename(filePath, &channels, &sampleRate, &pcmData);
	if(samples < 1 || channels == 0 || sampleRate == 0 || pcmData == NULL)
	{
		alDeleteBuffers(1, &buffer);
		return 0;
	}
	format = getFormat(channels);
	if(format == 0) {
		alDeleteBuffers(1, &buffer);
		free(pcmData);
		return 0;
	}
	alBufferData(buffer, format, pcmData, samples * channels * sizeof(short), sampleRate);
	free(pcmData);
	return buffer;
}

expo
ALuint loadOggFromMemory(size_t dataSize, const void* data)
{
	int channels = 0;
	int sampleRate = 0;
	ALuint format = 0;
	ALuint buffer = 0;
	short* pcmData = NULL;
	alGenBuffers(1, &buffer);
	if(!alIsBuffer(buffer)) {
		return 0;
	}
	int samples  = stb_vorbis_decode_memory((const unsigned char*)data, (int)dataSize, &channels, &sampleRate, &pcmData); 
	if(samples < 1 || channels == 0 || sampleRate == 0 || pcmData == NULL) {
		alDeleteBuffers(1, &buffer);
		return 0;
	}
	format = getFormat(channels);
	if(format == 0) {
		alDeleteBuffers(1, &buffer);
		free(pcmData);
		return 0;
	}
	alBufferData(buffer, format, pcmData, samples * channels * sizeof(short), sampleRate);
	free(pcmData);
	return buffer;
}

// wav files

expo
ALuint loadWavFromFile(const char* filePath)
{
	ALuint format = 0;
	ALuint buffer = 0;
	short* pcmData = NULL;
	drwav wavFile;
	if(!drwav_init_file(&wavFile, filePath, NULL))
		return 0;
	pcmData = new short[wavFile.totalPCMFrameCount * wavFile.channels];
	if(pcmData == NULL) {
		drwav_uninit(&wavFile);
		return 0;
	}
	size_t numFrames = drwav_read_pcm_frames_s16(&wavFile, wavFile.totalPCMFrameCount, pcmData);
	if (numFrames < 1) {
		drwav_uninit(&wavFile);
		delete []pcmData;
		return 0;
	}
	drwav_uninit(&wavFile);
	format = getFormat(wavFile.channels);
	if(format == 0) {
		delete []pcmData;
		return 0;
	}
	alGenBuffers(1, &buffer);
	if(!alIsBuffer(buffer)) {
		delete []pcmData;
		return 0;
	}
	alBufferData(buffer, format, pcmData, numFrames * wavFile.channels * sizeof(short), wavFile.sampleRate);
	delete []pcmData;
	return buffer;
}

expo
ALuint loadWavFromMemory(size_t dataSize, const void* data)
{
	ALuint format = 0;
	ALuint buffer = 0;
	short* pcmData = NULL;
	drwav memWavFile;
	if(!drwav_init_memory(&memWavFile, data, dataSize, NULL))
		return 0;
	pcmData = new short[memWavFile.totalPCMFrameCount * memWavFile.channels];
	if(pcmData == NULL) {
		drwav_uninit(&memWavFile);
		return 0;
	}
	size_t numFrames = drwav_read_pcm_frames_s16(&memWavFile, memWavFile.totalPCMFrameCount, pcmData);
	if (numFrames == 0) {
		drwav_uninit(&memWavFile);
		delete []pcmData;
		return 0;
	}
	drwav_uninit(&memWavFile);
	format = getFormat(memWavFile.channels);
	if(format == 0) {
		delete []pcmData;
		return 0;
	}
	alGenBuffers(1, &buffer);
	if(!alIsBuffer(buffer)) {
		delete []pcmData;
		return 0;
	}
	alBufferData(buffer, format, pcmData, numFrames * memWavFile.channels * sizeof(short), memWavFile.sampleRate);
	delete []pcmData;
	return buffer;
}


// c api


expo
Stream newStream(int decoder)
{
	auto s = newAudioStream(stream::DecoderType(decoder));
	Stream cs;
	cs.stream = static_cast<void*>(s);
	return cs;
}

expo
int deleteStream(Stream* cStream)
{
	auto s = static_cast<stream::AudioStream*>(cStream->stream);
	auto retVal = destroyStream(s);
	cStream->stream = NULL;
	return retVal;
}

expo
int openStreamFromFile(Stream* cStream, const char* filePath)
{
	auto s = static_cast<stream::AudioStream*>(cStream->stream);
	return s->openFromFile(filePath);
}

expo
int openStreamFromMemory(Stream* cStream, const void *data, size_t size)
{
	auto s = static_cast<stream::AudioStream*>(cStream->stream);
	return s->openFromMemory(data, size);
}

expo
int streamPlay(Stream* cStream)
{
	auto s = static_cast<stream::AudioStream*>(cStream->stream);
	return s->play();
}

expo
int streamPause(Stream* cStream)
{
	auto s = static_cast<stream::AudioStream*>(cStream->stream);
	return s->pause();
}

expo
int streamStop(Stream* cStream)
{
	auto s = static_cast<stream::AudioStream*>(cStream->stream);
	return s->stop();
}

expo
void streamSetLoop(Stream* cStream, int loop)
{
	auto s = static_cast<stream::AudioStream*>(cStream->stream);
	s->setLoop(loop);
}

expo
int streamGetLoop(Stream* cStream)
{
	auto s = static_cast<stream::AudioStream*>(cStream->stream);
	return s->getLoop();
}

expo
int streamSeek(Stream* cStream, size_t millisecond)
{
	auto s = static_cast<stream::AudioStream*>(cStream->stream);
	return s->seek(millisecond);
}

expo
int streamUpdate(Stream* cStream)
{
	auto s = static_cast<stream::AudioStream*>(cStream->stream);
	return s->update();
}
