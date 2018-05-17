// ClientConnection.h
// copyright 2000 Verant Interactive
// Author: Justin Randall

#ifndef	_ClientConnection_H
#define	_ClientConnection_H

//-----------------------------------------------------------------------

#include "serverUtility/ServerConnection.h"
#include "sharedFoundation/StationId.h"

#include <algorithm>
#include <cctype>
#include <string>

class ClientCommandChannel;
class NetworkId;

//-----------------------------------------------------------------------

class ClientConnection : public ServerConnection
{
public:
	                               ClientConnection(UdpConnectionMT *, TcpClient *);
	                              ~ClientConnection();

	bool                          getIsValidated() const;
	void                          setIsValidated(bool);

	bool                          getIsSecure() const;
	void                          setIsSecure(bool);

	int                           getAdminLevel() const;
	void                          setAdminLevel(int);
	
	const StationId               getStationId() const;
	void                          setStationId(StationId newValue) const;
	StationId                     getRequestedAdminSuid() const;
	
	const uint32                  getGameBits() const;
	const uint32                  getSubscriptionBits() const;
	void                          setGameBits(uint32 gameBits) const;
	void                          setSubscriptionBits(uint32 subscriptionBits) const;

	virtual void                 onConnectionClosed      ();
	virtual void                 onConnectionOpened      ();
	virtual void                 onReceive               (const Archive::ByteStream & message);

	void                          onCharacterDeletedFromLoginDatabase   (const NetworkId & characterId);
	void                          onCharacterDeletedFromCluster         (const NetworkId & characterId);
	
private:
	ClientConnection(const ClientConnection&);
	ClientConnection& operator=( const ClientConnection&);

	void                        validateClient(const std::string & id, const std::string & key);
private:
	int               m_clientId;
	bool              m_isValidated;
	bool              m_isSecure;   // player logged in using a SecureId
	int               m_adminLevel; // admin level of the player in the admin file (-1 if not in the admin file)
	mutable StationId m_stationId;
	StationId         m_requestedAdminSuid;
	mutable uint32    m_gameBits;
	mutable uint32    m_subscriptionBits;

	bool              m_waitingForCharacterLoginDeletion;
	bool              m_waitingForCharacterClusterDeletion;
	std::vector<NetworkId>  m_charactersPendingDeletion;

}; //lint !e1712 // default constructor not defined

//-----------------------------------------------------------------------

// thanks for the nasty BS Sony
inline const StationId hashOldSoeSUID(std::string name) {
        StationId oldSoeSUID = 0;

        if (name.length()) {
                char brokenSuid[50];
                uint32_t hash;

                //if (name.size() > MAX_ACCOUNT_NAME_LENGTH) {
                //      name.resize(MAX_ACCOUNT_NAME_LENGTH);
                //}

                // use SOE's shitty method for generating unique id's
                hash =  static_cast<uint32_t>(std::hash < std::string > {}(name.c_str()));

                // ...running them through sprintf to format, and overflowing or otherwise mangling the value
                sprintf(brokenSuid, "%i", hash);

                // and then go back to int...lol
                oldSoeSUID = std::stoi(brokenSuid);
        }

        return oldSoeSUID;
}
	
//-----------------------------------------------------------------------

inline const StationId ClientConnection::getStationId() const
{
	return m_stationId;
}

// ----------------------------------------------------------------------

inline void ClientConnection::setStationId(StationId newValue) const
{
	m_stationId = newValue;
}

//-----------------------------------------------------------------------

inline bool ClientConnection::getIsValidated() const
{
	return m_isValidated;
}

// ----------------------------------------------------------------------

inline void ClientConnection::setIsValidated(bool newValue) 
{
	m_isValidated = newValue;
}

// ----------------------------------------------------------------------

inline bool ClientConnection::getIsSecure() const
{
	return m_isSecure;
}

// ----------------------------------------------------------------------

inline void ClientConnection::setIsSecure(bool newValue)
{
	m_isSecure = newValue;
}

// ----------------------------------------------------------------------

inline int ClientConnection::getAdminLevel()const
{
	return m_adminLevel;
}

// ----------------------------------------------------------------------

inline void ClientConnection::setAdminLevel(int newValue)
{
	m_adminLevel = newValue;
}

//-----------------------------------------------------------------------

inline const uint32 ClientConnection::getGameBits() const
{
	return m_gameBits;
}

// ----------------------------------------------------------------------

inline void ClientConnection::setGameBits(uint32 gameBits) const
{
	m_gameBits = gameBits;
}

//-----------------------------------------------------------------------

inline const uint32 ClientConnection::getSubscriptionBits() const
{
	return m_subscriptionBits;
}

// ----------------------------------------------------------------------

inline void ClientConnection::setSubscriptionBits(uint32 subscriptionBits) const
{
	m_subscriptionBits = subscriptionBits;
}

//-----------------------------------------------------------------------


#endif	// _ClientConnection_H
