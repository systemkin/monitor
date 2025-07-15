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
    device(int id, QString serial, QString name, QString description, bool type);
    device(QJsonObject deviceJson);
private:
    int id;
    QString serial;
    QString name;
    QString description;
    bool type;
    bool operator==(const device& other) const;
    QJsonObject toJson();
};
#endif // DEVICE_H
