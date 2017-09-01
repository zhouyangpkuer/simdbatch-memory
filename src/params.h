#ifndef _PARAMS_H
#define _PARAMS_H



#define MAX_INSERT_PACKAGE 2000000
#define MAX_HASH_NUM 10

typedef long long lint;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef unsigned short usi;


// #define CMS
// #define FR
// #define SS
#define FM
// #define UNIV
// #define REV
// #define MRAC_DIS
// #define LCNT
// #define TWO // Must be SIP_DIP_PKT


// #define LRU
#define GRR


// #define SIP_PKT;
// #define DIP_PKT;
#define SIP_DIP_PKT;
// #define FOUR_TUPLE_PKT;
// #define FIVE_TUPLE_PKT;




#ifdef SIP_PKT
typedef struct _pkt
{
	uint sip;
	bool operator < (const _pkt& other) const
    {
        return sip < other.sip;
    }
	bool operator == (const _pkt& other) const
    {
        return sip == other.sip;
    }
}pkt;
namespace std 
{
	template <>
	struct hash<_pkt>
	{
    	std::size_t operator()(const _pkt& key) const
    	{
      		using std::size_t;
      		using std::hash;

      		return (hash<uint>()(key.sip));
    	}
  	};
}
#endif

#ifdef DIP_PKT
typedef struct _pkt
{
	uint dip;
	bool operator < (const _pkt& other) const
    {
        return dip < other.dip;
    }
	bool operator == (const _pkt& other) const
    {
        return dip == other.dip;
    }
}pkt;
namespace std 
{
	template <>
	struct hash<_pkt>
	{
    	std::size_t operator()(const _pkt& key) const
    	{
      		using std::size_t;
      		using std::hash;

      		return (hash<uint>()(key.dip));
    	}
  	};
}
#endif

#ifdef SIP_DIP_PKT
typedef struct _pkt
{
	uint sip;
	uint dip;
	bool operator < (const _pkt& other) const
    {
    	if(sip < other.sip) 
    		return true;
    	else if(sip == other.sip)
    	{
    		if(dip < other.dip)
    			return true;
    		else
    			return false;
    	}
    	else
    		return false;
    }
    bool operator == (const _pkt& other) const
    {
    	return sip == other.sip && dip == other.dip;
    }
}pkt;
namespace std 
{
	template <>
	struct hash<_pkt>
	{
    	std::size_t operator()(const _pkt& key) const
    	{
      		using std::size_t;
      		using std::hash;

      		return (hash<uint>()(key.sip) ^ hash<uint>()(key.dip));
    	}
  	};
}
#endif

#ifdef FOUR_TUPLE_PKT
typedef struct _pkt
{
	uint sip;
	uint dip;
	usi sport;
	usi dport;
	bool operator < (const _pkt& other) const
    {
    	if(sip < other.sip) 
    		return true;
    	else if(sip == other.sip)
    	{
    		if(dip < other.dip)
    			return true;
    		else if(dip == other.dip)
    		{
    			if(sport < other.sport)
    				return true;
    			else if(sport == other.sport)
    			{
    				if(dport < other.dport)
    					return true;
    				else
    					return false;
    			}
    			else
    				return false;
    		}
    		else
    			return false;
    	}
    	else
    		return false;
    }
    bool operator == (const _pkt& other) const
    {
    	return sip == other.sip && dip == other.dip && sport == other.sport && dport == other.dport;
    }
}pkt;
namespace std 
{
	template <>
	struct hash<_pkt>
	{
    	std::size_t operator()(const _pkt& key) const
    	{
      		using std::size_t;
      		using std::hash;

      		return (hash<uint>()(key.sip) ^ hash<uint>()(key.dip) ^ hash<usi>()(key.sport) ^ hash<usi>()(key.dport));
    	}
  	};
}
#endif


#ifdef FIVE_TUPLE_PKT
typedef struct _pkt
{
	uint sip;
	uint dip;
	usi sport;
	usi dport;
	usi type;
	bool operator < (const _pkt& other) const
    {
    	if(sip < other.sip) 
    		return true;
    	else if(sip == other.sip)
    	{
    		if(dip < other.dip)
    			return true;
    		else if(dip == other.dip)
    		{
    			if(sport < other.sport)
    				return true;
    			else if(sport == other.sport)
    			{
    				if(dport < other.dport)
    					return true;
    				else if(dport == other.dport)
    				{
    					if(type < other.type)
    						return true;
    					else
    						return false;
    				}
    				else
    					return false;
    			}
    			else
    				return false;
    		}
    		else
    			return false;
    	}
    	else
    		return false;
    }
    bool operator == (const _pkt& other) const
    {
    	return sip == other.sip && dip == other.dip && sport == other.sport && dport == other.dport && type == other.type;
    }
}pkt;

namespace std 
{
	template <>
	struct hash<_pkt>
	{
    	std::size_t operator()(const _pkt& key) const
    	{
      		using std::size_t;
      		using std::hash;

      		return (hash<uint>()(key.sip) ^ hash<uint>()(key.dip) ^ hash<usi>()(key.sport) ^ hash<usi>()(key.dport) ^ hash<usi>()(key.type));
    	}
  	};
}
#endif


#define PKT_LEN (sizeof(pkt))





#endif //_PARAMS_H