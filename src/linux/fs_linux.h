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

#ifndef _nodeoze_fs_linux_h
#define _nodeoze_fs_linux_h

#include <nodeoze/nfs.h>

namespace nodeoze {

class fs_linux : public fs
{
public:

	fs_linux();
	
	virtual ~fs_linux();

	virtual path
	make_safe( const path &p );
	
	virtual bool
	exists( const path &p );
	
	virtual std::size_t
	size( const path &p );

	virtual std::chrono::system_clock::time_point
	created( const path &p );
	
	virtual std::chrono::system_clock::time_point
	modified( const path &p );
	
	virtual bool
	is_directory( const path &p );
	
	virtual bool
	is_regular_file( const path &p );
	
	virtual bool
	is_executable( const path &p );
	
	virtual std::vector< path >
	list( const path &p );
	
	virtual std::error_code
	mkdir( const path &p );

	virtual std::error_code
	rmdir( const path &p );
	
	virtual std::error_code
	copy( const path &from, const path &to );
	
	virtual std::error_code
	move( const path &from, const path &to );
	
	virtual std::error_code
	unlink( const path &p );

	virtual bool
	lock( marker m );
	
	virtual bool
	unlock( marker m );
};

}

#endif
