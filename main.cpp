#include "authenticationagent.h"

#include <QApplication>
#include <QMessageBox>
#include <QDBusInterface>
#include <QIcon>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	a.setQuitOnLastWindowClosed(false);
	a.setApplicationName(QLatin1String("minimal-policykit-agent"));
	a.setApplicationVersion(QLatin1String("1.0"));
	QIcon::setThemeName(QLatin1String("breeze"));

	QDBusConnection connection = QDBusConnection::sessionBus();
	if (!connection.registerService(QLatin1String("org.minimal.PolicyKit1.AuthenticationAgent.%1").arg(a.applicationName()))) {
		QMessageBox::critical(nullptr, QLatin1String("Authentication"), QLatin1String("Cannot register PolicyKit authentication agent!"));
		return 1;
	}
	AuthenticationAgent agent;
	return a.exec();
}
