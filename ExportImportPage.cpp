#include "ExportImportPage.h"
#include "ExportManager.h"
#include "ImportManager.h"
#include "ui_ExportImportPage.h"
#include "ExportImportPlugin.h"

#include <retroshare-gui/RsAutoUpdatePage.h>

#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

//#define DEBUG
#define SEPARATOR ";"

ExportImportPage::ExportImportPage(RsPeers* mPeers, QWidget* parent, Qt::WindowFlags flags)
    : MainPage(parent, flags)
{
    this->mPeers = mPeers;

    /* Invoke the Qt Designer generated object setup routine */
    ui.setupUi(this);

    connect(ui.importDirButton, SIGNAL(clicked()), this, SLOT(setImportFile()));
    connect(ui.exportDirButton, SIGNAL(clicked()), this, SLOT(setExportFile()));

    connect(ui.importFromFileStart, SIGNAL(clicked()), this, SLOT(runImportFile()));
    connect(ui.exportToFileStart, SIGNAL(clicked()), this, SLOT(runExportFile()));

    connect(ui.pb_export, SIGNAL(clicked()), this, SLOT(exportKeysToTxt()));
    connect(ui.pb_import, SIGNAL(clicked()), this, SLOT(importKeysFromTxt()));
}

ExportImportPage::~ExportImportPage(){}
/*!
 * \brief ExportImportPage::displayMessage
 *
 * Display a message in a QMessageBox.  Title is always "RetroShare".
 *
 * \param msg The message to display.
 */
void ExportImportPage::displayMessage(const QString &msg)
{
    QMessageBox::information(this, tr("RetroShare"), msg);
}
/*!
 * \brief ExportImportPage::importGroups
 * \return True if the import check box is checked.
 */
bool ExportImportPage::importGroups()
{
    return (ui.cb_importGrps->checkState() == Qt::Checked);
}

/**************************** Exporting ***************************************/
/*!
 * \brief ExportImportPage::setExportFile
 *
 * Slot for the browse export file botton.  Opens a file browser to select a file
 * and sets that file's path to the export path text box.
 *
 */
void ExportImportPage::setExportFile()
{
    QString exportFile = QFileDialog::getSaveFileName(this, tr("Set Export File"));
    ui.exportFilepath->setText(exportFile);
}
/*!
 * \brief ExportImportPage::runExportFile
 *
 * Slot for file export button.  Exports peer id data to the file in the export
 * file name text box.
 */
void ExportImportPage::runExportFile()
{
    QString exportFile = ui.exportFilepath->text();
    printExportsToFile(exportFile);
}
/*!
 * \brief ExportImportPage::printExportsToFile
 *
 * Prints json data of peers to file path given.
 *
 * \param fileName Path of file where peer data will be printed.
 */
void ExportImportPage::printExportsToFile(const QString &filePath)
{
    QFile certFile(filePath);
    if(filePath.isEmpty()) {

        displayMessage(QString(tr("Export failed, no filepath set")));

    } else if (!certFile.open(QIODevice::WriteOnly | QIODevice::Text)) {

        displayMessage(QString(tr("Export failed, file open failed!")));

    } else {

        QTextStream out(&certFile);
        ExportManager exporter(mPeers);
        // export the data
        out << exporter.exportJson().c_str();
        certFile.close();

        int exportCount = exporter.getExportCount();
        displayMessage(QString(tr("%1 keys exported!")).arg(exportCount));
    }
}
/**************************** Importing ***************************************/
/*!
 * \brief ExportImportPage::setImportFile
 *
 * Slot for the browse import file button.  Opens a file dialog to select a file
 * to import from and writes that file's path to the import file path text box.
 */
void ExportImportPage::setImportFile()
{
    QString importFile = QFileDialog::getOpenFileName(this, tr("Set Import File"));
    ui.importFilepath->setText(importFile);
}
/*!
 * \brief ExportImportPage::runImportFile
 *
 * Slot fo
 */
void ExportImportPage::runImportFile()
{
    QString importFile = ui.importFilepath->text();
    importKeysFromFile(importFile);
}
/*!
 * \brief ExportImportPage::importKeys
 *
 * Imports keys from a given file path.
 *
 * Will fail with a message to the user if the file path given is empty, does
 * not exist, or cannot be opened for reading.
 */
void ExportImportPage::importKeysFromFile(const QString &filePath)
{
    QFile certFile(filePath);
    if (filePath.isEmpty()) {

        displayMessage(QString(tr("Import failed, no filepath set")));

    } else if (!QFile::exists(filePath)) {

        displayMessage(QString(tr("Certificate Load Failed:file %1 not found")).arg(filePath));

    } else if (!certFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

        displayMessage(QString(tr("Certificate Load Failed:can't read from file %1 ")).arg(filePath));

    } else {

        QString jsonData = QString(certFile.readAll());
        certFile.close();

        ImportManager importer(mPeers);
        importer.importData(jsonData.toStdString(), importGroups());

    }
}
/************************* Copy Paste *****************************************/
/*!
 * \brief ExportImportPage::exportKeysToTxt
 *
 * Exports peers to the copy-paste field.
 *
 */
void ExportImportPage::exportKeysToTxt()
{
    ui.pte_text->clear();
    ExportManager exporter(mPeers);
    QString result = QString::fromUtf8(exporter.exportJson().c_str());
    ui.pte_text->appendPlainText(result);
}
/*!
 * \brief ExportImportPage::importKeysFromTxt
 *
 * Imports peers from the copy-paste field
 */
void ExportImportPage::importKeysFromTxt()
{
    RsAutoUpdatePage::lockAllEvents();

    QString pastedText = ui.pte_text->toPlainText();

    ImportManager importer(mPeers);
    importer.importData(pastedText.toStdString(), importGroups());

    RsAutoUpdatePage::unlockAllEvents();
    ui.pte_text->clear();
}
