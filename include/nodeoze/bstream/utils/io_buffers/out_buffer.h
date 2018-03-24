/*
 * The MIT License
 *
 * Copyright 2018 David Curtis.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* 
 * File:   out_buffer.h
 * Author: David Curtis
 *
 * Created on January 7, 2018, 11:22 AM
 */

#ifndef UTILS_OUT_BUFFER_H
#define UTILS_OUT_BUFFER_H

#include <type_traits>
#include <nodeoze/bstream/utils/io_buffers/memory_block.h>
#include <nodeoze/bstream/utils/io_buffers/dump.h>
#include <sstream>
#include <boost/endian/conversion.hpp>
#include <assert.h>

#define TRACE_IOBUFS 0

namespace nodeoze
{
namespace bstream
{
namespace utils
{
	class in_buffer;

	class out_buffer
	{
	public:

		inline out_buffer() 
		: m_block{nullptr}, m_data{nullptr}, m_capacity{0ul}, m_pos{0ul} 
		{}

		out_buffer(memory_block::ptr&& block) 
		: m_block{std::move(block)}, m_data{m_block->data()}, m_capacity{m_block->capacity()}, m_pos{m_block->size()}
		{}

		out_buffer(out_buffer&& other)
		: out_buffer{other.release()}
		{}
		
		out_buffer(in_buffer&& ibuf)
		{
			hijack(std::move(ibuf));
		}
		
		out_buffer(out_buffer const&) = delete;

		virtual ~out_buffer() {}
	
		inline void capture(memory_block::ptr&& block)
		{
			m_block = std::move(block);
			m_data = m_block->data();
			m_capacity = m_block->capacity();
			m_pos = m_block->size();
		}

		inline memory_block::ptr release()
		{
			m_block->set_size(m_pos);
			m_data = nullptr;
			m_capacity = 0ul;
			m_pos = 0ul;
			auto p = std::move(m_block);
			m_block = nullptr;
			return p;
		}
		
		std::uint8_t* block_data()
		{
			return m_block->data();
		}
		
		std::size_t block_capacity()
		{
			return m_block->capacity();
		}

		std::size_t block_size()
		{
			return m_block->size();
		}
		
		void* block_addr()
		{
			return m_block.get();
		}
	
		void hijack(in_buffer&& ibuf);

		template<class T>
		out_buffer(T* blkp, typename std::enable_if_t<std::is_base_of<memory_block, T>::value,int> = 0)
		: out_buffer{std::unique_ptr<T>(blkp)} {}
		
		template<class T, class... Args>
		typename std::enable_if_t<std::is_base_of<memory_block, T>::value>
		use(Args&&... args)
		{
			capture(std::make_unique<T>(std::forward<Args>(args)...));
		}

		virtual out_buffer& clear() noexcept
		{
			m_pos = 0ul;
			return *this;
		}

		inline std::size_t remaining() const noexcept
		{
#if TRACE_IOBUFS
			std::cout << "in remaining(), m_capacity is " << m_capacity << ", m_pos is " << m_pos << std::endl;
#endif
			std::size_t result = 0;
			if (m_capacity > 0)
			{
				assert(m_pos <= m_capacity);
				result = m_capacity - m_pos;
			}
			return result;
		}

		inline bool has_remaining(std::size_t n = 1) const noexcept
		{
			return remaining() >= n ;
		}

		inline out_buffer& put(std::uint8_t byte)
		{
#if TRACE_IOBUFS
			std::cout << "in out_buffer::put, byte is " 
					<< static_cast<int>(byte) << ", m_data is " 
					<< (void*) m_data << ", m_pos is " << m_pos << std::endl;
#endif
			accommodate_put(1);
#if TRACE_IOBUFS
			std::cout << "in out_buffer::put, after accommodate_put(1)" << std::endl;
#endif
			m_data[m_pos++] = byte;
#if TRACE_IOBUFS
			std::cout << "in out_buffer::put, after assignent to vector element" << std::endl;
#endif
			return *this;                
		}

		inline out_buffer& put(const out_buffer& source)
		{
			std::size_t nbytes = source.size();
			return put(source.m_data, nbytes);
		}

		inline out_buffer& put(const void* src, std::size_t nbytes)
		{
			accommodate_put(nbytes);
			void* dst = m_data + m_pos;
			std::memcpy(dst, src, nbytes);
			m_pos += nbytes;
			return *this;                
		}

		template<class U>
		inline typename std::enable_if<std::is_arithmetic<U>::value && sizeof(U) == 1, out_buffer&>::type 
		put_arithmetic(U value)
		{
			accommodate_put(sizeof(U));
			m_data[m_pos] = reinterpret_cast<std::uint8_t const&>(value);
			++m_pos;
			return *this;
		}
		
		template<class U>
		inline typename std::enable_if<std::is_arithmetic<U>::value && sizeof(U) == 2, out_buffer&>::type 
		put_arithmetic(U value, boost::endian::order buffer_order = boost::endian::order::big)
		{
			static const bool reverse_order = boost::endian::order::native != buffer_order;
			constexpr std::size_t usize = sizeof(U);
			using canonical_type = std::uint16_t;
			accommodate_put(sizeof(U));

			union
			{
				std::uint8_t bytes[usize];
				canonical_type canonical_value;
			} val;

			val.canonical_value = reinterpret_cast<canonical_type const&>(value);
			if (reverse_order)
			{
				m_data[m_pos] = val.bytes[1];
				m_data[m_pos+1] = val.bytes[0];
			}
			else
			{
				m_data[m_pos] = val.bytes[0];
				m_data[m_pos+1] = val.bytes[1];
			}
			m_pos += usize;
			return *this;
		}
		
		template<class U>
		inline typename std::enable_if<std::is_arithmetic<U>::value && sizeof(U) == 4, out_buffer&>::type 
		put_arithmetic(U value, boost::endian::order buffer_order = boost::endian::order::big)
		{
			static const bool reverse_order = boost::endian::order::native != buffer_order;
			constexpr std::size_t usize = sizeof(U);
			using canonical_type = std::uint32_t;
			accommodate_put(sizeof(U));

			union
			{
				std::uint8_t bytes[usize];
				canonical_type canonical_value;
			} val;

			val.canonical_value = reinterpret_cast<canonical_type const&>(value);
			if (reverse_order)
			{
				m_data[m_pos] = val.bytes[3];
				m_data[m_pos+1] = val.bytes[2];
				m_data[m_pos+2] = val.bytes[1];
				m_data[m_pos+3] = val.bytes[0];
			}
			else
			{
				m_data[m_pos] = val.bytes[0];
				m_data[m_pos+1] = val.bytes[1];
				m_data[m_pos+2] = val.bytes[2];
				m_data[m_pos+3] = val.bytes[3];
			}
			m_pos += usize;
			return *this;
		}
		
		template<class U>
		inline typename std::enable_if<std::is_arithmetic<U>::value && sizeof(U) == 8, out_buffer&>::type 
		put_arithmetic(U value, boost::endian::order buffer_order = boost::endian::order::big)
		{
			static const bool reverse_order = boost::endian::order::native != buffer_order;
			constexpr std::size_t usize = sizeof(U);
			using canonical_type = std::uint64_t;
			accommodate_put(sizeof(U));

			union
			{
				std::uint8_t bytes[usize];
				canonical_type canonical_value;
			} val;

			val.canonical_value = reinterpret_cast<canonical_type const&>(value);
			if (reverse_order)
			{
				m_data[m_pos] = val.bytes[7];
				m_data[m_pos+1] = val.bytes[6];
				m_data[m_pos+2] = val.bytes[5];
				m_data[m_pos+3] = val.bytes[4];
				m_data[m_pos+4] = val.bytes[3];
				m_data[m_pos+5] = val.bytes[2];
				m_data[m_pos+6] = val.bytes[1];
				m_data[m_pos+7] = val.bytes[0];
			}
			else
			{
				m_data[m_pos] = val.bytes[0];
				m_data[m_pos+1] = val.bytes[1];
				m_data[m_pos+2] = val.bytes[2];
				m_data[m_pos+3] = val.bytes[3];
				m_data[m_pos+4] = val.bytes[4];
				m_data[m_pos+5] = val.bytes[5];
				m_data[m_pos+6] = val.bytes[6];
				m_data[m_pos+7] = val.bytes[7];
			}
			m_pos += usize;
			return *this;
		}
		
		// added to make this buffer directly usable by msgpack::packer

		inline void 
		write(const char* buf, size_t len)
		{
			put(buf, len);
		}

		inline std::size_t
		size() const noexcept
		{
			return m_pos;
		}

		inline const std::uint8_t* 
		data() const noexcept
		{
			return m_data;
		}

		inline std::size_t
		capacity() const noexcept
		{
			return m_capacity;
		}

		inline std::size_t
		position() const noexcept
		{
			return m_pos;
		}

		inline void
		position(std::size_t pos)
		{
			if (pos < m_capacity)
			{
				m_pos = pos;
			}
			else
			{
				throw std::invalid_argument("position out of range for buffer");
			}
		}

		inline void dump( std::ostream& os, std::size_t offset, std::size_t nbytes) const
		{
			utils::dump(os, &(m_data[offset]), nbytes);
		}
		
		inline std::string strdump(std::size_t offset, std::size_t nbytes) const
		{
			std::ostringstream oss;
			dump(oss, offset, nbytes);
			return oss.str();
		}

		inline void dump(std::ostream& os) const
		{
			dump(os, 0, m_pos);
		}
		
		inline std::string strdump() const
		{
			std::ostringstream oss;
			dump(oss);
			return oss.str();
		}

	protected:
		
		inline void
		accommodate_put(std::size_t nbytes)
		{
#if TRACE_IOBUFS
			std::cout << "entered accommodate_put, nbytes is " << nbytes << ", remaining is " << remaining()
					<< ", m_block is " << (void*) m_block.get() << std::endl;
#endif
			if (!has_remaining(nbytes))
			{
				assert(!(!m_block));
				if (!m_block->is_expandable())
				{
					throw std::out_of_range{"overflow on non-expandable buffer"};
				}
#if TRACE_IOBUFS
				std::cout << "expanding block, old m_data is " << (void*) m_data << ", old m_capacity is "
						<< m_capacity << std::endl;
#endif
				m_block->expand(m_pos + nbytes);
				m_data = m_block->data();
				m_capacity = m_block->capacity();
#if TRACE_IOBUFS
				std::cout << "expanded block, new m_data is " << (void*) m_data << ", new m_capacity is "
						<< m_capacity << std::endl;
#endif
			}
		}

		memory_block::ptr m_block;
		std::uint8_t *m_data;
		std::size_t m_capacity;
		std::size_t m_pos;
	};

} // namespace utils
} // namespace bstream
} // namespace nodeoze

#endif /* UTILS_OUT_BUFFER_H */
