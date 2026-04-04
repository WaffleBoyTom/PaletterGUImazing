#include "sick_file_line_edit.h"
#include "sick_logger.h"

SickFileLineEdit::SickFileLineEdit(QWidget *parent, QString label) : QWidget(parent)
{
    myLayout = new QHBoxLayout();
    myLineEdit = new QLineEdit(label);
    myLineEdit->setStyleSheet(
        "color: cornsilk; background-color: #232323"    
    );
    myFileChooser = new QPushButton(
        QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen), 
        tr("")
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

QString
SickFileLineEdit::text()
{
    return myLineEdit->text();
}

void
SickFileLineEdit::openNautilus()
{
    QFileDialog dialog;
    dialog.setWindowTitle(tr("File Chooser"));
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFile);

    // TODO: We should be using dialog::getSaveFileName static 

    if (dialog.exec() == QDialog::Accepted)
    {
        QString file_path = dialog.selectedFiles().first();
        myLineEdit->setText(file_path);
        {
            const QString native_path = QDir::toNativeSeparators(file_path);
            QString message = QString("Set Export Path: %1").arg(native_path);
            SickLogger::log(message);
        }
    }
    else
    {
        // User closed the dialog, so don't error out.
        return;
    }
}
