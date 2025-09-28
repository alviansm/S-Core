#ifndef HINTBOXDIALOG_H
#define HINTBOXDIALOG_H

#include <QDialog>
#include <QStackedWidget>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QWidget>
#include <QPointer>

namespace Ui {
class HintboxDialog;
}

class HintboxDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HintboxDialog(QWidget *parent = nullptr);
    ~HintboxDialog();

// Method
public:
    void setTitle(const QString& title);
    void addNewPage(QString imgPath, QString text);
    void removePageAtIndex(int index);

    void applyStylesheet_dark();

private:
    void setupWidget();
    void setupSignals();

    void updatePaginationCircles();

// Navigations
private slots:
    void gotoNextPage();
    void gotoPreviousPage();

private:
    Ui::HintboxDialog *ui;

    // From the .ui
    QPointer<QStackedWidget> m_hitboxStackedWidget;
    QPointer<QLabel> m_labelHintboxTitle;

    QPointer<QPushButton> m_pushButtonPrevious;
    QPointer<QWidget> m_paginationCircles;
    QPointer<QPushButton> m_pushButtonNext;

    QPointer<QDialogButtonBox> m_buttonBox;
};

#endif // HINTBOXDIALOG_H
