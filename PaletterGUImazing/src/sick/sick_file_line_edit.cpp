#include "sick_file_line_edit.h"

#include "sick_utils.h"

SickFileLineEdit::SickFileLineEdit(
    QWidget *parent, QString label, SickFileLineEdit::Mode mode
)
    : QWidget(parent), myMode(mode)
{
    myLayout = new QHBoxLayout();
    myLineEdit = new QLineEdit(label);
    myLineEdit->setModified(false);
    myLineEdit->setStyleSheet("color: cornsilk; background-color: #232323");
    myFileChooser = new QPushButton(
        QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen), tr("")
    );
    myFileChooser->setStyleSheet(
        "color: cornsilk; "
        "background-color: #191919; "
        "selection-color: lightseagreen"
    );
    connect(
        myFileChooser,
        &QPushButton::clicked,
        this,
        &SickFileLineEdit::openNautilus
    );
    myLayout->addWidget(myLineEdit);
    myLayout->addWidget(myFileChooser);
    setLayout(myLayout);
}

void
SickFileLineEdit::openNautilus()
{
    QString file_path;
    switch (myMode)
    {
    case SickFileLineEdit::Mode::WRITE:
    {
        file_path = QFileDialog::getSaveFileName(
            this,                                 // parent
            tr("File Writer"),                    // caption
            QDir::homePath(),                     // dir
            SickUtils::supportedPaletteFormats()  // filter
        );
        break;
    }
    case SickFileLineEdit::Mode::READ:
    {
        file_path = QFileDialog::getOpenFileName(
            this,                               // parent
            tr("File Chooser"),                 // caption
            QDir::homePath(),                   // dir
            SickUtils::supportedImageFormats()  // filter
        );
        break;
    }
    }

    if (file_path.isEmpty())
        return;

    myLineEdit->setText(file_path);
    emit tellBossAboutFileLoaded(file_path);
}
