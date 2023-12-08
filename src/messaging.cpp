#include "messaging.h"

#include <QProcess>

MessagingDaemon::MessagingDaemon(QObject *parent) 
	: QObject(parent)
	, m_modemSettings(ModemSettings(this)) {
	m_store.setAppName("cutie-messaging");
	m_store.setStoreName("messages");
	QList<CutieModem*> modemList = m_modemSettings.modems();
	m_modems = QSet(modemList.begin(), modemList.end());
	for (CutieModem *m : m_modems) {
		connect(m, &CutieModem::incomingMessage, this, &MessagingDaemon::onIncomingMessage);
	}

	connect(&m_modemSettings, &ModemSettings::modemsChanged, this, &MessagingDaemon::onModemsChanged);
}

void MessagingDaemon::onIncomingMessage(QString message, QVariantMap props) {
	props.insert("Message", QVariant(message));
	QVariantMap data = m_store.data();
	if (!data.contains("threads")) data.insert("threads", QVariant::fromValue<QList<QVariantMap>>(QList<QVariantMap>()));
	QVariantList threads = data["threads"].value<QVariantList>();

	bool threadFound = false;
	for (QVariant thread : threads) {
		if (thread.value<QVariantMap>()["Sender"].value<QString>() == props["Sender"].value<QString>()) {
			QVariantMap aThread = thread.value<QVariantMap>();
			QVariantList messages = aThread["Messages"].value<QVariantList>();
			messages.append(QVariant::fromValue<QVariantMap>(props));
			aThread["Messages"].setValue(messages);
			threads.removeAll(thread);
			threads.append(QVariant::fromValue<QVariantMap>(aThread));
			threadFound = true;
			break;
		}
	}

	if (!threadFound) {
		QVariantMap thread;
		thread.insert("Sender", props["Sender"]);
		QVariantList messages;
		messages.append(QVariant::fromValue<QVariantMap>(props));
		thread.insert("Messages", QVariant::fromValue<QVariantList>(messages));
		threads.append(QVariant::fromValue<QVariantMap>(thread));
	}

	data["threads"] = QVariant::fromValue<QVariantList>(threads);
	m_store.setData(data);
	
	m_notifications.notify("Messaging", 0, "", props["Sender"].value<QString>(), message, QStringList(), QVariantMap(), 0);
}

void MessagingDaemon::onModemsChanged(QList<CutieModem*> modems) {
	QSet<CutieModem*> modemSet(modems.begin(), modems.end());

	for (CutieModem* added : modemSet - m_modems) {
		connect(added, &CutieModem::incomingMessage, this, &MessagingDaemon::onIncomingMessage);
		m_modems.insert(added);
	}

	for (CutieModem* removed : m_modems - modemSet) {
		disconnect(removed, &CutieModem::incomingMessage, 0, 0);
		m_modems.remove(removed);
	}
}
