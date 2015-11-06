#ifndef EXPORTMANAGER_H
#define EXPORTMANAGER_H

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
 * \brief The ExportManager writes retroshare peers to a Json file.
 */
class ExportManager
{
public:
    /*!
     * \brief ExportManager constructor
     * \param mPeers Retroshare peers to export.
     */
    ExportManager(RsPeers* mPeers);
    /*!
     * \brief getExportCount
     *
     * Returns how many gpg peers ids were last exported by
     * ExportManager::exportJson. It will return 0 if no peers have yet been
     * exported.
     *
     * \return How many peers were last exported.
     */
    int getExportCount(){return export_count;}
    /*!
     * \brief exportJson
     *
     * Returns a string of json data containing a list of all retroshare peers
     * and groups. The format of the json data is as follows:
     * \code{.json}
     * {
     *      "gpg_ids" : [
     *          {
     *              "email" :
     *              "gpg_id" :
     *              "name" :
     *              "ssl_ids" : [
     *                  {
     *                      "id" :
     *                      "location" :
     *                      "pubkey" :
     *                      "service_perm_flags" :
     *                  }, ...
     *              ]
     *              "trustLvl":
     *          }, ...
     *      ],
     *      "groups" : [
     *          {
     *              "flag" :
     *              "name" :
     *              "peerIds" : [
     *                  gpg_id, ...
     *              ]
     *          }
     *      ]
     * }
     * \endcode
     * \return Json text of Retroshare peers.
     */
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
