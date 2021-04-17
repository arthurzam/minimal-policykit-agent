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
