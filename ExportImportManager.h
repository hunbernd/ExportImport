#ifndef EXPORTIMPORTMANAGER_H
#define EXPORTIMPORTMANAGER_H

#include <retroshare/rsplugin.h>
#include "jsoncpp/json/json.h"

class ExportImportManager
{
public:
    ExportImportManager(RsPeers* mPeers);
    std::string exportJson();
    void importData(const std::string& json_string, bool import_groups);
    int getExportCount(){return export_count;}
private:
	bool getGrpIdByName(const std::string &name, std::string &id);
	bool getGrp(const std::string &name, const Json::Value &flag, std::string &id);

    RsPeers* mPeers;
    int export_count;
};

#endif // EXPORTIMPORTMANAGER_H
