#ifndef EXPORTMANAGER_H
#define EXPORTMANAGER_H

#include <retroshare/rsplugin.h>
#include "jsoncpp/json/json.h"

class ExportManager
{
public:
    ExportManager(RsPeers* mPeers);
    int getExportCount(){return export_count;}
    std::string exportJson();
private:
    Json::Value readGrpInfoList(const std::list<RsGroupInfo> &group_info_list);
    // ho ho ho I am seeing a _pattern!_
    Json::Value readGPGIds(const std::list<RsPgpId> &gpg_ids);
    Json::Value readGPGId(const RsPgpId &gpg_id);
    Json::Value readSSLIds(const std::list<RsPeerId>& ssl_ids);
    Json::Value readSSLId(const RsPeerId &ssl_id);

    RsPeers* mPeers;
    int export_count;
};

#endif // EXPORTMANAGER_H
