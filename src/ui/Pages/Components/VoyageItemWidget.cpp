#include "VoyageItemWidget.h"

VoyageItemWidget::VoyageItemWidget(const QString &voyageName,
                                   const QString &from,
                                   const QString &to,
                                   const QString &etd,
                                   const QString &eta,
                                   QWidget *parent)
    : QWidget(parent)
{
    // Title
    m_voyageName = new QLabel(voyageName);
    m_voyageName->setStyleSheet("font-weight: bold; font-size: 14px; background-color: transparent; color: white;");
    m_voyageName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    // Route
    m_fromTo = new QLabel(from + "  →  " + to);
    m_fromTo->setStyleSheet("color: #ddd; font-size: 12px; background-color: transparent;");
    m_fromTo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    // ETD & ETA
    m_etd = new QLabel("ETD : " + etd);
    m_eta = new QLabel("ETA : " + eta);

    m_etd->setStyleSheet("font-size: 12px; color: #aaa; background-color: transparent;");
    m_eta->setStyleSheet("font-size: 12px; color: #aaa; background-color: transparent;");

    m_etd->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_eta->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    // Layout kiri (title + route)
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(m_voyageName);
    leftLayout->addWidget(m_fromTo);
    leftLayout->setSpacing(2);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    // Layout kanan (etd/eta)
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(m_etd);
    rightLayout->addWidget(m_eta);
    rightLayout->setAlignment(Qt::AlignRight);
    rightLayout->setSpacing(2);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    mainLayout->setContentsMargins(10, 8, 10, 8);
    mainLayout->setSpacing(10);

    setLayout(mainLayout);

    // Styling background & border
    setStyleSheet(R"(
        QWidget {
            background-color: #2b2b2b;
            border-radius: 6px;
        }
        QWidget:hover {
            background-color: #333;
        }
    )");
}

// ✅ Pastikan tinggi item seragam
QSize VoyageItemWidget::sizeHint() const {
    return QSize(200, 60); // lebar minimal 200px, tinggi 60px
}
