
#include "TMainForm.h"
#include <QDebug>

//Constructor
TMainForm::TMainForm(QWidget* aOwner)
	: QMainWindow(aOwner)
	, ui(new Ui::frmMain)
{
	ui->setupUi(this);

    ui->qtblMainSongList->setStyleSheet("QTableView::indicator { subcontrol-position: center; }");

	llSongs = new TLinkedList<TSong*>(true); // The student's linked list
	qtblSongModel = new TSongListModel(llSongs, this); // The model bridging the list to the view

	ui->qtblMainSongList->setModel(qtblSongModel); // Connect the model to the view

	//Connect the btnAddSongs OnClick event to the btnAddSongsClick event handler
	connect(ui->btnLoadSongs, &QPushButton::clicked, this, &TMainForm::btnLoadSongsClick);
}

//Destructor
TMainForm::~TMainForm()
{
	delete ui;
	delete qtblSongModel; // Clean up the model first
	delete llSongs; // Then clean up the linked list

}

//Event handler for btnAddSongs OnClick
void TMainForm::btnLoadSongsClick()
{
    qDebug() << "btnLoadSongs clicked, adding new songs...";

    llSongs->Append(new TSong{ "Hallowed Be Thy Name", "Iron Maiden", "The Number of the Beast", 1982, true, "" });
    llSongs->Append(new TSong{ "Master of Puppets", "Metallica", "Master of Puppets", 1986, true, "" });
    llSongs->Append(new TSong{ "Crazy Train", "Ozzy Osbourne", "Blizzard of Ozz", 1980, false, "" });

    // 3. CRUCIAL: Call the refresh function to update the QTableView.
    qtblSongModel->RefreshModel();
}