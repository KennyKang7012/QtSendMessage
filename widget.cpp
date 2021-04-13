#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <windows.h>

#include <WtsApi32.h>
#pragma comment(lib, "Wtsapi32.lib")

#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif

const ULONG_PTR CUSTOM_TYPE = 10000;
const QString c_strTitle = "ReceiveMessage";

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("SendMessage");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onSendMessage()
{
    HWND hwnd = NULL;
    do
    {
       LPWSTR path = (LPWSTR)c_strTitle.utf16();    //path = L"SendMessage"
       hwnd = ::FindWindowW(NULL, path);
    } while (hwnd == (HWND)effectiveWinId());     // 忽略自己

    if (::IsWindow(hwnd))
    {
        qDebug() << "IsWindow";
        QString filename = QStringLiteral("IPC-Windows Message");
        QByteArray data = filename.toUtf8();

        COPYDATASTRUCT copydata;
        copydata.dwData = CUSTOM_TYPE;  // 用戶定義數據
        copydata.lpData = data.data();  // 數據大小
        copydata.cbData = data.size();  // 指向數據的指針

        HWND sender = (HWND)effectiveWinId();

        ::SendMessage(hwnd, WM_COPYDATA, reinterpret_cast<WPARAM>(sender), reinterpret_cast<LPARAM>(&copydata));
    }
}

void Widget::on_pushButton_clicked()
{
        onSendMessage();
}
