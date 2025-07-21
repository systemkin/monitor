#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./ui_mainwindow.h"
#include "configreader.h"
#include "deviceinfomodel.h"
#include "tcpclient.h"


#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/*!
 * \brief class of main application windor
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /*!
     * \brief slot to serve completed tcp requests
     * \param requestObject - obect used to generate request
     * \param responseDoc - response from server
     */
    void onRequestCompleted(const QJsonObject &requestObject, const QJsonDocument &responseDoc);

    /*!
     * \brief slot to handle tcp errors
     */
    void onConnectionError();

    /*!
     * \brief slot to handle adding device to tracking
     */
    void on_buttonAdd_clicked();

    /*!
     * \brief slot to handle updating device
     */
    void on_buttonChange_clicked();

    /*!
     * \brief slot to handle deleting device
     */
    void on_buttonDelete_clicked();

    /*!
     * \brief slot to handle export current devices to file
     */
    void on_buttonExport_clicked();

    /*!
     * \brief slot to handle export current devices to heuman-readable format
     */
    void on_buttonPrint_clicked();

    /*!
     * \brief slot to handle refreshing info about devices
     */
    void on_buttonRefresh_clicked();

    /*!
     * \brief slot to handle showing log
     */
    void on_buttonHistory_clicked();

    /*!
     * \brief slot to handle importing devices from data
     */
    void on_buttonImport_clicked();

private:
    /*!
     * \brief config of a GUI application
     */
    Config config;
    Ui::MainWindow *ui;

    /*!
     * \brief model, that stores data, recieved from server
     */
    DeviceInfoModel* model;

    /*!
     * \brief function for saving vector of deviceInfo to different data formats
     * \param container - data to save
     * \param filename - file where to save
     */
    void saveToFile(const std::vector<deviceInfo>& container, const QString& filename);

    /*!
     * \brief function for saving vector of deviceInfo to PDF
     * \param fileName - file where to save
     * \param container - data to save
     */
    void exportToPDF(const QString &fileName, const std::vector<deviceInfo>& container);

    /*!
     * \brief function for saving vector of deviceInfo to text
     * \param fileName - file where to save
     * \param container - data to save
     */
    void exportToText(const QString &fileName, const std::vector<deviceInfo>& container);

    /*!
     * \brief function for saving vector of deviceInfo to CSV
     * \param fileName - file where to save
     * \param container - data to save
     */
    void exportToCSV(const QString &fileName, const std::vector<deviceInfo>& container);

    /*!
     * \brief function for saving vector of deviceInfoto HTML
     * \param fileName - file where to save
     * \param container - data to save
     */
    void exportToHTML(const QString &fileName, const std::vector<deviceInfo>& container);

    /*!
     * \brief function for generating HTML-table from vector of deviceInfo
     * \param fileName - file where to save
     * \param container - data to save
     */
    QString generateHTMLTable(const std::vector<deviceInfo>& container);

    /*!
     * \brief client for sending and recueving JSON reuqests and responces through TCP
     */
    tcpClient *client;
};
#endif // MAINWINDOW_H
