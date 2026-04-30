#include "PitchShifter.hpp"

//FMOD::DSP* pitchShifter = nullptr;

void PitchShifter::setPitch(float pitch) {
	//FMOD::DSP* pitchShifter = nullptr;
	//auto fme = gd::FMODAudioEngine::sharedEngine();
	//FMOD::System* system = fme->m_system;

	//if (pitch == 1.f) {
	//	if (pitchShifter) {
	//		fme->m_globalChannel->removeDSP(pitchShifter);
	//		pitchShifter->release();
	//		pitchShifter = nullptr;
	//	}
	//	return;
	//}

	//if (!pitchShifter) {
	//	if (system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &pitchShifter) != FMOD_OK) {
	//		return;
	//	}
	//	fme->m_globalChannel->addDSP(0, pitchShifter);
	//}

	//pitchShifter->setParameterFloat(FMOD_DSP_PITCHSHIFT_FFTSIZE, 0x800);
	//pitchShifter->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, pitch);



	//auto engine = gd::FMODAudioEngine::sharedEngine();

	//if (!engine || !engine->m_system || !engine->m_globalChannel)
	//	return;

	//if (pitchShifter)
	//	engine->m_globalChannel->removeDSP(pitchShifter);

	//pitchShifter->release();
	//pitchShifter = nullptr;

	//engine->m_system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &pitchShifter);

	//pitchShifter->setParameterFloat(FMOD_DSP_PITCHSHIFT_FFTSIZE, 4096);

	//pitchShifter->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, pitch);

	//engine->m_globalChannel->addDSP(0, pitchShifter);



	static FMOD::DSP* pitchShifter = nullptr;
	auto fmodAudioEngine = gd::FMODAudioEngine::sharedEngine();
	FMOD::System* system = fmodAudioEngine->m_system;

	if (pitchShifter) {
		fmodAudioEngine->m_globalChannel->removeDSP(pitchShifter);
		pitchShifter->release();
		pitchShifter = nullptr;
	}

	if (pitch == 1.f) {
		return;
	}

	system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &pitchShifter);
	pitchShifter->setParameterFloat(FMOD_DSP_PITCHSHIFT_FFTSIZE, 0x800);
	pitchShifter->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, pitch);
	fmodAudioEngine->m_globalChannel->addDSP(0, pitchShifter);
}