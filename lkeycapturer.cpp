#include "lkeycapturer.h"

LKeyCapturer* LKeyCapturer::m_kCapturer = nullptr;
HHOOK LKeyCapturer::m_keyHook = NULL;

LKeyCapturer *LKeyCapturer::getInstance()
{
    if(!m_kCapturer){
        m_kCapturer = new LKeyCapturer();
    }

    return m_kCapturer;
}

LKeyCapturer::~LKeyCapturer()
{
    if(m_keyHook){
        UnhookWindowsHookEx(m_keyHook);
    }
}

LRESULT keyHookEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(nCode < 0) return CallNextHookEx(LKeyCapturer::m_keyHook, nCode, wParam, lParam);
    if(wParam == WM_KEYDOWN && nCode == HC_ACTION){
        KBDLLHOOKSTRUCT *KeyInfo = (KBDLLHOOKSTRUCT *)lParam;
        LKeyCapturer::getInstance()->KeyPressed(KeyInfo->vkCode);
    }
    else if(wParam == WM_KEYUP && nCode == HC_ACTION){
        KBDLLHOOKSTRUCT *KeyInfo = (KBDLLHOOKSTRUCT *)lParam;
        LKeyCapturer::getInstance()->KeyRelease(KeyInfo->vkCode);
    }

    return CallNextHookEx(LKeyCapturer::m_keyHook, nCode, wParam, lParam);
}

LKeyCapturer::LKeyCapturer(QObject *parent) : QObject(parent)
{
    m_keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)keyHookEvent, GetModuleHandle(0), 0);
}
