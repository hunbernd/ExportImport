#ifndef IMPORTMANAGER_H
#define IMPORTMANAGER_H

#include <retroshare/rsplugin.h>
#include "jsoncpp/json/json.h"

class ImportManager
{
public:
    /*!
     * \brief ImportManager constructor
     * \param mPeers Where imported peers will be added.
     */
    ImportManager(RsPeers* mPeers);
    /*!
     * \brief Add peers to Retroshare from json string.
     *
     * Imports Retroshare peers into your retroshare profile from json data. The
     * json data is expected to be formatted in the same specifications that
     * ExportManager::exportData outputs.  It will optionally import groups from
     * the json data aswell.
     *
     * \param json_string Json data containing retroshare peers and, optionally,
     * groups
     * \param import_groups Whether or not groups should be imported as well.
     */
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
