#ifndef IMPORTMANAGER_H
#define IMPORTMANAGER_H

#include <retroshare/rsplugin.h>
#include "jsoncpp/json/json.h"
/*!
 * \file
 * \author Anonymous (orignal ExportImportManager)
 * \author cheezsteak
 *
 * \section LICENSE
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details
 * at https://www.gnu.org/copyleft/gpl.html
 *
 * \section DESCRIPTION
 *
 * \brief The ImportManager adds Retroshare Peers from a Json file.
 */
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
