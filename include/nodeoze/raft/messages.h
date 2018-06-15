#ifndef NODEOZE_RAFT_MESSAGES_H
#define NODEOZE_RAFT_MESSAGES_H

#include <nodeoze/raft/types.h>

namespace nodeoze
{
namespace raft
{

class message : BSTRM_BASE( message )
{
public:
	BSTRM_FRIEND_BASE( message )
	BSTRM_CTOR( message, , ( m_src, m_dest, m_term ) )
	BSTRM_ITEM_COUNT( , ( m_src, m_dest, m_term ) )

	virtual bstream::obstream&
	serialize(nodeoze::bstream::obstream& os) const
	{
		base_type::_serialize( os );
		os << m_src << m_dest << m_term;
		return os;
	}

    message( replicant_id_type src, replicant_id_type dest, term_type term )
    :
    m_src{ src },
    m_dest{ dest },
    m_term{ term }
    {}

    virtual ~message() {}

    inline replicant_id_type
    src() const noexcept
    {
        return m_src;
    }
    
    inline replicant_id_type
    dest() const noexcept
    {
        return m_dest;
    }
    
    inline term_type
    term() const noexcept
    {
        return m_term;
    }
    
protected:
    replicant_id_type m_src;
    replicant_id_type m_dest;
    term_type m_term;

};

class request_vote_message : BSTRM_BASE( request_vote_message ), public message
{
public:
	BSTRM_FRIEND_BASE( request_vote_message )
	BSTRM_CTOR( request_vote_message, ( message ) , ( m_last_log_term, m_last_log_index ) )
	BSTRM_ITEM_COUNT( ( message ) , ( m_last_log_term, m_last_log_index ) )

    request_vote_message( replicant_id_type src, replicant_id_type dest, term_type term, term_type last_log_term, index_type last_log_index )
    :
    message( src, dest, term ),
    m_last_log_term{ last_log_term },
    m_last_log_index{ last_log_index }
    {}

protected:
    term_type m_last_log_term;
    index_type m_last_log_index;
};

} // namespace raft
} // namespace nodeoze

