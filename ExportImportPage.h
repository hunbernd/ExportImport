#ifndef EXPORTPAGE_H
#define EXPORTPAGE_H

#include <retroshare-gui/mainpage.h>
#include <retroshare/rsplugin.h>
#include <retroshare/rsiface.h>
#include "ui_ExportImportPage.h"

class ExportImportPage : public MainPage
{
    Q_OBJECT
    
public:
    /*!
     * \brief ExportImportPage constructor
     *
     * \param mPeers User's retroshare peers
     * \param parent
     * \param flags
     */
    ExportImportPage(RsPeers* mPeers, QWidget* parent = 0, Qt::WindowFlags flags = 0);
    /*! Class destructor */
    ~ExportImportPage();
    /*!
     * \brief Inherited from MainPage, does nothing.
     *
     * Project can be built without this function.  I don't know why it is here.
     */
    void load(){}
    /*!
     * \brief Inherited from MainPage, does nothing.
     *
     * Project can be built without this function.  I don't know why it is here.
     *
     * \return True, always.
     */
    bool save(QString /*&str*/) {return true;}
    /*!
     * \brief Label of the plugin as it appears in Retroshare.
     * \return "ExportTools"
     */
    QString pageName() const { return "ExportTools";  }
    /*!
     * \brief Icon of the plugin as it appears in Retroshare
     * \return An Icon of a Lock.
     */
    QIcon iconPixmap() const { return QIcon(":/images/encrypted32.png") ; }
private:
    /** Qt Designer generated object */
    Ui::ExportPage ui;
    /** Peers to Export from/Import to */
    RsPeers* mPeers;

    // misc methods
    bool importGroups();
    void displayMessage(const QString &msg);
    // exporting
    void printExportsToFile(const QString &filePath);
    // importing
    void importKeysFromFile(const QString &filePath);
    // ???
    ServicePermissionFlags serviceFlags() const;

private slots:
    void setExportFile(); // browse export file button
    void setImportFile(); // browse import file button
    void runExportFile(); // run export file button
    void runImportFile(); // run import file button
    void exportKeysToTxt(); // export keys to copy-paste button
    void importKeysFromTxt(); // import keys to copy-paste button

};

#endif // EXPORTPAGE_H
