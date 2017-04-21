#ifndef DEBUGFORM_H
#define DEBUGFORM_H

#include <QWidget>

namespace Ui {
class DebugForm;
}

class Kuzya;

class DebugForm : public QWidget
{
    Q_OBJECT

public:
    explicit DebugForm(QWidget *parent = 0, Kuzya* _mw = 0);
    ~DebugForm();
public slots:
    void slotAddMarker();
    void slotMarginClicked(int, int, Qt::KeyboardModifiers);
private:
    Ui::DebugForm *ui;
    Kuzya* mw;
};

#endif // DEBUGFORM_H
