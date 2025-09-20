#ifndef CIRCLEPROGRESSBAR_H
#define CIRCLEPROGRESSBAR_H

#include <QWidget>
#include <QColor>

class CircleProgressBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)

public:
    explicit CircleProgressBar(QWidget *parent = nullptr);

    int value() const { return m_value; }
    void setValue(int value);
    void setMaximum(int max);
    void setText(const QString &text);

signals:
    void valueChanged(int newValue);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_value;
    int m_max;
    QString m_text;
    QColor m_startColor;
    QColor m_endColor;
};

#endif // CIRCLEPROGRESSBAR_H
