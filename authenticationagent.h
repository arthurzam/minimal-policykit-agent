#ifndef AUTHENTICATIONAGENT_H
#define AUTHENTICATIONAGENT_H

#include <polkitqt1-agent-session.h>
#include <polkitqt1-identity.h>
#include <polkitqt1-details.h>
#include <polkitqt1-agent-listener.h>

#include <QHash>

#include <memory>

class DialogAgent;

class AuthenticationAgent final : public PolkitQt1::Agent::Listener {
public:
	AuthenticationAgent(QObject *parent = nullptr);
	~AuthenticationAgent();

	void initiateAuthentication(const QString &actionId,
								const QString &message,
								const QString &iconName,
								const PolkitQt1::Details &details,
								const QString &cookie,
								const PolkitQt1::Identity::List &identities,
								PolkitQt1::Agent::AsyncResult *result) override;
	bool initiateAuthenticationFinish() override;
	void cancelAuthentication() override;

	void on_gui_finished(int result);
	void request(const QString &request, bool echo);
	void completed(bool gainedAuthorization);
	void showError(const QString &text);
	void showInfo(const QString &text);
private:
	bool m_inProgress = false;
	std::unique_ptr<DialogAgent> m_gui;
	PolkitQt1::Agent::Session *m_session;
	QHash<PolkitQt1::Agent::Session*, PolkitQt1::Identity> m_SessionIdentity;

	void deleteSessions();
};

#endif // AUTHENTICATIONAGENT_H
