#include "textbox.h"
#include <QDebug>
#include <QKeyEvent>

TextBox::TextBox(QWidget* parent)
    :QLineEdit(parent)
{

}

void TextBox::mousePressEvent(QMouseEvent *)
{
    emit expand();
}


void TextBox::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Return && text().size())
    {
        emit collapse();
        emit textEdited();
    }
    if (text().size() == 0 || e->key() == Qt::Key_Escape)
    {
        emit collapse();
    }
    QLineEdit::keyReleaseEvent(e);
}
