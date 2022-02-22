// This file was created as a helper because Golang does not support C function pointers and c-like strings.
//
// OpenAL-Soft uses many effect functions as function pointers.
// These functions need to be taken with **alGetProcAdress("funcName")**
// and converted to the desired function type.
//
// Usually it defines a macro and this macro loads all function pointers.
// No macro is used and defined in this file. The whole process proceeds dynamically.
// This ensures that functions still work when OpenAL-Soft is restarted or in a similar scenario.
//
// However, the memory load (high memory usage, leaks...) and speed of this dynamic process has not been tested.
// It could be a move to a sensible and well-designed macros.
//
// Code structure of the file:
//
// h_ : "helper_"
//    Prepended to the name of each function so that it does not conflict with OpenAL function names.
// v_h_ : variable_helper_
//    Added to each function pointer variable name  to create an orderly structure and avoid clutter.
#include <string>
#include <cstring>

#include "general.hpp"
#include "AL/al.h"
#include "AL/alext.h"
#include "AL/alc.h"

static std::string deviceList;

// OpenAL-Soft creates drive name strings differently than standard cstrings, so we use c++ as an helper.
// I will be grateful for any suggestions to move this function  to pure Golang.
expo
const char* getAllOutputDevices()
{
	if(!deviceList.empty())
		deviceList.clear();

	const char* dlist;
	if(alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT") != AL_FALSE)
		dlist = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
	else
		dlist = alcGetString(NULL, ALC_DEVICE_SPECIFIER);

	do {
		deviceList += std::string(dlist) + " --- "; // In Golang we add a separator to split it up
		dlist += strlen(dlist) + 1;
	} while(*dlist != '\0');
	if(!deviceList.empty())
		deviceList.erase(deviceList.size()-5, 5);
	return deviceList.c_str();
}


// typedef void (AL_APIENTRY*PFNALBUFFERDATASTATICPROC)(const ALint,ALenum,ALvoid*,ALsizei,ALsizei);
// AL_API void AL_APIENTRY alBufferDataStatic(const ALint buffer, ALenum format, ALvoid *data, ALsizei len, ALsizei freq);
//
// ALC_API void ALC_APIENTRY alcDevicePauseSOFT(ALCdevice *device);
// ALC_API void ALC_APIENTRY alcDeviceResumeSOFT(ALCdevice *device);
//

expo
const ALCchar* h_alcGetStringiSOFT(ALCdevice *device, ALCenum paramName, ALCsizei index) {
	LPALCGETSTRINGISOFT v_h_alcGetStringiSOFT;
	v_h_alcGetStringiSOFT = (LPALCGETSTRINGISOFT)alGetProcAddress("alcGetStringiSOFT");
	return v_h_alcGetStringiSOFT(device, paramName, index);
}

expo
const ALCchar* h_d_alcGetStringiSOFT(ALCdevice *device, ALCenum paramName, ALCsizei index) {
	LPALCGETSTRINGISOFT v_h_alcGetStringiSOFT;
	v_h_alcGetStringiSOFT = (LPALCGETSTRINGISOFT)alcGetProcAddress(device, "alcGetStringiSOFT");

	return v_h_alcGetStringiSOFT(device, paramName, index);
}

expo
ALCboolean h_alcResetDeviceSOFT(ALCdevice *device, const ALCint *attribs) {
	LPALCRESETDEVICESOFT v_h_alcResetDeviceSOFT;
	v_h_alcResetDeviceSOFT  = (LPALCRESETDEVICESOFT)alGetProcAddress("alcResetDeviceSOFT");
	return v_h_alcResetDeviceSOFT(device, attribs);
}

expo
ALCboolean h_d_alcResetDeviceSOFT(ALCdevice *device, const ALCint *attribs) {
	LPALCRESETDEVICESOFT v_h_alcResetDeviceSOFT;
	v_h_alcResetDeviceSOFT  = (LPALCRESETDEVICESOFT)alcGetProcAddress(device, "alcResetDeviceSOFT");
	return v_h_alcResetDeviceSOFT(device, attribs);
}

expo
void h_alGenEffects(ALsizei n, ALuint *effects) {
	LPALGENEFFECTS v_h_alGenEffects;
	v_h_alGenEffects = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");

	v_h_alGenEffects(n, effects);
}

expo
void h_alGenFilters(ALsizei n, ALuint *filters) {
	LPALGENFILTERS v_h_alGenFilters;
	v_h_alGenFilters = (LPALGENFILTERS)alGetProcAddress("alGenFilters");
	v_h_alGenFilters(n, filters);
}

expo
void h_alDeleteFilters(ALsizei n, const ALuint *filters) {
	LPALDELETEFILTERS v_h_alDeleteFilters;
	v_h_alDeleteFilters = (LPALDELETEFILTERS)alGetProcAddress("alDeleteFilters");
	v_h_alDeleteFilters(n, filters);
}

expo
ALboolean h_alIsFilter(ALuint filter) {
	LPALISFILTER v_h_alIsFilter;
	v_h_alIsFilter = (LPALISFILTER)alGetProcAddress("alIsFilter");
	return v_h_alIsFilter(filter);
}

expo
void h_alFilteri(ALuint filter, ALenum param, ALint iValue) {
	LPALFILTERI v_h_alFilteri;
	v_h_alFilteri = (LPALFILTERI)alGetProcAddress("alFilteri");
	v_h_alFilteri(filter, param, iValue);
}

expo
void h_alFilteriv(ALuint filter, ALenum param, const ALint *piValues) {
	LPALFILTERIV v_h_alFilteriv;
	v_h_alFilteriv = (LPALFILTERIV)alGetProcAddress("alFilteriv");
	v_h_alFilteriv(filter, param, piValues);
}

expo
void h_alFilterf(ALuint filter, ALenum param, ALfloat flValue) {
	LPALFILTERF v_h_alFilterf;
	v_h_alFilterf = (LPALFILTERF)alGetProcAddress("alFilterf");
v_h_alFilterf(filter, param, flValue);
}

expo
void h_alFilterfv(ALuint filter, ALenum param, const ALfloat *pflValues) {
	LPALFILTERFV v_h_alFilterfv;
	v_h_alFilterfv = (LPALFILTERFV)alGetProcAddress("alFilterfv");
	v_h_alFilterfv(filter, param, pflValues);
}

expo
void h_alGetFilteri(ALuint filter, ALenum param, ALint *piValue) {
	LPALGETFILTERI v_h_alGetFilteri;
	v_h_alGetFilteri = (LPALGETFILTERI)alGetProcAddress("alGetFilteri");
	v_h_alGetFilteri(filter, param, piValue);
}

expo
void h_alGetFilteriv(ALuint filter, ALenum param, ALint *piValues) {
	LPALGETFILTERIV v_h_alGetFilteriv;
	v_h_alGetFilteriv = (LPALGETFILTERIV)alGetProcAddress("alGetFilteriv");
	v_h_alGetFilteriv(filter, param, piValues);
}

expo
void h_alGetFilterf(ALuint filter, ALenum param, ALfloat *pflValue) {
	LPALGETFILTERF v_h_alGetFilterf;
	v_h_alGetFilterf = (LPALGETFILTERF)alGetProcAddress("alGetFilterf");
	v_h_alGetFilterf(filter, param, pflValue);
}

expo
void h_alGetFilterfv(ALuint filter, ALenum param, ALfloat *pflValues) {
	LPALGETFILTERFV v_h_alGetFilterfv;
	v_h_alGetFilterfv = (LPALGETFILTERFV)alGetProcAddress("alGetFilterfv");
	v_h_alGetFilterfv(filter, param, pflValues);
}

expo
void h_alDeleteEffects(ALsizei n, const ALuint *effects) {
	LPALDELETEEFFECTS v_h_alDeleteEffects;
	v_h_alDeleteEffects = (LPALDELETEEFFECTS)alGetProcAddress("alDeleteEffects");
	v_h_alDeleteEffects(n, effects);
}

expo
ALboolean h_alIsEffect(ALuint effect) {
	LPALISEFFECT v_h_alIsEffect;
	v_h_alIsEffect = (LPALISEFFECT)alGetProcAddress("alIsEffect");
	return h_alIsEffect(effect);
}

expo
void h_alEffecti(ALuint effect, ALenum param, ALint iValue) {
	LPALEFFECTI v_h_alEffecti;
	v_h_alEffecti = (LPALEFFECTI)alGetProcAddress("alEffecti");
	v_h_alEffecti(effect, param, iValue);
}

expo
void h_alEffectiv(ALuint effect, ALenum param, const ALint *piValues) {
	LPALEFFECTIV v_h_alEffectiv;
	v_h_alEffectiv = (LPALEFFECTIV)alGetProcAddress("alEffectiv");
	v_h_alEffectiv(effect, param, piValues);
}

expo
void h_alEffectf(ALuint effect, ALenum param, ALfloat flValue) {
	LPALEFFECTF v_h_alEffectf;
	v_h_alEffectf = (LPALEFFECTF)alGetProcAddress("alEffectf");
	v_h_alEffectf(effect, param, flValue);
}

expo
void h_alEffectfv(ALuint effect, ALenum param, const ALfloat *pflValues) {
	LPALEFFECTFV v_h_alEffectfv;
	v_h_alEffectfv = (LPALEFFECTFV)alGetProcAddress("alEffectfv");
	v_h_alEffectfv(effect, param, pflValues);
}

expo
void h_alGetEffecti(ALuint effect, ALenum param, ALint *piValue) {
	LPALGETEFFECTI v_h_alGetEffecti;
	v_h_alGetEffecti = (LPALGETEFFECTI)alGetProcAddress("alGetEffecti");
	v_h_alGetEffecti(effect, param, piValue);
}

expo
void h_alGetEffectiv(ALuint effect, ALenum param, ALint *piValues) {
	LPALGETEFFECTIV v_h_alGetEffectiv;
	v_h_alGetEffectiv = (LPALGETEFFECTIV)alGetProcAddress("alGetEffectiv");
	v_h_alGetEffectiv(effect, param, piValues);
}

expo
void h_alGetEffectf(ALuint effect, ALenum param, ALfloat *pflValue) {
	LPALGETEFFECTF v_h_alGetEffectf;
	v_h_alGetEffectf = (LPALGETEFFECTF)alGetProcAddress("alGetEffectf");
	v_h_alGetEffectf(effect, param, pflValue);
}

expo
void h_alGetEffectfv(ALuint effect, ALenum param, ALfloat *pflValues) {
	LPALGETEFFECTFV v_h_alGetEffectfv;
	v_h_alGetEffectfv = (LPALGETEFFECTFV)alGetProcAddress("alGetEffectfv");
	v_h_alGetEffectfv(effect, param, pflValues);
}

expo
void h_alGenAuxiliaryEffectSlots(ALsizei n, ALuint *effectslots) {
	LPALGENAUXILIARYEFFECTSLOTS v_h_alGenAuxiliaryEffectSlots;
	v_h_alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS)alGetProcAddress("alGenAuxiliaryEffectSlots");
	v_h_alGenAuxiliaryEffectSlots(n, effectslots);
}

expo
void h_alDeleteAuxiliaryEffectSlots(ALsizei n, const ALuint *effectslots) {
	LPALDELETEAUXILIARYEFFECTSLOTS v_h_alDeleteAuxiliaryEffectSlots;
	v_h_alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");
	v_h_alDeleteAuxiliaryEffectSlots(n, effectslots);
}

expo
ALboolean  h_alIsAuxiliaryEffectSlot(ALuint effectslot) {
	LPALISAUXILIARYEFFECTSLOT v_h_alIsAuxiliaryEffectSlot;
	v_h_alIsAuxiliaryEffectSlot = (LPALISAUXILIARYEFFECTSLOT)alGetProcAddress("alIsAuxiliaryEffectSlot");
	return v_h_alIsAuxiliaryEffectSlot(effectslot);
}

expo
void h_alAuxiliaryEffectSloti(ALuint effectslot, ALenum param, ALint iValue) {
	LPALAUXILIARYEFFECTSLOTI v_h_alAuxiliaryEffectSloti;
	v_h_alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI)alGetProcAddress("alAuxiliaryEffectSloti");
	v_h_alAuxiliaryEffectSloti(effectslot, param, iValue);
}

expo
void h_alAuxiliaryEffectSlotiv(ALuint effectslot, ALenum param, const ALint *piValues) {
	LPALAUXILIARYEFFECTSLOTIV v_h_alAuxiliaryEffectSlotiv;
	v_h_alAuxiliaryEffectSlotiv = (LPALAUXILIARYEFFECTSLOTIV)alGetProcAddress("alAuxiliaryEffectSlotiv");
	v_h_alAuxiliaryEffectSlotiv(effectslot, param, piValues);
}

expo
void h_alAuxiliaryEffectSlotf(ALuint effectslot, ALenum param, ALfloat flValue) {
	LPALAUXILIARYEFFECTSLOTF v_h_alAuxiliaryEffectSlotf;
	v_h_alAuxiliaryEffectSlotf = (LPALAUXILIARYEFFECTSLOTF)alGetProcAddress("alAuxiliaryEffectSlotf");
	v_h_alAuxiliaryEffectSlotf(effectslot, param, flValue);
}

expo
void h_alAuxiliaryEffectSlotfv(ALuint effectslot, ALenum param, const ALfloat *pflValues) {
	LPALAUXILIARYEFFECTSLOTFV v_h_alAuxiliaryEffectSlotfv;
	v_h_alAuxiliaryEffectSlotfv = (LPALAUXILIARYEFFECTSLOTFV)alGetProcAddress("alAuxiliaryEffectSlotfv");
	v_h_alAuxiliaryEffectSlotfv(effectslot, param, pflValues);
}

expo
void h_alGetAuxiliaryEffectSloti(ALuint effectslot, ALenum param, ALint *piValue) {
	LPALGETAUXILIARYEFFECTSLOTI v_h_alGetAuxiliaryEffectSloti;
	v_h_alGetAuxiliaryEffectSloti = (LPALGETAUXILIARYEFFECTSLOTI)alGetProcAddress("alGetAuxiliaryEffectSloti");
	v_h_alGetAuxiliaryEffectSloti(effectslot,param, piValue);
}

expo
void h_alGetAuxiliaryEffectSlotiv(ALuint effectslot, ALenum param, ALint *piValues) {
	LPALGETAUXILIARYEFFECTSLOTIV v_h_alGetAuxiliaryEffectSlotiv;
	v_h_alGetAuxiliaryEffectSlotiv = (LPALGETAUXILIARYEFFECTSLOTIV)alGetProcAddress("alGetAuxiliaryEffectSlotiv");
	v_h_alGetAuxiliaryEffectSlotiv(effectslot, param, piValues);
}

expo
void h_alGetAuxiliaryEffectSlotf(ALuint effectslot, ALenum param, ALfloat *pflValue) {
	LPALGETAUXILIARYEFFECTSLOTF v_h_alGetAuxiliaryEffectSlotf;
	v_h_alGetAuxiliaryEffectSlotf = (LPALGETAUXILIARYEFFECTSLOTF) alGetProcAddress("alGetAuxiliaryEffectSlotf");
	v_h_alGetAuxiliaryEffectSlotf(effectslot, param, pflValue);
}

expo
void h_alGetAuxiliaryEffectSlotfv(ALuint effectslot, ALenum param, ALfloat *pflValues) {
	LPALGETAUXILIARYEFFECTSLOTFV v_h_alGetAuxiliaryEffectSlotfv;
	v_h_alGetAuxiliaryEffectSlotfv = (LPALGETAUXILIARYEFFECTSLOTFV)alGetProcAddress("alGetAuxiliaryEffectSlotfv");
	v_h_alGetAuxiliaryEffectSlotfv(effectslot, param, pflValues);
}

expo
const ALchar* h_alGetStringiSOFT(ALenum pname, ALsizei index) {
	LPALGETSTRINGISOFT getStringiSOFT;
	getStringiSOFT = (LPALGETSTRINGISOFT)alGetProcAddress("alGetStringiSOFT");
	return getStringiSOFT(pname, index);
}

