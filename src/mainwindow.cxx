#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hardware/c-disk-drive.hpp"


#include "hardware/models/c-apple2e-hardware.hpp"
#include "hardware/cards/c-disk-controler-card.hpp"

#include "gui/c-gui-monitor.hpp"

#include  <QKeyEvent>

#ifdef Q_OS_WIN32 // MS Windows version
#include <Windows.h>
#include <WinUser.h>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    buildComputer();

    oaDown = false;
    saDown = false;

    grabKeyboard();
   // startTimer(33);
    ui->setupUi(this);
    connect(ui->m_start_button, &QPushButton::clicked,
            this, &MainWindow::startPc);
    connect(ui->m_stop_button, &QPushButton::clicked,
            this, &MainWindow::stopPc);
    connect(ui->m_reset_button, &QPushButton::clicked,
            this, &MainWindow::resetPc);


    auto gl = new  QGridLayout();
    gl->setContentsMargins(0,0,0,0);
    ui->m_monitor_frame->setLayout(gl);


    auto w = new CGuiMonitor(m_system->bwvideo);
    ui->m_monitor_frame->setMinimumSize(w->width(),w->height());

    ui->m_monitor_frame->layout()->addWidget(w);

    connect(this,&MainWindow::saveImage,
            w, &CGuiMonitor::saveBuffer);
}

void MainWindow::buildComputer()
{

    CRomMemory *mainRom   = new CRomMemory(0x3000);
    CRomMemory *internalRom  = new CRomMemory(0x1000);
    CRomMemory *diskRom = new CRomMemory(0x0100);

//   mainRom->loadFromFile(":Apple2e_Enhanced.rom",0x1000);
//    internalRom->loadFromFile(":Apple2e_Enhanced.rom",0x0000);
//    diskRom->loadFromFile(":DISK2.rom",0x0000);
    mainRom->loadFromFile(  ":APPLE2E.ROM",0x5000);
    internalRom->loadFromFile( ":APPLE2E.ROM",0x4000);
    diskRom->loadFromFile(":APPLE2E.ROM",0x0600);

    // Build an Apple //e
    m_system = new CApple2eHardware(mainRom, internalRom);
    CDiskControlerCard *controler = new CDiskControlerCard(diskRom);
    m_system->insertCard(6, controler);
    CDiskDrive *drive1 = new CDiskDrive(m_system->get_clock(), controler->get_interface(0));
    CDiskDrive *drive2 = new CDiskDrive(m_system->get_clock(), controler->get_interface(1));
//    CFloppyDisk diskette("C:/develop/git/AppleSaws/disk-images/dos.3.3.system.master.dsk");
    CFloppyDisk diskette("C:/apple/Disks/Apple Disks/test2.dsk");
    drive1->insert(&diskette);

    QObject::connect(this, &MainWindow::startPc,
                     m_system,&CApple2eHardware::start);
    QObject::connect(this, &MainWindow::stopPc,
                     m_system,&CApple2eHardware::stop);
    QObject::connect(this, &MainWindow::resetPc,
                     m_system,&CApple2eHardware::reset);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent * /*event*/)
{
  //  qDebug("Update timer");
 //   update();
}

bool MainWindow::event(QEvent *ev)
{
    bool retval = false;
    if (ev->type() == QEvent::KeyPress)
    {
        QKeyEvent *e = (QKeyEvent *) ev;
        keyPressEvent(e);
        retval = true;
    }
    else
    {
        retval = QMainWindow::event(ev);
    }
    return retval;
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
 //       qDebug() << "Keypress" << hex << ev->key() << ev->modifiers();
    quint8 ap = 0;

    static bool caps = true;

    bool keyfound = true;
    bool shift = ev->modifiers().testFlag(Qt::ShiftModifier);
    bool ctrl = ev->modifiers().testFlag(Qt::ControlModifier);

    switch (ev->key())
    {
    case Qt::Key_CapsLock:
// platform dependent method of determining if CAPS LOCK is on
#ifdef Q_OS_WIN32 // MS Windows version
        caps = (GetKeyState(VK_CAPITAL) & 1);
        keyfound = false;
        qDebug("Setting caps to %d",caps);
#else // X11 version (Linux/Unix/Mac OS X/etc...)
            Display * d = XOpenDisplay((char*)0);
            bool caps_state = false;
            if (d)
            {
                unsigned n;
                XkbGetIndicatorState(d, XkbUseCoreKbd, &n);
                caps_state = (n & 0x01) == 1;
            }
            caps = caps_state;
#endif
        break;
    case Qt::Key_Delete:
        ap = 0x7F;
        break;
    case Qt::Key_Backspace:
        if (shift)
        {
            ap = 0x7F;
        }
        else
        {
            ap = 0x08;
        }
        break;
    case Qt::Key_Left:
        ap = 0x08;
        break;
    case Qt::Key_Tab:
    case Qt::Key_Backtab:
        ap = 0x09;
        break;
    case Qt::Key_Down:
        ap = 0x0A;
        break;
    case Qt::Key_Up:
        ap = 0x0B;
        break;
    case Qt::Key_Return:
        ap = 0x0D;
        break;
    case Qt::Key_Right:
        ap = 0x15;
        break;
    case Qt::Key_Escape:
        ap = 0x1B;
        break;
    case Qt::Key_Space:
        ap = 0x20;
        break;
    case Qt::Key_Apostrophe:
        ap = 0x27;
        break;
    case Qt::Key_QuoteDbl:
        ap = 0x22;
        break;
    case Qt::Key_Comma:
        ap = 0x2C;
        break;
    case Qt::Key_Greater:
        ap = 0x3C;
        break;
    case Qt::Key_Minus:
        ap = ctrl ? 0x1f : 0x2D;
        break;
    case Qt::Key_Underscore:
        ap = ctrl ? 0x1f : 0x5F;
        break;
    case Qt::Key_Period:
        ap = 0x2E;
        break;
    case Qt::Key_Less:
        ap = 0x3E;
        break;
    case Qt::Key_Slash:
        ap = 0x2F;
        break;
    case Qt::Key_Question:
        ap = 0x3F;
        break;
    case Qt::Key_0:
        ap = 0x30;
        break;
    case Qt::Key_ParenRight:
        ap = 0x29;
        break;
    case Qt::Key_1:
        ap = 0x31;
        break;
    case Qt::Key_Exclam:
        ap = 0x21;
        break;
    case Qt::Key_2:
        ap = ctrl ? 0x00 : 0x32;
        break;
    case Qt::Key_At:
        ap = ctrl ? 0x00 : 0x40;
        break;
    case Qt::Key_3:
        ap = 0x33;
        break;
    case Qt::Key_NumberSign:
        ap = 0x23;
        break;
    case Qt::Key_4:
        ap = 0x34;
        break;
    case Qt::Key_Dollar:
        ap = 0x24;
        break;
    case Qt::Key_5:
        ap = 0x35;
        break;
    case Qt::Key_Percent:
        ap = 0x25;
        break;
    case Qt::Key_6:
        ap = ctrl ? 0x1e : 0x36;
        break;
    case Qt::Key_AsciiCircum:
        ap = ctrl ? 0x1e : 0x5E;
        break;
    case Qt::Key_7:
        ap = 0x37;
        break;
    case Qt::Key_Ampersand:
        ap = 0x26;
        break;
    case Qt::Key_8:
        ap = 0x38;
        break;
    case Qt::Key_Asterisk:
        ap = 0x2A;
        break;
    case Qt::Key_9:
        ap = 0x39;
        break;
    case Qt::Key_ParenLeft:
        ap = 0x28;
        break;
    case Qt::Key_Semicolon:
        ap = 0x3B;
        break;
    case Qt::Key_Colon:
        ap = 0x3A;
        break;
    case Qt::Key_Equal:
        ap = 0x3D;
        break;
    case Qt::Key_Plus:
        ap = 0x2B;
        break;
    case Qt::Key_BracketLeft:
        ap = ctrl ? 0x1b : 0x5B;
        break;
    case Qt::Key_BraceLeft:
        ap = ctrl ? 0x1b : 0x7B;
        break;
    case Qt::Key_Backslash:
        ap = ctrl ? 0x1c : 0x5C;
        break;
    case Qt::Key_Bar:
        ap = ctrl ? 0x1c : 0x7C;
        break;
    case Qt::Key_BracketRight:
        ap = ctrl ? 0x1d : 0x5D;
        break;
    case Qt::Key_BraceRight:
        ap = ctrl ? 0x1d : 0x7D;
        break;
    case Qt::Key_QuoteLeft:
        ap = 0x60;
        break;
    case Qt::Key_AsciiTilde:
        ap = 0x7E;
        break;
    case Qt::Key_A:
        ap = ctrl ? 0x01 : (shift||caps) ? 0x41 : 0x61;
        break;
    case Qt::Key_B:
        ap = ctrl ? 0x02 : (shift||caps) ? 0x42 : 0x62;
        break;
    case Qt::Key_C:
        ap = ctrl ? 0x03 : (shift||caps) ? 0x43 : 0x63;
        break;
    case Qt::Key_D:
        ap = ctrl ? 0x04 : (shift||caps) ? 0x44 : 0x64;
        break;
    case Qt::Key_E:
        ap = ctrl ? 0x05 : (shift||caps) ? 0x45 : 0x65;
        break;
    case Qt::Key_F:
        ap = ctrl ? 0x06 : (shift||caps) ? 0x46 : 0x66;
        break;
    case Qt::Key_G:
        ap = ctrl ? 0x07 : (shift||caps) ? 0x47 : 0x67;
        break;
    case Qt::Key_H:
        ap = ctrl ? 0x08 : (shift||caps) ? 0x48 : 0x68;
        break;
    case Qt::Key_I:
        ap = ctrl ? 0x09 : (shift||caps) ? 0x49 : 0x69;
        break;
    case Qt::Key_J:
        ap = ctrl ? 0x0A : (shift||caps) ? 0x4a : 0x6A;
        break;
    case Qt::Key_K:
        ap = ctrl ? 0x0B : (shift||caps) ? 0x4B : 0x6B;
        break;
    case Qt::Key_L:
        ap = ctrl ? 0x0C : (shift||caps) ? 0x4C : 0x6C;
        break;
    case Qt::Key_M:
        ap = ctrl ? 0x0D : (shift||caps) ? 0x4D : 0x6D;
        break;
    case Qt::Key_N:
        ap = ctrl ? 0x0E : (shift||caps) ? 0x4e : 0x6E;
        break;
    case Qt::Key_O:
        ap = ctrl ? 0x0F : (shift||caps) ? 0x4F : 0x6F;
        break;
    case Qt::Key_P:
        ap = ctrl ? 0x10 : (shift||caps) ? 0x50 : 0x70;
        break;
    case Qt::Key_Q:
        ap = ctrl ? 0x11 : (shift||caps) ? 0x51 : 0x71;
        break;
    case Qt::Key_R:
        ap = ctrl ? 0x12 : (shift||caps) ? 0x52 : 0x72;
        break;
    case Qt::Key_S:
        ap = ctrl ? 0x13 : (shift||caps) ? 0x53 : 0x73;
        break;
    case Qt::Key_T:
        ap = ctrl ? 0x14 : (shift||caps) ? 0x54 : 0x74;
        break;
    case Qt::Key_U:
        ap = ctrl ? 0x15 : (shift||caps) ? 0x55 : 0x75;
        break;
    case Qt::Key_V:
        ap = ctrl ? 0x16 : (shift||caps) ? 0x56 : 0x76;
        break;
    case Qt::Key_W:
        ap = ctrl ? 0x17 : (shift||caps) ? 0x57 : 0x77;
        break;
    case Qt::Key_X:
        ap = ctrl ? 0x18 : (shift||caps) ? 0x58 : 0x78;
        break;
    case Qt::Key_Y:
        ap = ctrl ? 0x19 : (shift||caps) ? 0x59 : 0x79;
        break;
    case Qt::Key_Z:
        ap = ctrl ? 0x1a : (shift||caps) ? 0x5a : 0x7A;
        break;
    default:
        keyfound = false;
        if (!ev->isAutoRepeat())
        {
            if (ev->key() == Qt::Key_F1)
            {
                if (!oaDown)
                {
                    setOAActive(true);
                }
            }
            else if (ev->key() == Qt::Key_F2)
            {
                if (!saDown)
                {
                    setSAActive(true);
                }
            }
            else if (ev->key() == Qt::Key_F12 && ctrl)
            {
                emit resetPc();
            }
            else if (ev->key() == Qt::Key_F12)
            {
                emit startPc();
            }
            else if (ev->key() == Qt::Key_F5)
            {
                emit saveImage();
            }
        }
        break;
    }


    if (keyfound)
    {
        m_system->getKeyboard().press_key(ap);
     //   emit keyDown(ap);
    }
}

void MainWindow::setOAActive(int active)
{
  //  ui->m_openAppleBox->setChecked(active);
    if (active)
    {
        if (!oaDown)
        {
            oaDown=true;
            m_system->getKeyboard().press_open_apple();

//            emit openAppleKey(true);
        }
    }
    else
    {
        if (oaDown)
        {
            oaDown=false;
 //           emit openAppleKey(false);
            m_system->getKeyboard().release_open_apple();

        }
    }
}

void MainWindow::setSAActive(int active)
{
 //   ui->m_solidAppleBox->setChecked(active);
    if (active)
    {
        if (!saDown)
        {
            saDown=true;
            m_system->getKeyboard().press_closed_apple();

 //           emit solidAppleKey(true);
        }
    }
    else
    {
        if (saDown)
        {
            saDown=false;
            m_system->getKeyboard().release_closed_apple();

   //         emit solidAppleKey(false);
        }
    }

}

void MainWindow::keyReleaseEvent(QKeyEvent *ev)
{
    if (ev->isAutoRepeat()) return;
    //    qDebug() << "Key release";
    if (ev->key() == Qt::Key_F1)
    {
        if (oaDown)
        {
            setOAActive(false);
        }
    }
    else if (ev->key() == Qt::Key_F2)
    {
        if (saDown)
        {
            setSAActive(false);
        }
    }
}

