#include "mapboxwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWebEngineView>
#include <QFrame>

MapboxWidget::MapboxWidget(QWidget *parent)
    : QWidget(parent)
{
    // Web map
    m_view = new QWebEngineView(this);
    m_view->setUrl(QUrl("qrc:/html/map.html"));

    // SOLUSI: Gunakan layout sederhana tanpa QStackedLayout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_view);

    setLayout(mainLayout);

    // // Posisikan weather box menggunakan parent-child relationship
    // m_weatherBox->setParent(this);
    // m_weatherBox->raise(); // Pastikan di atas
}

void MapboxWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (m_weatherBox) {
        m_weatherBox->move(width() - m_weatherBox->width() - 20, 20);
    }
}

MapboxWidget::~MapboxWidget()
{
}
