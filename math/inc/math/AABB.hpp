#pragma once

#include "Rect.hpp"
#include "Viewport.hpp"

#include <glm/common.hpp> // glm min max
#include <glm/vec2.hpp>

#include <limits>
#include <optional>

namespace rast
{
	inline namespace math
	{
		struct AABB
		{
			AABB() = default;

			AABB(const glm::vec2& a, const glm::vec2& b) noexcept
			{
				min = glm::min(a, b);
				max = glm::max(a, b);
			}

			// Triangle
			AABB(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) noexcept
			{
				min = glm::min(a, glm::min(b, c));
				max = glm::max(a, glm::max(b, c));
			}

			// Quad
			AABB(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d) noexcept
			{
				min = glm::min(glm::min(a, b), glm::min(c, d));
				max = glm::max(glm::min(a, b), glm::max(c, d));
			}

			// From a viewport
			explicit AABB(const math::Viewport& viewport)
			{
				min = glm::vec2{viewport.x, viewport.y};
				max = glm::vec2{viewport.x + viewport.width - 1, viewport.y + viewport.height - 1};
			}

			// To translate the AABB by a vector
			AABB operator+(const glm::vec2& a) const noexcept
			{
				return {min + a, max + a};
			}

			AABB& operator+=(const glm::vec2& a) noexcept
			{
				min += a;
				max += a;
				return *this;
			}

			AABB operator-(const glm::vec2& a) const noexcept
			{
				return {min - a, max - a};
			}

			AABB& operator-=(const glm::vec2& a) noexcept
			{
				min -= a;
				max -= a;
				return *this;
			}

			[[nodiscard]] float width() const noexcept
			{
				return max.x - min.x;
			}

			[[nodiscard]] float height() const noexcept
			{
				return max.y - min.y;
			}

			[[nodiscard]] float area() const noexcept
			{
				return width() * height();	
			}

			[[nodiscard]] glm::vec2 size() const noexcept
			{
				return max - min;
			}

			[[nodiscard]] glm::vec2 extent() const noexcept
			{
				return size() * 0.5f;
			}

			[[nodiscard]] glm::vec2 center() const noexcept
			{
				return (min + max) * 0.5f;
			}

			AABB& expand( const glm::vec2& p) noexcept
			{
				min = glm::min(min, p); 
				max = glm::max(max, p);
				return *this;
			}

			AABB& expand( const AABB& aabb) noexcept
			{
				min = glm::min(min, aabb.min);
				max = glm::max(max, aabb.max);
				return *this;
			}

			AABB& clamp(const AABB& aabb) noexcept
			{
				min = glm::max(min, aabb.min);
				max = glm::min(max, aabb.max);
				return *this;
			}

			// Return true if two AABB intersect
			[[nodiscard]] bool intersect(const AABB& aabb) const noexcept
			{
				return min.x <= aabb.max.x && min.y <= aabb.max.y 
				&& max.x >= aabb.min.x && max.y >= aabb.min.y;
			}

			// Return true if the point p is in the AABB
			[[nodiscard]] bool contains(const glm::vec2& p) const noexcept
			{
				return p.x  >= min.x && p.y >= min.y && p.x <= max.x && p.y <= max.y;
			}

			// overlap for collision resolution
			[[nodiscard]] std::optional<glm::vec2> overlap(const AABB& aabb) const noexcept
			{
				glm::vec2 overlap = glm::min(max, aabb.max) - glm::max(min, aabb.min);

				if (overlap.x > 0.f && overlap.y > 0.f)
				{
					if (overlap.x < overlap.y)
					{
						return glm::vec2 { (center().x < aabb.center().x ?
							max.x - aabb.min.x : min.x - aabb.max.x), 0.f};
					}

					return glm::vec2{0.f, (center().y < aabb.center().y ?
						max.y - aabb.min.y : min.y - aabb.max.y)};
				}

				return {};
			}

			/// Construct an AABB from min & max points.
			static AABB fromMinMax(const glm::vec2& min, const glm::vec2& max)
			{
				AABB aabb;

				aabb.min = min;
				aabb.max = max;

				return aabb;
			}

			/// Construct an AABB from a rectangle.
			template <typename T>
			static AABB fromRect(const Rect<T>& rect)
			{
				return fromMinMax({rect.topLeft(), 0.0f}, {rect.bottomRight(), 0.0f});
			}

			/// Return this AABB clamped to another.
			AABB clamped(const AABB& aabb) const noexcept
			{
				return fromMinMax(glm::max(min, aabb.min), glm::min(max, aabb.max));
			}

		public:
			glm::vec2 min{std::numeric_limits<float>::max()};
			glm::vec2 max{std::numeric_limits<float>::lowest()};
		};
	} // namespace math
} // namespace rast