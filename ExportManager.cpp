#include "ExportManager.h"
#include <QtGlobal>
#include <algorithm>
#include "json/json.h"

/**
 * Constructor
 */
ExportManager::ExportManager(RsPeers* mPeers)
{
    this->mPeers = mPeers;
}
/**
 * Reads all current contacts and returns them as a json file.
 */
std::string ExportManager::exportJson()
{
    std::list<RsGroupInfo> group_info_list;
    mPeers->getGroupInfoList(group_info_list);

    std::list<RsPgpId> gpg_ids;
    mPeers->getGPGAcceptedList(gpg_ids);
    //mPeers->getGPGAllList(gpg_ids);

    try 
    {
        Json::Value root;
        root["groups"] = readGrpInfoList(group_info_list);
        root["gpg_ids"] = readGPGIds(gpg_ids);
        this->export_count = root["gpg_ids"].size();
        Json::StyledWriter writer;
        return writer.write(root);
    }
    catch (std::string& msg)
    {
        std::cerr << msg;
        return NULL;
    }
}
Json::Value ExportManager::readGPGIds(const std::list<RsPgpId> &gpg_ids)
{
    Json::Value json_gpg_ids;
    foreach(const RsPgpId& gpg_id, gpg_ids)
    {
        json_gpg_ids.append(readGPGId(gpg_id));
    }
    return json_gpg_ids;

}
Json::Value ExportManager::readGPGId(const RsPgpId &gpg_id)
{
    RsPeerDetails gpg_detail;
    mPeers->getGPGDetails(gpg_id, gpg_detail);

    Json::Value json_gpg_id;
    json_gpg_id["name"] = gpg_detail.name;
    json_gpg_id["email"] = gpg_detail.email;
    json_gpg_id["trustLvl"] = gpg_detail.trustLvl;
    std::string gid = gpg_detail.gpg_id.toStdString();
    json_gpg_id["gpg_id"] = gid;

    //json_gpg_id["id"] = gpg_detail.id;
    //json_gpg_id["validLvl"] = gpg_detail.validLvl;
    //json_gpg_id["location"] = gpg_detail.location;
    //json_gpg_id["service_perm_flags"] = gpg_detail.service_perm_flags.toUInt32();

    std::list<RsPeerId> ssl_ids;
    mPeers->getAssociatedSSLIds(gpg_id, ssl_ids);
    json_gpg_id["ssl_ids"] = readSSLIds(ssl_ids);
    return json_gpg_id;
}
/**
 */
Json::Value ExportManager::readSSLIds(const std::list<RsPeerId> &ssl_ids)
{
    Json::Value json_ssl_ids;
    foreach(const RsPeerId& ssl_id, ssl_ids)
    {
        json_ssl_ids.append(readSSLId(ssl_id));
    }
    return json_ssl_ids;
}
Json::Value ExportManager::readSSLId(const RsPeerId &ssl_id)
{
    RsPeerDetails detail;
    if (!mPeers->getPeerDetails(ssl_id, detail))
    {
        throw "Error : cannot get peer details.";
    }
    //std::string invite = mPeers->GetRetroshareInvite(detail.id,ui._shouldAddSignatures_CB->isChecked(),ui._useOldFormat_CB->isChecked()) ; // this needs to be a SSL id
    std::string invite = mPeers->GetRetroshareInvite(detail.id, true) ; // this needs to be a SSL id

    Json::Value json_ssl_id;
    std::string sid = detail.id.toStdString();
    json_ssl_id["id"] = sid;
    json_ssl_id["location"] = detail.location;
    json_ssl_id["pubkey"] = invite.c_str();
    json_ssl_id["service_perm_flags"] = detail.service_perm_flags.toUInt32();
    return json_ssl_id;
}
Json::Value ExportManager::readGrpInfoList(const std::list<RsGroupInfo> &group_info_list)
{
    Json::Value json_groups;
    foreach(const RsGroupInfo& group_info, group_info_list)
    {
        //RsGroupInfo group_info = *list_iter;
        Json::Value json_group;
        json_group["name"] = group_info.name;
        json_group["flag"] = group_info.flag;
        // use name as identitfier since IDs may change between instances
        // json_group["id"] = group_info.id;

        Json::Value json_peer_ids;
        foreach(const RsPgpId& gpg_id, group_info.peerIds)
        {
            json_peer_ids.append(gpg_id.toStdString());
        }
        json_group["peerIds"] = json_peer_ids;
        json_groups.append(json_group);
    }
    return json_groups;
}
