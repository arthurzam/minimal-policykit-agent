#ifndef DIALOGAGENT_H
#define DIALOGAGENT_H

#include <polkitqt1-identity.h>

#include <QDialog>

class QLineEdit;
class QLabel;
class QComboBox;

class DialogAgent final : public QDialog {
public:
	DialogAgent(const QString &details, const QString &message, const QString &iconName,
				const PolkitQt1::Identity::List &identities, QWidget *parent = nullptr);
	~DialogAgent() = default;

	void setPrompt(const QString &text, bool echo);
	QString identity();
	QString response();
private:
	QLineEdit *tb_password;
	QLabel *lb_prompt;
	QComboBox *combo_identity;
};
#endif // DIALOGAGENT_H
