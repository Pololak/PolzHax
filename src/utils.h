#pragma once
#include "include.h"
#include <sys/stat.h>
#include "../libs/cocos2dx/support/base64.h"
#include "hsv.h"

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

inline bool create_directories(const std::string& path) {
    std::stringstream ss(path);
    std::string item;
    std::string current_path = "";

    while (std::getline(ss, item, '/')) {
        if (item.empty()) continue;
        current_path += "/" + item;
        if (mkdir(current_path.c_str(), 512) != 0) {
            if (errno != EEXIST) return false;
        }
    }
    return true;
}

inline std::pair<std::string, std::string> split_once(const std::string& str, char split) {
	const auto n = str.find(split);
	return { str.substr(0, n), str.substr(n + 1) };
}

namespace base64 {
	inline std::string encode(std::experimental::string_view str) {
		char* out;
		const auto size = cocos2d::base64Encode(str.data(), str.size(), &out, false);
		std::string outs(out);
		free(out);
		return outs;
	}

	inline std::string decode(std::experimental::string_view str) {
		char* out;
		const auto size = cocos2d::base64Decode(str.data(), str.size(), &out);
		std::string outs(out, size);
		free(out);
		return outs;
	}
}

inline std::vector<char> readFile(const char* filename) {
    std::basic_ifstream<char> file(filename, std::ios::binary);

    return std::vector<char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

inline cocos2d::ccColor3B getLightBGColor(ccColor3B bg, ccColor3B p1) {
	auto hsv = color_utils::rgb_to_hsv({ bg.r / 255., bg.g / 255., bg.b / 255. });
	hsv.s = (std::max)(hsv.s - 0.2, 0.0);
	hsv.v = (std::min)(hsv.v + 0.2, 1.0);

	const auto rgb = color_utils::hsv_to_rgb(hsv);

	const ccColor3B lbg = { static_cast<GLubyte>(rgb.r * 255.), static_cast<GLubyte>(rgb.g * 255.), static_cast<GLubyte>(rgb.b * 255.) };

	const auto amt = (static_cast<float>(bg.r) + static_cast<float>(bg.g) + static_cast<float>(bg.b)) / 150.f;

	if (amt < 1.f) {
		return GameToolbox::getMixedColor(lbg, p1, amt);
	}
	else {
		return lbg;
	}
}