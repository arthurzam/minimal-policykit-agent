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

#include "dialogagent.h"

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QToolButton>
#include <QLabel>

static QSizePolicy calc_size_policy(QSizePolicy::Policy horizontal, QSizePolicy::Policy vertical, QWidget *widget) {
	QSizePolicy sizePolicy(horizontal, vertical);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
	return sizePolicy;
}

DialogAgent::DialogAgent(const QString &details, const QString &message, const QString &iconName,
						 const PolkitQt1::Identity::List &identities, QWidget *parent)
	: QDialog(parent, Qt::WindowStaysOnTopHint)
{
	setWindowTitle(QLatin1String("Authentication Required"));
	setMinimumSize(400, 0);
	setSizePolicy(calc_size_policy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding, this));

	QGridLayout *grid = new QGridLayout(this);
	grid->setSizeConstraint(QLayout::SetMinimumSize);

	QLabel *lb_icon = new QLabel(this);
	lb_icon->setMinimumSize(64, 64);
	lb_icon->setMaximumSize(64, 64);
	lb_icon->setPixmap(QIcon::fromTheme(iconName, QIcon::fromTheme("dialog-password")).pixmap(64, 64));
	grid->addWidget(lb_icon, 0, 0);

	QLabel *lb_msg = new QLabel(message, this);
	lb_msg->setSizePolicy(calc_size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding, lb_msg));
	lb_msg->setWordWrap(true);
	grid->addWidget(lb_msg, 0, 1);

	grid->addWidget(new QLabel(QLatin1String("Identity"), this), 1, 0);
	grid->addWidget(combo_identity = new QComboBox(this), 1, 1);
	for (const auto &id : identities) {
		combo_identity->addItem(id.toString());
	}
	int idx = combo_identity->findText(qgetenv("USER"));
	if (idx >= 0)
		combo_identity->setCurrentIndex(idx);

	grid->addWidget(lb_prompt = new QLabel(QLatin1String("Password"), this), 2, 0);
	tb_password = new QLineEdit(this);
	tb_password->setEchoMode(QLineEdit::Normal);
	tb_password->setFocus();
	grid->addWidget(tb_password, 2, 1);

	QToolButton *bt_details = new QToolButton(this);
	bt_details->setText(QLatin1String("Details"));
	bt_details->setCheckable(true);
	bt_details->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	bt_details->setArrowType(Qt::DownArrow);
	grid->addWidget(bt_details, 3, 0);

	QLabel *lb_details = new QLabel(details, this);
	lb_details->setVisible(false);
	grid->addWidget(lb_details, 4, 0, 1, 2);
	connect(bt_details, &QToolButton::toggled, lb_details, &QLabel::setVisible);
	connect(bt_details, &QToolButton::toggled, this, &DialogAgent::adjustSize);

	QDialogButtonBox *buttons = new QDialogButtonBox(this);
	buttons->setOrientation(Qt::Horizontal);
	grid->addWidget(buttons, 5, 0, 1, 2);
	buttons->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
	connect(buttons, &QDialogButtonBox::accepted, this, &DialogAgent::accept);
	connect(buttons, &QDialogButtonBox::rejected, this, &DialogAgent::reject);
}

void DialogAgent::setPrompt(const QString &text, bool echo) {
	lb_prompt->setText(text);
	tb_password->setEchoMode(echo ? QLineEdit::Normal : QLineEdit::Password);
	tb_password->setFocus();
}

QString DialogAgent::identity() {
	return combo_identity->currentText();
}

QString DialogAgent::response() {
	return tb_password->text();
}
