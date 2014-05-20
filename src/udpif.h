#ifndef UdpIf_H
#define UdpIf_H
#include <QObject>
#include <QNetworkConfiguration>
#include <QUdpSocket>

class UdpIf : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ipAddress READ readIpAddress() NOTIFY ipAddressChanged())
    Q_PROPERTY(QString interfaceName READ readInterfaceName() NOTIFY interfaceNameChanged())
    Q_PROPERTY(QString hostIp READ readHostIp() WRITE writeHostIp(QString) NOTIFY hostIpChanged())
    Q_PROPERTY(QString udpPort READ readUdpPort() WRITE writeUdpPort(QString) NOTIFY udpPortChanged())
    Q_PROPERTY(QString fromPort READ readFromPort() NOTIFY fromPortChanged())
    Q_PROPERTY(QString fromIp READ readFromIp() NOTIFY fromIpChanged())
    Q_PROPERTY(QString datagram READ readDatagram NOTIFY datagramChanged())

public:
    explicit UdpIf(QObject *parent = 0);
    ~UdpIf();

    QString readIpAddress();
    QString readInterfaceName();
    QString readHostIp();
    QString readUdpPort();
    void writeHostIp(QString);
    void writeUdpPort(QString);
    QString readFromIp();
    QString readFromPort();
    QString readDatagram();

    Q_INVOKABLE void initSocket();
    Q_INVOKABLE void sendMessage(QString msg, bool bCast);

    Q_INVOKABLE void updateNetworkInfo();
    Q_INVOKABLE void readInitParams();
    Q_INVOKABLE void writeInitParams();

signals:
    void ipAddressChanged();
    void interfaceNameChanged();
    void udpPortChanged();
    void hostIpChanged();
    void datagramChanged();
    void fromIpChanged();
    void fromPortChanged();

private slots:
    void readPendingDatagrams();

private:
    void closeSocket();
    QString m_ipAddress;
    QString m_interfaceName;
	QString m_udpPort;
	QString m_hostIp;
    QUdpSocket *m_udpSocket;
    QUdpSocket *m_udpSocketTx;
    QByteArray m_datagram;
    QString m_fromIp;
    QString m_fromPort;

};


#endif // UdpIf_H
