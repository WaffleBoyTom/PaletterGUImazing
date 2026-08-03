#ifndef SICK_FILELINEEDIT_H
#define SICK_FILELINEEDIT_H

#include <QtWidgets>

class SickFileLineEdit : public QWidget
{
    Q_OBJECT

public:
    /// this determines the behaviour of the file chooser
    /// Read means it only looks for existing files
    /// Write means it lets you write the name of a file as well.
    enum class Mode
    {
        READ = 0,
        WRITE
    };

    explicit SickFileLineEdit(
        QWidget *parent,
        QString label,
        SickFileLineEdit::Mode mode = SickFileLineEdit::Mode::READ
    );

    QString
    text() const
    {
        return myLineEdit->text();
    }

    void
    updateText(QString text) const
    {
        myLineEdit->blockSignals(true);
        myLineEdit->setText(std::move(text));
        myLineEdit->setModified(false);
        myLineEdit->blockSignals(false);
    }

    SickFileLineEdit::Mode
    mode() const
    {
        return myMode;
    }

    bool
    isRead() const
    {
        return myMode == Mode::READ;
    }
    bool
    isWrite() const
    {
        return myMode == Mode::WRITE;
    }

    /// accessor to be able to hook into signal
    const QLineEdit *
    lineEdit() const
    {
        return myLineEdit;
    }

signals:
    /// emit a signal when a valid file is loaded
    void tellBossAboutFileLoaded(QString &file);

private:
    /// called when button is pushed
    void openNautilus();

    QHBoxLayout *myLayout;
    QPushButton *myFileChooser;
    QLineEdit *myLineEdit;

    SickFileLineEdit::Mode myMode;
};

#endif  // SICK_FILELINEEDIT_H
