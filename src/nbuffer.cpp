/*
 * Copyright (c) 2013-2017, Collobos Software Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

 #include <nodeoze/nbuffer.h>
 #include <nodeoze/nmacros.h>


 nodeoze::buffer::deleter_f nodeoze::buffer::do_not_delete_data = []( std::uint8_t *data )
 {
	nunused( data );
 };

void
nodeoze::buffer::dump( std::ostream& os ) const
{
	static const int bytes_per_line = 16;

	std::ios sstate(nullptr);
	sstate.copyfmt( os );
	
	os << std::endl;
	auto remaining = size();
	auto line_index = 0;
	while ( remaining > 0 )
	{
		
		if ( remaining >= bytes_per_line )
		{
			os << std::hex << std::setfill( '0' );
			os << std::setw(8) << line_index << ": ";
			for ( auto i = 0; i < bytes_per_line; ++i )
			{
				auto byte = m_data[ line_index + i ];
				os << std::setw(2) << (unsigned) byte << ' ';
			}
			os << "    ";
			os.copyfmt( sstate );
			for ( auto i = 0; i < bytes_per_line; ++i )
			{
				auto byte = m_data[ line_index + i ];
				if ( isprint( byte ) )
				{
					os << (char)byte;
				}
				else
				{
					os << '.';
				}
			}
			line_index += bytes_per_line;
			remaining -= bytes_per_line;
			
			os << std::endl;
		}
		else
		{
			os << std::hex << std::setfill( '0' );
			os << std::setw(8) << line_index << ": ";
			for ( auto i = 0u; i < remaining; ++i )
			{
				auto byte = m_data[ line_index + i ];
				os << std::setw(2) << (unsigned)byte << ' ';
			}
			for ( auto i = 0u; i < bytes_per_line - remaining; ++i )
			{
				os << "   ";
			}
			os << "    ";
			os.copyfmt( sstate );
			for ( auto i = 0u; i < remaining; ++i )
			{
				auto byte = m_data[ line_index + i ];
				if ( isprint( byte ) )
				{
					os << (char) byte;
				}
				else
				{
					os << '.';
				}
			}
			os << std::endl;
			remaining = 0;
		}
	
	}
	os.copyfmt( sstate ); // just to make sure
}
