#pragma once
#include "include.h"

#define CCARRAY_FOREACH_B_BASE(__array__, __obj__, __type__, __index__)                                                                    \
    if (__array__ && __array__->count())                                                                                                   \
        for (auto [__index__, __obj__] = std::tuple<unsigned int, __type__> {0u, nullptr};                                                 \
             (__index__ < __array__->count() && (__obj__ = reinterpret_cast<__type__>(__array__->objectAtIndex(__index__)))); __index__++)

#define CCARRAY_FOREACH_B_TYPE(__array__, __obj__, __type__) CCARRAY_FOREACH_B_BASE(__array__, __obj__, __type__*, ix)

inline std::string colorToString(int id) {
	switch (id) {
	case 0: return "Default"; break;
	case 1: return "P-Col1"; break;
	case 2: return "P-Col2"; break;
	case 3: return "Col1"; break;
	case 4: return "Col2"; break;
	case 5: return "LBG"; break;
	case 6: return "Col3"; break;
	case 7: return "Col4"; break;
	case 8: return "3DL"; break;
	case 9: return "White"; break;
	default: return "Unknown"; break;
	}
}

inline std::string typeToString(GameObjectType type) {
	switch (type) {
	case GameObjectType::Solid: return "Solid"; break;
	case GameObjectType::Hazard: return "Hazard"; break;
	case GameObjectType::InverseGravityPortal: return "Inverse Gravity Portal"; break;
	case GameObjectType::NormalGravityPortal: return "Normal Gravity Portal"; break;
	case GameObjectType::ShipPortal: return "Ship Portal"; break;
	case GameObjectType::CubePortal: return "Cube Portal"; break;
	case GameObjectType::Decoration: return "Decoration"; break;
	case GameObjectType::PulsingDecoration: return "Pulse Object"; break;
	case GameObjectType::YellowJumpPad: return "Yellow Jump Pad"; break;
	case GameObjectType::PinkJumpPad: return "Pink Jump Pad"; break;
	case GameObjectType::GravityPad: return "Gravity Pad"; break;
	case GameObjectType::YellowJumpRing: return "Yellow Jump Ring"; break;
	case GameObjectType::PinkJumpRing: return "Pink Jump Ring"; break;
	case GameObjectType::GravityRing: return "Gravity Ring"; break;
	case GameObjectType::InverseMirrorPortal: return "Inverse Mirror Portal"; break;
	case GameObjectType::NormalMirrorPortal: return "Normal Mirror Portal"; break;
	case GameObjectType::BallPortal: return "Ball Portal"; break;
	case GameObjectType::RegularSizePortal: return "Regular Size Portal"; break;
	case GameObjectType::MiniSizePortal: return "Mini Size Portal"; break;
	case GameObjectType::UfoPortal: return "UFO Portal"; break;
	case GameObjectType::Modifier: return "Modifier"; break;
	case GameObjectType::Breakable: return "Breakable"; break;
	case GameObjectType::SecretCoin: return "Secret Coin"; break;
	case GameObjectType::DualPortal: return "Dual Portal"; break;
	case GameObjectType::SoloPortal: return "Solo Portal"; break;
	case GameObjectType::Slope: return "Slope"; break;
	case GameObjectType::WavePortal: return "Wave Portal"; break;
	default: return "Unknown"; break;
	}
}