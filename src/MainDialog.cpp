#include "MainDialog.h"
#include <QSettings>
#include <QPushButton>
#include <QProcess>
#include <QFile>
#include <iostream>

MainDialog::MainDialog(QWidget *parent):QDialog(parent),_layout(this) {
	_options = new QGroupBox(tr("Would you like to keep your system up to date automatically?"));
	_layout.addWidget(_options);

	QVBoxLayout *optionsLayout = new QVBoxLayout(_options);
	_configs[0] = new QRadioButton(tr("Do not download updates automatically"), _options);
	_configs[1] = new QRadioButton(tr("Download updates automatically, install manually"), _options);
	_configs[2] = new QRadioButton(tr("Download and install updates automatically"), _options);
	for(int i=0; i<3; i++)
		optionsLayout->addWidget(_configs[i]);

	if(QProcess::execute("/usr/bin/systemctl", QStringList{QStringLiteral("is-enabled"), QStringLiteral("system-update.timer")}) == 0) {
		int conf = 2;
		QFile f("/etc/sysconfig/system-update");
		if(f.open(QFile::ReadOnly)) {
			while(!f.atEnd()) {
				QByteArray line = f.readLine();
				if(line.startsWith("NO_INSTALL=1"))
					conf = 1;
			}
			f.close();
		}
		_configs[conf]->setChecked(true);
	} else
		_configs[0]->setChecked(true);

	_buttons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, this);
	connect(_buttons, &QDialogButtonBox::clicked, this, &MainDialog::buttonClicked);
	_layout.addWidget(_buttons);

	_buttons->button(QDialogButtonBox::Ok)->setFocus();
}

bool MainDialog::updateConfig(QString config) {
	return QProcess::execute("/usr/bin/pkexec", QStringList() << "/usr/bin/om-update-config" << config);
}

void MainDialog::buttonClicked(QAbstractButton *button) {
	QDialogButtonBox::ButtonRole role = _buttons->buttonRole(button);
	if(role == QDialogButtonBox::RejectRole)
		reject();
	else {
		if(_configs[0]->isChecked())
			updateConfig("none");
		else if(_configs[1]->isChecked())
			updateConfig("download");
		else
			updateConfig("install");
		accept();
	}
}
