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

//
//  Created by Matt Gallagher on 2009/06/03.
//  Copyright 2009 Matt Gallagher. All rights reserved.
//
//  This software is provided 'as-is', without any express or implied
//  warranty. In no event will the authors be held liable for any damages
//  arising from the use of this software. Permission is granted to anyone to
//  use this software for any purpose, including commercial applications, and to
//  alter it and redistribute it freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source
//     distribution.
//
 
#include <nodeoze/base64.h>
#include <nodeoze/buffer.h>
#include <stdlib.h>

//
// Definition for "masked-out" areas of the base64_decode_lookup mapping
//
#define xx 65

#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

using namespace nodeoze::codec;

std::string
base64::encode( const nodeoze::buffer &s )
{
	std::string out;
    int val=0;
	int valb=-6;

    for ( auto i = 0u; i < s.size(); i++ )
	{
		auto c = s[ i ];
        val = ( val << 8 ) + c;
        valb += 8;
        while ( valb >= 0 )
		{
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val>>valb)&0x3F] );
            valb -= 6;
        }
    }

    if ( valb > -6 )
	{
		out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val<<8)>>(valb+8))&0x3F]);
	}

    while ( out.size() %4 )
	{
		out.push_back( '=' );
	}

    return out;
}

nodeoze::buffer
base64::decode( const std::string &s )
{
	nodeoze::buffer out{ s.size() };
	std::size_t next = 0;


//    std::vector<int> T(256,-1);
//    for (int i=0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i; 

    int val=0, valb=-8;
    for ( std::uint8_t c : s )
	{
//		if (T[c] == -1) break;
        if (m_decode_table.at(c) == -1) break;
//		val = (val<<6) + T[c];
		val = (val<<6) + m_decode_table.at(c);
        valb += 6;
        if (valb >= 0 )
		{
//			out.push_back(char((val>>valb)&0xFF));
			out.put( next, static_cast<std::uint8_t>( ( val >> valb) & 0xFF ) );
			++next;
            valb-=8;
        }
    }

	out.size( next );
    return out;
}

const std::vector<int> base64::m_decode_table = 
{
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  62,  -1,  -1,  -1,  63, 
 52,  53,  54,  55,  56,  57,  58,  59, 
 60,  61,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,   0,   1,   2,   3,   4,   5,   6, 
  7,   8,   9,  10,  11,  12,  13,  14, 
 15,  16,  17,  18,  19,  20,  21,  22, 
 23,  24,  25,  -1,  -1,  -1,  -1,  -1, 
 -1,  26,  27,  28,  29,  30,  31,  32, 
 33,  34,  35,  36,  37,  38,  39,  40, 
 41,  42,  43,  44,  45,  46,  47,  48, 
 49,  50,  51,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
};