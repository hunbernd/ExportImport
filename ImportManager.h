#ifndef IMPORTMANAGER_H
#define IMPORTMANAGER_H

#include <retroshare/rsplugin.h>
#include "jsoncpp/json/json.h"

class ImportManager
{
public:
    ImportManager(RsPeers* mPeers);
    void importData(const std::string& json_string, bool import_groups);
private:
    void importGPGIds(const Json::Value &gpg_ids);
    void importSSLIds(const Json::Value &ssl_ids);
    void importGroups(const Json::Value &groups);
    void importSSLId(const Json::Value &json_ssl_id);

    bool getGrpIdByName(const std::string &name, std::string &id);
	bool getGrp(const std::string &name, const Json::Value &flag, std::string &id);

    RsPeers* mPeers;
};

#endif // IMPORTMANAGER_H
