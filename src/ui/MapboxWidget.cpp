#include "mapboxwidget.h"
#include <QWebEngineView>
#include <QVBoxLayout>

MapboxWidget::MapboxWidget(QWidget *parent)
    : QWidget(parent)
{
    // Buat instance QWebEngineView
    m_view = new QWebEngineView(this);

    // Muat file HTML dari Qt Resource System
    m_view->setUrl(QUrl("qrc:/html/map.html"));

    // Atur layout agar QWebEngineView mengisi seluruh widget
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_view);
    layout->setContentsMargins(0, 0, 0, 0); // Hapus margin agar peta penuh
    setLayout(layout);
}

MapboxWidget::~MapboxWidget()
{
}
