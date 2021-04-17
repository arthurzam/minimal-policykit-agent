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
#include "dialogagent.h"

#include <polkitqt1-subject.h>

#include <QApplication>
#include <QMessageBox>

using PolkitQt1::Agent::Session;

AuthenticationAgent::AuthenticationAgent(QObject *parent)
	: PolkitQt1::Agent::Listener(parent) {
	PolkitQt1::UnixSessionSubject session(getpid());
	if (!registerListener(session, QLatin1String("/org/minimal/PolicyKit1/AuthenticationAgent"))) {
		QMessageBox::critical(nullptr, QLatin1String("Authentication"), QLatin1String("Cannot register PolicyKit authentication agent!"));
		qApp->quit();
	}
}

AuthenticationAgent::~AuthenticationAgent() {
	if (m_gui)
		m_gui->blockSignals(true);
}

static QString getDetails(const QString &actionId, const PolkitQt1::Details &details) {
	QString result = actionId;
	for (const auto &key : details.keys()) {
		result += "<br />" + key + " : " + details.lookup(key);
	}
	return result;
}

void AuthenticationAgent::initiateAuthentication(const QString &actionId, const QString &message,
												 const QString &iconName, const PolkitQt1::Details &details,
												 const QString &cookie, const PolkitQt1::Identity::List &identities,
												 PolkitQt1::Agent::AsyncResult *result) {
	if (m_inProgress) {
		QMessageBox::information(nullptr, QLatin1String("PolicyKit Information"), QLatin1String("Another authentication is in progress. Please try again later."));
		return;
	}
	m_inProgress = true;
	deleteSessions();

	m_gui.reset(new DialogAgent(getDetails(actionId, details), message, iconName, identities));

	for (const auto &id : identities) {
		Session *session = new Session(id, cookie, result);
		m_SessionIdentity[session] = id;
		connect(session, &Session::request, this, &AuthenticationAgent::request);
		connect(session, &Session::completed, this, &AuthenticationAgent::completed);
		connect(session, &Session::showError, this, &AuthenticationAgent::showError);
		connect(session, &Session::showInfo, this, &AuthenticationAgent::showInfo);
		session->initiate();
	}
}

bool AuthenticationAgent::initiateAuthenticationFinish() {
	m_inProgress = false;
	return true;
}

void AuthenticationAgent::cancelAuthentication() {
	m_inProgress = false;
}

void AuthenticationAgent::on_gui_finished(int result) {
	Q_ASSERT(m_session);
	if (result == QDialog::Accepted && m_gui->identity() == m_SessionIdentity[m_session].toString())
		m_session->setResponse(m_gui->response());
	else
		m_session->cancel();
}

void AuthenticationAgent::request(const QString &request, bool echo) {
	m_gui->setPrompt(request, echo);
	m_session = qobject_cast<PolkitQt1::Agent::Session *>(sender());
	connect(m_gui.get(), &QDialog::finished, this, &AuthenticationAgent::on_gui_finished);
	m_gui->show();
	m_gui->raise();
}

void AuthenticationAgent::completed(bool gainedAuthorization) {
	PolkitQt1::Agent::Session *session = qobject_cast<PolkitQt1::Agent::Session *>(sender());
	if (m_inProgress && m_gui->identity() == m_SessionIdentity[session].toString()) {
		if (!gainedAuthorization)
			QMessageBox::information(nullptr, QLatin1String("Authorization Failed"), QLatin1String("Authorization failed for some reason"));

		session->result()->setCompleted();
		m_inProgress = false;
	}
}

void AuthenticationAgent::showError(const QString &text) {
	QMessageBox::warning(nullptr, QLatin1String("PolicyKit Error"), text);
}

void AuthenticationAgent::showInfo(const QString &text) {
	QMessageBox::information(nullptr, QLatin1String("PolicyKit Information"), text);
}

void AuthenticationAgent::deleteSessions() {
	for (auto i = m_SessionIdentity.begin(), i_e = m_SessionIdentity.end(); i != i_e; ++i)
		delete i.key();
	m_SessionIdentity.clear();
}

