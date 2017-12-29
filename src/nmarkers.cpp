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

#include <nodeoze/nmarkers.h>

using namespace nodeoze;

const log::marker &marker::root						= log::get_marker( "nodeoze", false );
const log::marker &marker::database					= log::get_marker( marker::root, "database", false );
const log::marker &marker::connection				= log::get_marker( marker::root, "connection", false );
const log::marker &marker::http						= log::get_marker( marker::connection, "http", false );
const log::marker &marker::rpc						= log::get_marker( marker::connection, "rpc", false );
const log::marker &marker::rpc_performance			= log::get_marker( marker::rpc, "performance", false );
const log::marker &marker::json						= log::get_marker( marker::rpc, "json", false );
const log::marker &marker::msgpack					= log::get_marker( marker::rpc, "msgpack", false );
const log::marker &marker::process					= log::get_marker( marker::root, "process", false );
const log::marker &marker::shell					= log::get_marker( marker::root, "shell", false );
const log::marker &marker::socket					= log::get_marker( marker::root, "socket", false );
const log::marker &marker::socket_filter			= log::get_marker( marker::socket, "filter", false );
const log::marker &marker::socket_ip				= log::get_marker( marker::socket, "ip", false );
const log::marker &marker::socket_tcp				= log::get_marker( marker::socket_ip, "tcp", false );
const log::marker &marker::socket_tcp_accept		= log::get_marker( marker::socket_tcp, "accept", false );
const log::marker &marker::socket_tcp_connect		= log::get_marker( marker::socket_tcp, "connect", false );
const log::marker &marker::socket_tcp_send			= log::get_marker( marker::socket_tcp, "send", false );
const log::marker &marker::socket_tcp_recv			= log::get_marker( marker::socket_tcp, "recv", false );
const log::marker &marker::socket_tcp_close			= log::get_marker( marker::socket_tcp, "close", false );
const log::marker &marker::socket_udp				= log::get_marker( marker::socket_ip, "udp", false );
const log::marker &marker::socket_udp_bind			= log::get_marker( marker::socket_udp, "bind", false );
const log::marker &marker::socket_udp_send			= log::get_marker( marker::socket_udp, "send", false );
const log::marker &marker::socket_udp_recv			= log::get_marker( marker::socket_udp, "recv", false );
const log::marker &marker::socket_udp_close			= log::get_marker( marker::socket_udp, "close", false );
const log::marker &marker::websocket				= log::get_marker( marker::root, "websocket", false );
const log::marker &marker::proxy					= log::get_marker( marker::root, "proxy", false );
const log::marker &marker::proxy_http				= log::get_marker( marker::proxy, "http", false );
const log::marker &marker::proxy_socks				= log::get_marker( marker::proxy, "socks", false );
const log::marker &marker::async_iter				= log::get_marker( marker::root, "async_iter", false );
const log::marker &marker::machine					= log::get_marker( marker::root, "machine", false );
const log::marker &marker::buffer					= log::get_marker( marker::root, "buffer", false );
const log::marker &marker::logclk					= log::get_marker( marker::root, "logclk", false );
const log::marker &marker::dns_sd					= log::get_marker( marker::root, "dns_sd", false );
const log::marker &marker::arp						= log::get_marker( marker::root, "arp", false );
const log::marker &marker::state_machine			= log::get_marker( marker::root, "state_machine", false );


