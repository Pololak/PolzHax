#ifndef __AUDIOEFFECTSLAYER_H__
#define __AUDIOEFFECTSLAYER_H__

#include <gd.h>
#include "cocos2d.h"

namespace gd {

	class AudioEffectsLayer {
	public:

		float getAudioScale() {
			return from<float>(this, 0x1AC);
		}
	};
}
#endif
