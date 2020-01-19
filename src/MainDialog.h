#pragma once
#include <QDialog>
#include <QGroupBox>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>

class MainDialog:public QDialog {
	Q_OBJECT
public:
	MainDialog(QWidget *parent=nullptr);
protected:
	bool updateConfig(QString config);
protected slots:
	void buttonClicked(QAbstractButton*);
protected:
	QVBoxLayout			_layout;
	QGroupBox *			_options;
	QRadioButton *			_configs[3];
	QDialogButtonBox *		_buttons;
};
