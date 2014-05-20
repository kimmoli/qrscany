#include "udpif.h"
#include <QProcess>
#include <QTime>
#include <QNetworkInterface>
#include <QSettings>
#include <QCoreApplication>
#include <QUdpSocket>


UdpIf::UdpIf(QObject *parent) :
    QObject(parent)
{
    m_ipAddress = "Unknown";
    m_interfaceName = "Unknown";
	m_udpPort = "Unknown";
	m_hostIp = "Unknown";
}

void UdpIf::readInitParams()
{
    QSettings settings;
    m_udpPort = settings.value("udpport", "4444").toString();
    m_hostIp = settings.value("hostip", "255.255.255.255").toString();

    emit udpPortChanged();
	emit hostIpChanged();
}

void UdpIf::writeInitParams()
{
    QSettings settings;
    settings.setValue("udpport", m_udpPort);
	settings.setValue("hostip", m_hostIp);
}


UdpIf::~UdpIf()
{
    m_udpSocket->close();
    m_udpSocketTx->close();
}

QString UdpIf::readInterfaceName()
{
    return m_interfaceName;
}

QString UdpIf::readIpAddress()
{
    return m_ipAddress;
}

QString UdpIf::readHostIp()
{
    return m_hostIp;
}

QString UdpIf::readUdpPort()
{
    return m_udpPort;
}

void UdpIf::writeHostIp(QString s)
{
    QSettings settings;

    m_hostIp = s;
    settings.setValue("hostip", m_hostIp);

    emit hostIpChanged();
}

void UdpIf::writeUdpPort(QString s)
{
    QSettings settings;

    m_udpPort = s;
    settings.setValue("udpport", m_udpPort);

    initSocket();

    emit udpPortChanged();
}

QString UdpIf::readDatagram()
{
    return QString(m_datagram);
}

QString UdpIf::readFromIp()
{
    return m_fromIp;
}

QString UdpIf::readFromPort()
{
    return m_fromPort;
}


void UdpIf::updateNetworkInfo()
{
    m_ipAddress = "Unknown";
    m_interfaceName = "Unknown";

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
            m_ipAddress = address.toString();
    }

    foreach (const QNetworkInterface &iface, QNetworkInterface::allInterfaces())
    {
        foreach (const QNetworkAddressEntry &entry, iface.addressEntries())
        {
            if (entry.ip().toString() == m_ipAddress)
                m_interfaceName = iface.name();
        }
    }

    emit interfaceNameChanged();
    emit ipAddressChanged();
}

void UdpIf::initSocket()
{
    // start listening?
    if (m_udpSocket)
        closeSocket();

    m_udpSocket = new QUdpSocket(this);
    m_udpSocket->bind(QHostAddress(m_ipAddress), quint16(m_udpPort.toInt()));

    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));

    qDebug() << "Listening " + m_ipAddress + ":" + m_udpPort;

}

void UdpIf::closeSocket()
{
    m_udpSocket->close();
}

void UdpIf::readPendingDatagrams()
{
    while (m_udpSocket->hasPendingDatagrams())
    {

        m_datagram.resize(m_udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_udpSocket->readDatagram(m_datagram.data(), m_datagram.size(), &sender, &senderPort);

        qDebug() << "from " << sender.toString() << ":" << QString::number(senderPort);
        qDebug() << m_datagram;

        m_fromIp = sender.toString();
        m_fromPort = QString::number(senderPort);

        emit datagramChanged();
        emit fromIpChanged();
        emit fromPortChanged();
    }
}

void UdpIf::sendMessage(QString msg, bool bCast)
{
    QHostAddress toAddress = bCast ? QHostAddress::Broadcast : QHostAddress(m_hostIp);

    qDebug() << "Sending \"" << msg << "\" to " << (bCast ? "broadcast" : toAddress.toString()) << ":" << m_udpPort;

    QByteArray datagram = msg.toLocal8Bit();

    if (!m_udpSocketTx)
        m_udpSocketTx = new QUdpSocket(this);

    m_udpSocketTx->writeDatagram(datagram.data(), datagram.size(), toAddress, quint16(m_udpPort.toInt()));
}


