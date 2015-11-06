#ifndef EXPORTTOOLSPLUGIN_H
#define EXPORTTOOLSPLUGIN_H

#include <retroshare/rsplugin.h>
/*!
 * \file
 * \author Anonymous
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
 * \brief The ExportImportPlugin is ExportImport's interface with the
 * Retroshare program.
 */
class ExportImportPlugin : public RsPlugin
{
public:
    ExportImportPlugin();

    MainPage* qt_page()const;
    ConfigPage* qt_config_page() const;
    QDialog* qt_about_page() const;
    QIcon* qt_icon() const;

    std::string getShortPluginDescription() const;
    std::string getPluginName() const;
    void getPluginVersion(int& major, int& minor, int &build, int& svn_rev) const;

    //
    //========================== Plugin Interface ================================//
    //
    // Use these methods to access main objects from RetroShare.
    //
    void setInterfaces(RsPlugInInterfaces& interfaces);
    void setPlugInHandler(RsPluginHandler* pgHandler);

private:
    RsPeers* mPeers;
    RsPluginHandler* mPlugInHandler;
    QIcon* mIcon;
};

#endif // EXPORTTOOLSPLUGIN_H
