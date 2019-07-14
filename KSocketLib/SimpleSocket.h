#pragma once
#include "winsock2.h"
#include "ws2tcpip.h"
#include "string"
using namespace std;
#pragma comment(lib, "Ws2_32.lib")

namespace KSocketLib {
	string FormatString(const char *lpcszFormat, ...)
	{
		char *pszStr = NULL;
		if (NULL != lpcszFormat)
		{
			va_list marker = NULL;
			va_start(marker, lpcszFormat); //初始化变量参数
			size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度
			pszStr = new char[nLength];
			memset(pszStr, '\0', nLength);
			_vsnprintf_s(pszStr, nLength, nLength, lpcszFormat, marker);
			va_end(marker); //重置变量参数
		}
		string strResult(pszStr);
		delete[]pszStr;
		return strResult;
	}
	template <class T>
	struct ActionResult
	{
		bool Success;
		long ErrorCode;
		string ErrorMsg;
		T refDataObject;
		ActionResult<T>(bool s,long code,string msg): Success(s), ErrorCode(code),ErrorMsg(msg)
		{

		}
	};
	using  ActionResultS = ActionResult<char>;
	ActionResult<WSAData> InitEngine()
	{
		ActionResult<WSAData> r(true,0,"");
		int code = WSAStartup(MAKEWORD(2, 2), &r.refDataObject);
		r.ErrorCode = code;
		r.Success = code == 0;
		r.ErrorMsg = r.Success?"":FormatString("WAS start up failed! Error code:%d", code);
		return r;
	}
	ActionResultS StopEngine()
	{
		int ret = WSACleanup();
		bool success= ret != SOCKET_ERROR;
		long code = success ? WSAGetLastError() : 0;
		ActionResultS result(success, code, success ? "" : FormatString("WAS clean up failed! Error code:%ld", code));
		return result;
	}
	enum class AFFamily
	{
		UNSPEC    =  0  ,             // unspecified
		UNIX      =  1  ,             // local to host (pipes, portals)
		INET      =  2  ,             // internetwork: UDP, TCP, etc.
		IMPLINK   =  3  ,             // arpanet imp addresses
		PUP       =  4  ,             // pup protocols: e.g. BSP
		CHAOS     =  5  ,             // mit CHAOS protocols
		NS        =  6  ,             // XEROX NS protocols
		IPX       =  6	,			  // IPX protocols: IPX, SPX, etc.
		ISO       =  7  ,             // ISO protocols
		OSI       =  7	,			  // OSI is ISO
		ECMA      =  8  ,             // european computer manufacturers
		DATAKIT   =  9  ,             // datakit protocols
		CCITT     =  10 ,             // CCITT protocols, X.25 etc
		SNA       =  11 ,             // IBM SNA
		DECnet    =  12 ,             // DECnet
		DLI       =  13 ,             // Direct data link interface
		LAT       =  14 ,             // LAT
		HYLINK    =  15 ,             // NSC Hyperchannel
		APPLETALK =  16 ,             // AppleTalk
		NETBIOS   =  17 ,             // NetBios-style addresses
		VOICEVIEW =  18 ,             // VoiceView
		FIREFOX   =  19 ,             // Protocols from Firefox
		UNKNOWN1  =  20 ,             // Somebody is using this!
		BAN       =  21 ,             // Banyan
		ATM       =  22 ,             // Native ATM Services
		INET6     =  23 ,             // Internetwork Version 6
		CLUSTER   =  24 ,             // Microsoft Wolfpack
		IEEE12844 =  25 ,             // IEEE 1284.4 WG AF
		IRDA      =  26 ,             // IrDA
		NETDES    =  28 ,             // Network Designers OSI & gateway
	};
	enum class Protocol
	{
#if(_WIN32_WINNT >= 0x0501)
		HOPOPTS = 0,  // IPv6 Hop-by-Hop options
#endif//(_WIN32_WINNT >= 0x0501)
		ICMP = 1,
		IGMP = 2,
		GGP = 3,
#if(_WIN32_WINNT >= 0x0501)
		IPV4 = 4,
#endif//(_WIN32_WINNT >= 0x0501)
#if(_WIN32_WINNT >= 0x0600)
		ST = 5,
#endif//(_WIN32_WINNT >= 0x0600)
		TCP = 6,
#if(_WIN32_WINNT >= 0x0600)
		CBT = 7,
		EGP = 8,
		IGP = 9,
#endif//(_WIN32_WINNT >= 0x0600)
		PUP = 12,
		UDP = 17,
		IDP = 22,
#if(_WIN32_WINNT >= 0x0600)
		RDP = 27,
#endif//(_WIN32_WINNT >= 0x0600)

#if(_WIN32_WINNT >= 0x0501)
		IPV6 = 41, // IPv6 header
		ROUTING = 43, // IPv6 Routing header
		FRAGMENT = 44, // IPv6 fragmentation header
		ESP = 50, // encapsulating security payload
		AH = 51, // authentication header
		ICMPV6 = 58, // ICMPv6
		NONE = 59, // IPv6 no next header
		DSTOPTS = 60, // IPv6 Destination options
#endif//(_WIN32_WINNT >= 0x0501)

		ND = 77,
#if(_WIN32_WINNT >= 0x0501)
		ICLFXBM = 78,
#endif//(_WIN32_WINNT >= 0x0501)
#if(_WIN32_WINNT >= 0x0600)
		PIM = 103,
		PGM = 113,
		L2TP = 115,
		SCTP = 132,
#endif//(_WIN32_WINNT >= 0x0600)
		RAW = 255,

		MAX = 256,
		//
		//  These are reserved for internal use by Windows.
		//
		RESERVED_RAW = 257,
		RESERVED_IPSEC = 258,
		RESERVED_IPSECOFFLOAD = 259,
		RESERVED_WNV = 260,
		RESERVED_MAX = 261
	};
	enum class SocketType
	{
		STREAM    = 1,
		DGRAM     = 2,
		RAW       = 3,
		RDM       = 4,
		SEQPACKET = 5,
	};

};
using namespace KSocketLib;
class SimpleSocket
{
public:
	SimpleSocket();
	~SimpleSocket();
private:
	SOCKET sock = INVALID_SOCKET;
	static ActionResult<SOCKET> createSocket(AFFamily family = AFFamily::INET, SocketType sockType= SocketType::STREAM, Protocol protocol =Protocol::TCP);
};

