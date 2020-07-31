#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <QWidget>
#include <QLineEdit>

class TextBox : public QLineEdit
{
    Q_OBJECT
public:
    TextBox(QWidget* parent);
    void mousePressEvent(QMouseEvent *e) override;
    void keyReleaseEvent(QKeyEvent* e) override;
signals:
    void textEdited();
    void collapse();
    void expand();
};

#endif // TEXTBOX_H
