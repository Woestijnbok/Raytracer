#pragma once

namespace dae
{
	struct ColorRGB
	{
		static ColorRGB Lerp(const ColorRGB& c1, const ColorRGB& c2, float factor);

		void MaxToOne();

		ColorRGB& operator+=(const ColorRGB& c);
		ColorRGB& operator-=(const ColorRGB& c);
		ColorRGB& operator*=(const ColorRGB& c);
		ColorRGB& operator*=(float s);
		ColorRGB& operator/=(const ColorRGB& c);
		ColorRGB& operator/=(float s);

		float r;
		float g;
		float b;
	};

	inline ColorRGB operator*(float lhs, const ColorRGB& rhs)
	{
		return ColorRGB{ lhs * rhs.r, lhs * rhs.g, lhs * rhs.b };
	}

	inline ColorRGB operator*(const ColorRGB& lhs, float rhs)
	{
		return ColorRGB{ lhs.r * rhs, lhs.g * rhs, lhs.b * rhs };
	}

	inline ColorRGB operator*(const ColorRGB& lhs, const ColorRGB& rhs)
	{
		return ColorRGB{ lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b };
	}

	inline ColorRGB operator/(const ColorRGB& lhs, float rhs)
	{
		return ColorRGB{ lhs.r / rhs, lhs.g / rhs, lhs.b / rhs };
	}

	inline ColorRGB operator+(const ColorRGB& lhs, const ColorRGB& rhs)
	{
		return ColorRGB{ lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b };
	}

	inline ColorRGB operator-(const ColorRGB& lhs, const ColorRGB& rhs)
	{
		return ColorRGB{ lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b };
	}

	namespace colors
	{
		static ColorRGB Red{ 1.0f,0.0f,0.0f };
		static ColorRGB Blue{ 0.0f,0.0f,1.0f };
		static ColorRGB Green{ 0.0f,1.0f,0.0f };
		static ColorRGB Yellow{ 1.0f,1.0f,0.0f };
		static ColorRGB Cyan{ 0.0f,1.0f,1.0f };
		static ColorRGB Magenta{ 1.0f,0.0f,1.0f };
		static ColorRGB White{ 1.0f,1.0f,1.0f };
		static ColorRGB Black{ 0.0f,0.0f,0.0f };
		static ColorRGB Gray{ 0.5f,0.5f,0.5f };
	}
}