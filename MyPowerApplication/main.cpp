#include <QCoreApplication>
#include <QtMqtt/QMqttClient>
#include <QString>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QMqttClient mqttClient;
    mqttClient.setHostname("192.168.7.80");
    mqttClient.setPort(1883);



    QObject::connect(&mqttClient, &QMqttClient::messageReceived, [&](const QByteArray &message, const QMqttTopicName &topic) {
        qDebug() << "Received message:" << message << "on topic:" << topic.name();
        if (topic.name()=="power" && message=="shutdown") {
            qDebug() << "shutting down";
            system("shutdown /s /f /t 0");
        }
    });

    QObject::connect(&mqttClient, &QMqttClient::stateChanged, [&](QMqttClient::ClientState state) {
        qDebug() << "MQTT client state changed:" << state;

        if (state == QMqttClient::Connected) {
            QString topic = "power";
            quint8 qos = 1;
            mqttClient.subscribe(topic, qos);
            qDebug() << "Subscribe";
        }
    });

    mqttClient.connectToHost();


    return a.exec();
}
