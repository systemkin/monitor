#ifndef SHOWHISTORYFORM_H
#define SHOWHISTORYFORM_H

#include <QDialog>
#include "mainwindow.h"
#include "showhistorymodel.h"
namespace Ui {
class showHistoryForm;
}
/*!
 * \brief Form to display log and tied buttons
 */
class showHistoryForm : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief constructor
     * \param client - tcpClient to be used for conenction with server
     * \param parent
     */
    explicit showHistoryForm(tcpClient *client, MainWindow *parent = nullptr);
    ~showHistoryForm();

private slots:
    /*!
     * \brief slot for serving click on button for printing
     */
    void on_pushButton_print_clicked();

    /*!
     * \brief slot for serving click on button for refreshing
     */
    void on_pushButton_request_clicked();

private:

    Ui::showHistoryForm *ui;

    /*!
     * \brief model for showing data in a table
     */
    showHistoryModel* model;

    /*!
     * \brief functiion to serve completed tcp requests
     * \param requestObject - obect used to generate request
     * \param responseDoc - response from server
     */
    void onRequestCompleted(const QJsonObject &requestObject, const QJsonDocument &responseDoc);
    /*!
     * \brief function for saving vector of historyItem to different data formats
     * \param container - data to save
     * \param filename - file where to save
     */
    void saveToFile(const std::vector<historyItem>& container, const QString& filename);

    /*!
     * \brief function for saving vector of historyItem to PDF
     * \param fileName - file where to save
     * \param container - data to save
     */
    void exportToPDF(const QString &fileName, const std::vector<historyItem>& container);

    /*!
     * \brief function for saving vector of historyItem to text
     * \param fileName - file where to save
     * \param container - data to save
     */
    void exportToText(const QString &fileName, const std::vector<historyItem>& container);

    /*!
     * \brief function for saving vector of historyItem to CSV
     * \param fileName - file where to save
     * \param container - data to save
     */
    void exportToCSV(const QString &fileName, const std::vector<historyItem>& container);

    /*!
     * \brief function for saving vector of historyItem to HTML
     * \param fileName - file where to save
     * \param container - data to save
     */
    void exportToHTML(const QString &fileName, const std::vector<historyItem>& container);

    /*!
     * \brief function for generating HTML-table from vector of historyItem
     * \param fileName - file where to save
     * \param container - data to save
     */
    QString generateHTMLTable(const std::vector<historyItem>& container);

    /*!
     * \brief client for sending and recueving JSON reuqests and responces through TCP
     */
    tcpClient *client;
};

#endif // SHOWHISTORYFORM_H
