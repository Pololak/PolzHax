#include "PitchShifter.hpp"

void PitchShifter::setPitch(float pitch) {
	FMOD::DSP* pitchShifter = nullptr;
	auto fme = gd::FMODAudioEngine::sharedEngine();
	FMOD::System* system = fme->m_system;

	if (pitch == 1.f) {
		if (pitchShifter) {
			fme->m_globalChannel->removeDSP(pitchShifter);
			pitchShifter->release();
			pitchShifter = nullptr;
		}
		return;
	}

	if (!pitchShifter) {
		if (system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &pitchShifter) != FMOD_OK) {
			return;
		}
		fme->m_globalChannel->addDSP(0, pitchShifter);
	}

	pitchShifter->setParameterFloat(FMOD_DSP_PITCHSHIFT_FFTSIZE, 0x800);
	pitchShifter->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, pitch);
}