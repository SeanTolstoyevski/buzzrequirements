#ifndef _BUZZ_GENERAL_HPP_
#define _BUZZ_GENERAL_HPP_

#include "AL/al.h"
#include "AL/alext.h"
#include <stddef.h>

#if defined(_WIN32)
	#define expo __declspec(dllexport)
	#define impi __cdecl
#else
	#define expo extern
	#define impi
#endif

#ifdef __cplusplus
extern "C"
{
#endif


#define DecoderFlac 1
#define DecoderMp3 2
#define DecoderOgg 3
#define DecoderWav 4


expo
ALenum getFormat(ALuint  channel);

expo
void setBufferSize(size_t size);

expo
void setBufferCount(size_t number);

expo
unsigned char getBufferCount();

// device

expo
const char* getAllOutputDevices();

// stream

typedef struct Stream
{
	void *stream;
} Stream;

expo
Stream newStream(int decoder);

expo
int reopenFromFileStream(Stream* cStream, const char* path, unsigned char decoder);

expo
int reopenFromMemoryStream(Stream* cStream, const void* data, size_t dataSize, unsigned char decoder);

expo
int deleteStream(Stream* stream);

expo
int openStreamFromFile(Stream* stream, const char* filePath);

expo
int openStreamFromMemory(Stream* stream, const void *data, size_t size);

expo
int streamPlay(Stream* stream);

expo
int streamPause(Stream* stream);

expo
int streamStop(Stream* stream);

expo
int streamSeek(Stream* stream, size_t millisecond);

expo
void streamSetLoop(Stream* cStream, int loop);

expo
int streamGetLoop(Stream* cStream);

expo
ALuint streamGetSource(Stream* cStream);

expo
int streamUpdate( Stream* stream);

// files

expo
ALuint loadFlacFromFile(const char *filePath);

expo
ALuint loadFlacFromMemory(size_t dataSize, const void* data);


expo
ALuint loadMp3FromFile(const char *filePath);

expo
ALuint loadMp3FromMemory(size_t dataSize, const void* data);

expo
ALuint loadOggFromFile(const char *filePath);

expo
ALuint loadOggFromMemory(size_t dataSize, const void* data);

expo
ALuint loadWavFromFile(const char *filePath);

expo
ALuint loadWavFromMemory(size_t dataSize, const void* data);

// function pointers

expo
const ALCchar* h_alcGetStringiSOFT(ALCdevice *device, ALCenum paramName, ALCsizei index);

expo
const ALCchar* h_d_alcGetStringiSOFT(ALCdevice *device, ALCenum paramName, ALCsizei index);
expo
ALCboolean h_alcResetDeviceSOFT(ALCdevice *device, const ALCint *attribs);

expo
ALCboolean h_d_alcResetDeviceSOFT(ALCdevice *device, const ALCint *attribs);

expo
void h_alGenEffects(ALsizei n, ALuint *effects);

expo
void h_alGenFilters(ALsizei n, ALuint *filters);

expo
void h_alDeleteFilters(ALsizei n, const ALuint *filters);

expo
ALboolean h_alIsFilter(ALuint filter);

expo
void h_alFilteri(ALuint filter, ALenum param, ALint iValue);

expo
void h_alFilteriv(ALuint filter, ALenum param, const ALint *piValues);

expo
void h_alFilterf(ALuint filter, ALenum param, ALfloat flValue);

expo
void h_alFilterfv(ALuint filter, ALenum param, const ALfloat *pflValues);

expo
void h_alGetFilteri(ALuint filter, ALenum param, ALint *piValue);

expo
void h_alGetFilteriv(ALuint filter, ALenum param, ALint *piValues);

expo
void h_alGetFilterf(ALuint filter, ALenum param, ALfloat *pflValue);

expo
void h_alGetFilterfv(ALuint filter, ALenum param, ALfloat *pflValues);

expo
void h_alDeleteEffects(ALsizei n, const ALuint *effects);

expo
ALboolean h_alIsEffect(ALuint effect);

expo
void h_alEffecti(ALuint effect, ALenum param, ALint iValue);

expo
void h_alEffectiv(ALuint effect, ALenum param, const ALint *piValues);

expo
void h_alEffectf(ALuint effect, ALenum param, ALfloat flValue);

expo
void h_alEffectfv(ALuint effect, ALenum param, const ALfloat *pflValues);

expo
void h_alGetEffecti(ALuint effect, ALenum param, ALint *piValue);

expo
void h_alGetEffectiv(ALuint effect, ALenum param, ALint *piValues);

expo
void h_alGetEffectf(ALuint effect, ALenum param, ALfloat *pflValue);

expo
void h_alGetEffectfv(ALuint effect, ALenum param, ALfloat *pflValues);

expo
void h_alGenAuxiliaryEffectSlots(ALsizei n, ALuint *effectslots);

expo
void h_alDeleteAuxiliaryEffectSlots(ALsizei n, const ALuint *effectslots);

expo
ALboolean  h_alIsAuxiliaryEffectSlot(ALuint effectslot);

expo
void h_alAuxiliaryEffectSloti(ALuint effectslot, ALenum param, ALint iValue);

expo
void h_alAuxiliaryEffectSlotiv(ALuint effectslot, ALenum param, const ALint *piValues);

expo
void h_alAuxiliaryEffectSlotf(ALuint effectslot, ALenum param, ALfloat flValue);

expo
void h_alAuxiliaryEffectSlotfv(ALuint effectslot, ALenum param, const ALfloat *pflValues);

expo
void h_alGetAuxiliaryEffectSloti(ALuint effectslot, ALenum param, ALint *piValue);

expo
void h_alGetAuxiliaryEffectSlotiv(ALuint effectslot, ALenum param, ALint *piValues);

expo
void h_alGetAuxiliaryEffectSlotf(ALuint effectslot, ALenum param, ALfloat *pflValue);

expo
void h_alGetAuxiliaryEffectSlotfv(ALuint effectslot, ALenum param, ALfloat *pflValues);

expo
const ALchar* h_alGetStringiSOFT(ALenum pname, ALsizei index);


#ifdef __cplusplus
}
#endif

#endif