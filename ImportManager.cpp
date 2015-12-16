#include "ImportManager.h"
#include <QtGlobal>
#include "json/json.h"

/**
 * Constructor
 *
 * Will need to duplicate in Exporter and Importer
 */
ImportManager::ImportManager(RsPeers* mPeers)
{
    this->mPeers = mPeers;
}
/**
 * Reads a json file containing retroshare gpg keys and loads them into Retroshare.
 */ 
void ImportManager::importData(const std::string &certFileStr, bool import_groups)
{
    Json::Value root;
    Json::Reader reader;
    if (reader.parse(certFileStr, root))
    {
        importGPGIds(root["gpg_ids"]);
		if(import_groups){
            importGroups(root["groups"]);
        }
    }
}
void ImportManager::importGPGIds(const Json::Value &gpg_ids)
{
    foreach(const Json::Value& gpg_id, gpg_ids)
    {
        importSSLIds(gpg_id["ssl_ids"]);
        RsPgpId gid(gpg_id["gpg_id"].asString());
        mPeers->trustGPGCertificate(gid, gpg_id["trustLvl"].asUInt());
    }
}
void ImportManager::importSSLIds(const Json::Value &ssl_ids)
{
    foreach(const Json::Value& json_ssl_id, ssl_ids)
    {
        importSSLId(json_ssl_id);
    }
}
void ImportManager::importSSLId(const Json::Value &json_ssl_id)
{
    std::string certStr = json_ssl_id["pubkey"].asString();
    uint32_t cert_error_code;

    RsPeerDetails peerDetails;
    if (mPeers->loadDetailsFromStringCert(certStr, peerDetails, cert_error_code)) {
        RsPeerId ssl_id;
        RsPgpId pgp_id;
        std::string errorStr;
        if(!mPeers->loadCertificateFromString(certStr, ssl_id, pgp_id, errorStr))
        {
            std::cerr << "ConnectFriendWizard::accept(): cannot load that certificate." << std::endl;
        } else {
            ServicePermissionFlags service_perm_flags(json_ssl_id["service_perm_flags"].asUInt());
            if (!peerDetails.id.isNull() && !peerDetails.gpg_id.isNull()) {
                mPeers->addFriend(peerDetails.id, peerDetails.gpg_id, service_perm_flags);

                if(peerDetails.isHiddenNode) {
                    if (!peerDetails.hiddenNodeAddress.empty() && peerDetails.hiddenNodePort) {
                        std::cerr << "ConnectFriendWizard::accept() : setting hidden address." << std::endl;
                        mPeers->setHiddenNode(peerDetails.id, peerDetails.hiddenNodeAddress, peerDetails.hiddenNodePort);
                    }
                } else {
                    //let's check if there is ip adresses in the wizard.
                    if (!peerDetails.extAddr.empty() && peerDetails.extPort) {
                        std::cerr << "ConnectFriendWizard::accept() : setting ip ext address." << std::endl;
                        mPeers->setExtAddress(peerDetails.id, peerDetails.extAddr, peerDetails.extPort);
                    }
                    if (!peerDetails.localAddr.empty() && peerDetails.localPort) {
                        std::cerr << "ConnectFriendWizard::accept() : setting ip local address." << std::endl;
                        mPeers->setLocalAddress(peerDetails.id, peerDetails.localAddr, peerDetails.localPort);
                    }
                    if (!peerDetails.dyndns.empty()) {
                        std::cerr << "ConnectFriendWizard::accept() : setting DynDNS." << std::endl;
                        mPeers->setDynDNS(peerDetails.id, peerDetails.dyndns);
                    }
                    if (!peerDetails.location.empty()) {
                        std::cerr << "ConnectFriendWizard::accept() : setting peerLocation." << std::endl;
                        mPeers->setLocation(peerDetails.id, peerDetails.location);
                    }
                }
            } else if (!peerDetails.gpg_id.isNull()) {
                std::cerr << "ConclusionPage::validatePage() accepting GPG key for connection." << std::endl;
                RsPeerId pid;
                mPeers->addFriend(pid, peerDetails.gpg_id, service_perm_flags);
            }

            //rsicontrol->getNotify().notifyListChange(NOTIFY_LIST_NEIGHBOURS,1) ;
        }
    } else {
        //setField("errorMessage", QString(tr("Certificate Load Failed:something is wrong with %1 ")).arg(fn) + ": " + getErrorString(cert_error_code));
    }
}
void ImportManager::importGroups(const Json::Value &json_groups)
{
    foreach(const Json::Value& json_group, json_groups)
    {
        std::string groupId = ""; //json_group["id"].asString();
        if(!getGrp(json_group["name"].asString(), json_group["flag"], groupId ))
        {
            // error - just skip
            continue;
        }
        Json::Value json_peers = json_group["peerIds"];
        foreach(const Json::Value& json_peer, json_peers)
        {
            RsPgpId pid(json_peer.asString());
            mPeers->assignPeerToGroup(groupId, pid, true);
        }

    }
}
/**
 * TODO: Return string and throw error instead of assigning string and returning boolean.
 */
bool ImportManager::getGrp(const std::string &name, const Json::Value &flag, std::string &id)
{
	if(getGrpIdByName(name, id))
		return true;

	// -> create one
	RsGroupInfo grp;
	grp.id = "0"; // RS will generate an ID
	grp.name = name;
	grp.flag = flag.asUInt();

	if(!mPeers->addGroup(grp))
		return false;

	// try again
	return getGrpIdByName(name, id);
}
/**
 * TODO: Return string and throw error instead of assigning string and returning boolean.
 */
bool ImportManager::getGrpIdByName(const std::string &name, std::string &id)
{
	std::list<RsGroupInfo> grpList;
	if(!mPeers->getGroupInfoList(grpList))
		return false;

	foreach (const RsGroupInfo &grp, grpList) {
		if(grp.name == name) {
			id = grp.id;
			return true;
		}
	}

	return false;
}

