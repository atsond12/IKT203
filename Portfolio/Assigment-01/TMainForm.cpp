
#include "TMainForm.h"
#include <QDebug>

//Constructor
TMainForm::TMainForm(QWidget* aOwner)
	: QMainWindow(aOwner)
	, ui(new Ui::frmMain)
{
	ui->setupUi(this);

    ui->qtblMainSongList->setStyleSheet("QTableView::indicator { subcontrol-position: center; }");

	llSongs = new TLinkedList<TSong*>(); // The student's linked list
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

    // 1. Create the new songs using factory functions, just like in the constructor.
    auto songFactory1 = []() {
        return new TSong{ "Hallowed Be Thy Name", "Iron Maiden", "The Number of the Beast", 1982, true, "" };
        };
    auto songFactory2 = []() {
        return new TSong{ "Master of Puppets", "Metallica", "Master of Puppets", 1986, true, "" };
        };
    auto songFactory3 = []() {
        return new TSong{ "Crazy Train", "Ozzy Osbourne", "Blizzard of Ozz", 1980, false, "" };
        };

    // 2. Add the songs to the student's linked list.
    llSongs->Append(songFactory1);
    llSongs->Append(songFactory2);
    llSongs->Append(songFactory3);

    // 3. CRUCIAL: Call the refresh function to update the QTableView.
    qtblSongModel->RefreshModel();
}