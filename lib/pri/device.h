#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QJsonObject>
/*!
 * \brief Class that describes device. Provided with Json serialization and deserialization for transmitting data between GUI and service.
 */
class device
{
public:
    device();

    /*!
     * \brief - constructor with setting all fields directly
     * \param serial - serial nubmer
     * \param name - device name
     * \param description - device description
     * \param type - true for monitoring from bash, false for monitoring from file
     */
    device(QString serial, QString name, QString description, bool type);

    /*!
     * \brief constructor from Json
     * \param deviceJson - json with necessary fields: serial, name, description, type
     */
    device(QJsonObject deviceJson);
    /*!
     * \brief creates json representation of device
     * \return Json-representation of device with fields: serial, name, description, type
     */
    QJsonObject toJson();
    /*!
     * \brief device serial
     */
    QString serial;

    /*!
     * \brief device name
     */
    QString name;

    /*!
     * \brief device description
     */
    QString description;

    /*!
     * \brief device type. true for monitoring from bash, false for monitoring from file
     */
    bool type;

    /*!
     * \brief comparison operator
     * \param other - other device
     * \return true if equals, false otherwise.
     */
    bool operator==(const device& other) const;
private:
};
#endif // DEVICE_H
