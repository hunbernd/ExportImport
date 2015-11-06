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
 * Slot for the browse export file botton.  Opens a file dialog to export peers
 * to a selected file.
 */
void ExportImportPage::setExportFile()
{
    QString exportFile = QFileDialog::getSaveFileName(this, tr("Set Export File"));
    ui.exportFilenameLine->setText(exportFile);

    if(!exportFile.isEmpty())
    {
        exportKeysTo(exportFile);
    }
}
/*!
 * \brief ExportImportPage::exportKeys
 *
 * Method for the to a file.
 */
void ExportImportPage::exportKeysTo(const QString &filePath)
{
    if(filePath.length() > 0) {
        printExportsToFile(filePath);
    } else {
        displayMessage(QString(tr("Export failed, no filepath set")));
    }
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
    if (certFile.open(QIODevice::WriteOnly | QIODevice::Text)) {

        QTextStream out(&certFile);
        ExportManager exporter(mPeers);
        // export the data
        out << exporter.exportJson().c_str();
        certFile.close();

        int exportCount = exporter.getExportCount();
        displayMessage(QString(tr("%1 keys exported!")).arg(exportCount));

    } else {
        displayMessage(QString(tr("Export failed, file open failed!")));
    }
}
/**************************** Importing ***************************************/
/*!
 * \brief ExportImportPage::setImportFile
 *
 * Slot for the browse import file button.  Opens a file dialog to import peers
 * from a selected file.
 */
void ExportImportPage::setImportFile()
{
    QString importFile = QFileDialog::getOpenFileName(this, tr("Set Import File"));
    ui.importFilepath->setText(importFile);

    if(!importFile.isEmpty())
    {
        importKeysFrom(importFile);
    }
}
/*!
 * \brief ExportImportPage::importKeys
 *
 * Method for importing keys from a file.
 */
void ExportImportPage::importKeysFrom(const QString &filePath)
{
    if (QFile::exists(filePath)) {

        QString jsonData = readImportFile(filePath);
        if (!jsonData.isEmpty()) {

            ImportManager importer(mPeers);
            importer.importData(jsonData.toStdString(), importGroups());

        } else {
            displayMessage(QString(tr("Certificate Load Failed:can't read from file %1 ")).arg(filePath));
        }

    } else {
        displayMessage(QString(tr("Certificate Load Failed:file %1 not found")).arg(filePath));
    }
}
/*!
 * \brief ExportImportPage::readImportFile
 *
 * Returns the contents of the file at filePath as a QString.  If the file
 * cannot be opened for any reason, an empty string is returned.  It really
 * should throw an error instead.
 *
 * \param filePath The file to read text from.
 * \return Contents of the file at filePath, or an empty string if it can't be
 * opened
 */
QString ExportImportPage::readImportFile(const QString &filePath)
{
    QFile certFile(filePath);
    if (certFile.open(QIODevice::ReadOnly | QIODevice::Text)){

        return QString(certFile.readAll());
        certFile.close();

    } else { // TODO: Should really throw an error instead.
        return QString("");
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
