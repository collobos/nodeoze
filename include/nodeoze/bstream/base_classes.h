/*
 * The MIT License
 *
 * Copyright 2017 David Curtis.
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
 * File:   base_classes.h
 * Author: David Curtis
 *
 * Created on June 30, 2017, 7:58 PM
 */

#ifndef BSTREAM_BASE_CLASSES_H
#define BSTREAM_BASE_CLASSES_H

#include <nodeoze/bstream/ibstream.h>
#include <nodeoze/bstream/obstream.h>
#include <sstream>

namespace nodeoze
{
namespace bstream
{
	
	template<class Derived>
	class map_base
	{
	public:
		inline map_base(bstream::ibstream& is)
		{
			auto length = is.read_map_header();
            if (length != item_count())
            {
				std::ostringstream msg;
                msg << "invalid element count in map streaming base class, expected " 
					<< std::to_string(item_count()) << ", found " << std::to_string(length);
                throw type_error(msg.str());
            }
		}

        inline map_base() {};
            
        inline obstream& 
        serialize(obstream& os) const
        {
            return os.write_map_header(item_count());
        }
        
        inline ibstream&
        deserialize(ibstream& is)
        {
            auto length = is.read_map_header();
            if (length != item_count())
            {
                std::ostringstream msg;
				msg << "invalid element count in map streaming base class, expected " 
					<< std::to_string(item_count()) << ", found " << std::to_string(length);
                throw type_error(msg.str());
            }
            return is;
        }
        
        inline std::size_t
        item_count() const
        {
            return static_cast<const Derived&>(*this)._streamed_item_count();
        }
	};
    
    template<class Derived>
    class array_base
    {
    public:
        inline array_base(bstream::ibstream& is)
        {			
            auto length = is.read_array_header();
            if (length != item_count())
            {
				std::ostringstream msg;
                msg << "invalid element count in array streaming base class, expected " 
					<< std::to_string(item_count()) << ", found " << std::to_string(length);
                throw type_error(msg.str());
            }
        }

        inline array_base() {};
            
        inline obstream& 
        serialize(obstream& os) const
        {
            return os.write_array_header(item_count());
        }
        
        inline ibstream&
        deserialize(ibstream& is)
        {
            auto length = is.read_array_header();
            if (length != item_count())
            {
				std::ostringstream msg;
                msg << "invalid element count in array streaming base class, expected " 
					<< std::to_string(item_count()) << ", found " << std::to_string(length);
                throw type_error(msg.str());
            }
            return is;
        }
        
        inline std::size_t
        item_count() const
        {
            return static_cast<const Derived&>(*this)._streamed_item_count();
        }
    };

} // namespace bstream
} // namespace nodeoze

#endif /* BSTREAM_BASE_CLASSES_H */