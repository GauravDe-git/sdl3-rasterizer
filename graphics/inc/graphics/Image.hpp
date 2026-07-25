#pragma once

#include <graphics/Color.hpp>    
#include <graphics/BlendMode.hpp>
#include "aligned_unique_ptr.hpp"

#include <math/AABB.hpp>

#include <filesystem>
#include <optional>
#include <utility>

namespace rast
{
	inline namespace graphics
	{
		struct Image
		{
			Image();
			~Image();
			Image(const Image& copy);
			Image(Image&& other) noexcept;

			explicit Image(const std::filesystem::path& fileName);
			Image(uint32_t width, uint32_t height, std::optional<Color> color = {});

			Image& operator=(const Image& copy);
			Image& operator=(Image&& other) noexcept;

			const Color& operator[](size_t i) const
			{
				assert(std::cmp_less(i, m_Width * m_Height));
				return m_Pixels[i];
			}

			Color& operator[](size_t i)
			{
				assert(std::cmp_less(i, m_Width * m_Height));
				return m_Pixels[i];
			}

			/// Note: C++23 would allow multiple params in operator[]

			const Color& operator()(size_t x, size_t y) const
			{
				assert(std::cmp_less(x, m_Width));
				assert(std::cmp_less(y, m_Height));
				return m_Pixels[y * m_Width + x];
			}

			Color& operator()(size_t x, size_t y)
			{
				assert(std::cmp_less(x, m_Width));
				assert(std::cmp_less(y, m_Height));
				return m_Pixels[y * m_Width + x];
			}

			explicit operator bool() const noexcept
			{
				return m_Pixels != nullptr;
			}

			int getWidth() const noexcept
			{
				return m_Width;
			}

			int getHeight() const noexcept
			{
				return m_Height;
			}

			int getPitch() const noexcept
			{
				return m_Width * static_cast<int>(sizeof(Color));
			}

			const AABB& getAABB() const noexcept
			{
				return m_AABB;
			}

			Color* data() noexcept
			{
				return m_Pixels.get();
			}

			const Color* data() const noexcept
			{
				return m_Pixels.get();
			}

			template <bool BoundsCheck = true, bool Blending = true>
			void plot(uint32_t x, uint32_t y, const Color& src, const BlendMode& blendMode = BlendMode{}) noexcept
			{
				if constexpr (BoundsCheck)
				{
					if (std::cmp_greater_equal(x, m_Width) || std::cmp_greater_equal(y, m_Height))
						return;
				}
				else
				{
					assert(std::cmp_less(x, m_Width));
					assert(std::cmp_less(y, m_Height));
				}

				Color& dst = m_Pixels[y * m_Width + x];
				if constexpr (Blending)
				{
					dst = blendMode.Blend(src, dst);
				}
				else
				{
					dst = src;
				}
			}

			void resize(uint32_t width, uint32_t height);

			void save(const std::filesystem::path& file) const;

			void clear(const Color& color) noexcept;

		  private:
			AABB						m_AABB;
			int							m_Width	 = 0;
			int							m_Height = 0;
			aligned_unique_ptr<Color[]> m_Pixels;
		};
	} // namespace graphics

} // namespace rast