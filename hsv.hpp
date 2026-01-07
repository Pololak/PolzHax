#pragma once

namespace color_utils {
	struct RGB {
		double r, g, b;
	};

	struct HSV {
		// h is 0-360
		double h, s, v;
	};

	// https://stackoverflow.com/a/6930407/9124836

	inline HSV rgb_to_hsv(const RGB& in) {
		HSV out;
		double min, max, delta;

		min = in.r < in.g ? in.r : in.g;
		min = min  < in.b ? min  : in.b;

		max = in.r > in.g ? in.r : in.g;
		max = max  > in.b ? max  : in.b;

		out.v = max;
		delta = max - min;
		if (delta < 0.00001) {
			out.s = 0;
			out.h = 0; // undefined, maybe nan?
			return out;
		}

		if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
			out.s = delta / max;
		} else {
			// if max is 0, then r = g = b = 0
			// s = 0, h is undefined
			out.s = 0.0;
			out.h = 0.0; // 0 is better than nan sorry
			return out;
		}
		if (in.r >= max) // > is bogus, just keeps compilor happy
			out.h = (in.g - in.b) / delta;
		else if (in.g >= max)
			out.h = 2.0 + (in.b - in.r) / delta;
		else
			out.h = 4.0 + (in.r - in.g) / delta;
		out.h *= 60.0;

		if (out.h < 0.0)
			out.h += 360.0;

		return out;
	}

	inline RGB hsv_to_rgb(const HSV& in) {
		if (in.s <= 0.0) {
			return { in.v, in.v, in.v };
		}

		double hh = in.h;
		if (hh >= 360.0) hh = 0.0;
		hh /= 60.0;
		int i = static_cast<int>(hh);
		double ff = hh - i;
		double p = in.v * (1.0 - in.s);
		double q = in.v * (1.0 - (in.s * ff));
		double t = in.v * (1.0 - (in.s * (1.0 - ff)));

		switch (i) {
		case 0:
			return { in.v, t, p };
		case 1:
			return { q, in.v, p };
		case 2:
			return { p, in.v, t, };
		case 3:
			return { p, q, in.v, };
		case 4:
			return { t, p, in.v, };
		case 5:
		default:
			return { in.v, p, q, };
		}
	}
}