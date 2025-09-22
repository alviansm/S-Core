#ifndef VOYAGEITEMWIDGET_H
#define VOYAGEITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class VoyageItemWidget : public QWidget {
    Q_OBJECT
public:
    explicit VoyageItemWidget(const QString &voyageName,
                              const QString &from,
                              const QString &to,
                              const QString &etd,
                              const QString &eta,
                              QWidget *parent = nullptr);

    QSize sizeHint() const override;
private:
    QLabel *m_voyageName;
    QLabel *m_fromTo;
    QLabel *m_etd;
    QLabel *m_eta;
};

#endif // VOYAGEITEMWIDGET_H
