#ifndef SICK_FILELINEEDIT_H
#define SICK_FILELINEEDIT_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
class QAction;
class QWidget;
class QPushButton;
class QLineEdit;
class QHBoxLayout;
class QString;
QT_END_NAMESPACE


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
        SickFileLineEdit::Mode mode = SickFileLineEdit::Mode::READ);
    
    QString text() const { return myLineEdit->text(); }
    
    SickFileLineEdit::Mode mode()  const { return myMode; }
    
    bool isRead()  const { return myMode == Mode::READ;  }
    bool isWrite() const { return myMode == Mode::WRITE; }

private:
    
    /// called when button is pushed
    void openNautilus();
    
    
    QHBoxLayout *myLayout;
    QPushButton *myFileChooser;
    QLineEdit   *myLineEdit;
    
    
    SickFileLineEdit::Mode myMode;
    
    
};

#endif  // SICK_FILELINEEDIT_H
