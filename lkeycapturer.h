#ifndef LKEYCAPTURER_H
#define LKEYCAPTURER_H

#include <QObject>
#include <windows.h>

class LKeyCapturer : public QObject
{
    Q_OBJECT

public:
    static LKeyCapturer* getInstance();
    static HHOOK m_keyHook;
    ~LKeyCapturer();

private:
    explicit LKeyCapturer(QObject *parent = nullptr);
    static LKeyCapturer* m_kCapturer;

signals:
    void KeyPressed(int key);
    void KeyRelease(int key);
};

#endif // LKEYCAPTURER_H
