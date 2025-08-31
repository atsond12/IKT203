#ifndef TMAINFORM_H
#define TMAINFORM_H

#include <QMainWindow>
#include "ui_frmMain.h"
#include "list.hpp"       
#include "TSong.h"
#include "TSongListModel.h"

//TODO: Create Qt GUI class frmMain
class TMainForm : public QMainWindow
{
	Q_OBJECT
public:
	explicit TMainForm(QWidget*);
	~TMainForm() override;

private:
	Ui::frmMain* ui;

	TLinkedList<TSong*>* llSongs; // The student's main data structure
	TSongListModel* qtblSongModel;   // Our bridge to the view


private slots:
	void btnLoadSongsClick();
};

#endif // TMAINFORM_H