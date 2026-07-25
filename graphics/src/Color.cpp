#include <graphics/Color.hpp>

#include <algorithm>

using namespace rast;

// format is 0xAABBGGRR
const Color Color::Black{0xff000000};
const Color Color::White{0xffffffff};
const Color Color::Red{0xff0000ff};
const Color Color::Green{0xff008000};
const Color Color::Blue{0xffff0000};
const Color Color::Yellow{0xff00ffff};
const Color Color::Cyan{0xffffff00};
const Color Color::Magenta{0xffff00ff};

Color Color::operator+(const Color& rhs) const noexcept
{
	return {
		static_cast<uint8_t>(std::min(channels.r + rhs.channels.r, 255)),
		static_cast<uint8_t>(std::min(channels.g + rhs.channels.g, 255)),
		static_cast<uint8_t>(std::min(channels.b + rhs.channels.b, 255)),
		static_cast<uint8_t>(std::min(channels.a + rhs.channels.a, 255)),
	};
}
Color& Color::operator+=(const Color& rhs) noexcept
{
	*this = *this + rhs;
	return *this;
}

Color Color::operator-(const Color& rhs) const noexcept
{
	return {
		static_cast<uint8_t>(std::max(channels.r - rhs.channels.r, 0)),
		static_cast<uint8_t>(std::max(channels.g - rhs.channels.g, 0)),
		static_cast<uint8_t>(std::max(channels.b - rhs.channels.b, 0)),
		static_cast<uint8_t>(std::max(channels.a - rhs.channels.a, 0)),
	};
}
Color& Color::operator-=(const Color& rhs) noexcept
{
	*this = *this - rhs;
	return *this;
}

Color Color::operator*(const Color& rhs) const noexcept
{
	return {
		static_cast<uint8_t>(channels.r * rhs.channels.r / 255),
		static_cast<uint8_t>(channels.g * rhs.channels.g / 255),
		static_cast<uint8_t>(channels.b * rhs.channels.b / 255),
		static_cast<uint8_t>(channels.a * rhs.channels.a / 255),
	};
}
Color& Color::operator*=(const Color& rhs) noexcept
{
	*this = *this * rhs;
	return *this;
}

Color Color::operator*(float rhs) const noexcept
{
	return {
		static_cast<uint8_t>(std::clamp(channels.r * rhs, 0.0f, 255.0f)),
		static_cast<uint8_t>(std::clamp(channels.g * rhs, 0.0f, 255.0f)),
		static_cast<uint8_t>(std::clamp(channels.b * rhs, 0.0f, 255.0f)),
		static_cast<uint8_t>(std::clamp(channels.a * rhs, 0.0f, 255.0f)),
	};
}
Color& Color::operator*=(float rhs) noexcept
{
	*this = *this * rhs;
	return *this;
}

