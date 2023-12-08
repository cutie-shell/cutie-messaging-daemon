#pragma once

#include <cutiemodem.h>
#include <cutienotification.h>
#include <cutiestore.h>

class MessagingDaemon : public QObject {
	Q_OBJECT
public:
	MessagingDaemon(QObject *parent = nullptr);
public slots:
	void onIncomingMessage(QString message, QVariantMap props);
	void onModemsChanged(QList<CutieModem*> modems);
private:
	ModemSettings m_modemSettings;
	QSet<CutieModem*> m_modems;
	CutieStore m_store;
	Notifications m_notifications;
};