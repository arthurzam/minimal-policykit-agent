/*
 * This file is part of is3-status (https://github.com/arthurzam/is3-status).
 * Copyright (C) 2021  Arthur Zamarin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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
