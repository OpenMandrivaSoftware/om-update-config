#include "MainDialog.h"
#include <QSettings>
#include <QPushButton>
#include <QProcess>
#include <iostream>

MainDialog::MainDialog(QWidget *parent):QDialog(parent),_layout(this) {
	_options = new QGroupBox(tr("Would you like OpenMandriva Lx to update automatically?"));
	_layout.addWidget(_options);

	QVBoxLayout *optionsLayout = new QVBoxLayout(_options);
	_configs[0] = new QRadioButton(tr("No, do not check nor download updates automatically"), _options);
	_configs[1] = new QRadioButton(tr("Yes, download updates automatically, but do not install them automatically"), _options);
	_configs[2] = new QRadioButton(tr("Yes, download and install updates automatically"), _options);
	for(int i=0; i<3; i++)
		optionsLayout->addWidget(_configs[i]);

	QSettings s("/etc/dnf/automatic.conf", QSettings::IniFormat);
	s.beginGroup("commands");
	if(s.value("apply_updates").toString() == "yes")
		_configs[2]->setChecked(true);
	else if(s.value("download_updates").toString() == "yes")
		_configs[1]->setChecked(true);
	else
		_configs[0]->setChecked(true);

	_buttons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, this);
	connect(_buttons, &QDialogButtonBox::clicked, this, &MainDialog::buttonClicked);
	_layout.addWidget(_buttons);

	_buttons->button(QDialogButtonBox::Ok)->setFocus();
}

bool MainDialog::updateConfig(QString config) {
	QSettings s("/etc/dnf/automatic.conf", QSettings::IniFormat);
	if(s.isWritable()) {
		s.setValue("download_updates", _configs[0]->isChecked() ? "no" : "yes");
		s.setValue("apply_updates", _configs[2]->isChecked() ? "yes" : "no");
	} else
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
