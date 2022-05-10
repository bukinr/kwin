/*
    KWin - the KDE window manager
    This file is part of the KDE project.

    SPDX-FileCopyrightText: 2019 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kwin_wayland_test.h"

#include "cursor.h"
#include "output.h"
#include "platform.h"
#include "wayland_server.h"
#include "window.h"
#include "workspace.h"

#include <KWayland/Client/surface.h>

namespace KWin
{

static const QString s_socketName = QStringLiteral("wayland_test_activation-0");

class ActivationTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanup();

    void testSwitchToWindowToLeft();
    void testSwitchToWindowToRight();
    void testSwitchToWindowAbove();
    void testSwitchToWindowBelow();
    void testSwitchToWindowMaximized();
    void testSwitchToWindowFullScreen();

private:
    void stackScreensHorizontally();
    void stackScreensVertically();
};

void ActivationTest::initTestCase()
{
    qRegisterMetaType<Window *>();

    QSignalSpy applicationStartedSpy(kwinApp(), &Application::started);
    QVERIFY(applicationStartedSpy.isValid());
    kwinApp()->platform()->setInitialWindowSize(QSize(1280, 1024));
    QVERIFY(waylandServer()->init(s_socketName));
    QMetaObject::invokeMethod(kwinApp()->platform(), "setVirtualOutputs", Qt::DirectConnection, Q_ARG(int, 2));

    kwinApp()->start();
    QVERIFY(applicationStartedSpy.wait());
    const auto outputs = kwinApp()->platform()->enabledOutputs();
    QCOMPARE(outputs.count(), 2);
    QCOMPARE(outputs[0]->geometry(), QRect(0, 0, 1280, 1024));
    QCOMPARE(outputs[1]->geometry(), QRect(1280, 0, 1280, 1024));
    Test::initWaylandWorkspace();
}

void ActivationTest::init()
{
    QVERIFY(Test::setupWaylandConnection());

    workspace()->setActiveOutput(QPoint(640, 512));
    Cursors::self()->mouse()->setPos(QPoint(640, 512));
}

void ActivationTest::cleanup()
{
    Test::destroyWaylandConnection();

    stackScreensHorizontally();
}

void ActivationTest::testSwitchToWindowToLeft()
{
    // This test verifies that "Switch to Window to the Left" shortcut works.

    using namespace KWayland::Client;

    // Prepare the test environment.
    stackScreensHorizontally();

    // Create several windows on the left screen.
    QScopedPointer<KWayland::Client::Surface> surface1(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface1(Test::createXdgToplevelSurface(surface1.data()));
    Window *window1 = Test::renderAndWaitForShown(surface1.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window1);
    QVERIFY(window1->isActive());

    QScopedPointer<KWayland::Client::Surface> surface2(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface2(Test::createXdgToplevelSurface(surface2.data()));
    Window *window2 = Test::renderAndWaitForShown(surface2.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window2);
    QVERIFY(window2->isActive());

    window1->move(QPoint(300, 200));
    window2->move(QPoint(500, 200));

    // Create several windows on the right screen.
    QScopedPointer<KWayland::Client::Surface> surface3(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface3(Test::createXdgToplevelSurface(surface3.data()));
    Window *window3 = Test::renderAndWaitForShown(surface3.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window3);
    QVERIFY(window3->isActive());

    QScopedPointer<KWayland::Client::Surface> surface4(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface4(Test::createXdgToplevelSurface(surface4.data()));
    Window *window4 = Test::renderAndWaitForShown(surface4.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window4);
    QVERIFY(window4->isActive());

    window3->move(QPoint(1380, 200));
    window4->move(QPoint(1580, 200));

    // Switch to window to the left.
    workspace()->switchWindow(Workspace::DirectionWest);
    QVERIFY(window3->isActive());

    // Switch to window to the left.
    workspace()->switchWindow(Workspace::DirectionWest);
    QVERIFY(window2->isActive());

    // Switch to window to the left.
    workspace()->switchWindow(Workspace::DirectionWest);
    QVERIFY(window1->isActive());

    // Switch to window to the left.
    workspace()->switchWindow(Workspace::DirectionWest);
    QVERIFY(window4->isActive());

    // Destroy all windows.
    shellSurface1.reset();
    QVERIFY(Test::waitForWindowDestroyed(window1));
    shellSurface2.reset();
    QVERIFY(Test::waitForWindowDestroyed(window2));
    shellSurface3.reset();
    QVERIFY(Test::waitForWindowDestroyed(window3));
    shellSurface4.reset();
    QVERIFY(Test::waitForWindowDestroyed(window4));
}

void ActivationTest::testSwitchToWindowToRight()
{
    // This test verifies that "Switch to Window to the Right" shortcut works.

    using namespace KWayland::Client;

    // Prepare the test environment.
    stackScreensHorizontally();

    // Create several windows on the left screen.
    QScopedPointer<KWayland::Client::Surface> surface1(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface1(Test::createXdgToplevelSurface(surface1.data()));
    Window *window1 = Test::renderAndWaitForShown(surface1.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window1);
    QVERIFY(window1->isActive());

    QScopedPointer<KWayland::Client::Surface> surface2(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface2(Test::createXdgToplevelSurface(surface2.data()));
    Window *window2 = Test::renderAndWaitForShown(surface2.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window2);
    QVERIFY(window2->isActive());

    window1->move(QPoint(300, 200));
    window2->move(QPoint(500, 200));

    // Create several windows on the right screen.
    QScopedPointer<KWayland::Client::Surface> surface3(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface3(Test::createXdgToplevelSurface(surface3.data()));
    Window *window3 = Test::renderAndWaitForShown(surface3.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window3);
    QVERIFY(window3->isActive());

    QScopedPointer<KWayland::Client::Surface> surface4(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface4(Test::createXdgToplevelSurface(surface4.data()));
    Window *window4 = Test::renderAndWaitForShown(surface4.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window4);
    QVERIFY(window4->isActive());

    window3->move(QPoint(1380, 200));
    window4->move(QPoint(1580, 200));

    // Switch to window to the right.
    workspace()->switchWindow(Workspace::DirectionEast);
    QVERIFY(window1->isActive());

    // Switch to window to the right.
    workspace()->switchWindow(Workspace::DirectionEast);
    QVERIFY(window2->isActive());

    // Switch to window to the right.
    workspace()->switchWindow(Workspace::DirectionEast);
    QVERIFY(window3->isActive());

    // Switch to window to the right.
    workspace()->switchWindow(Workspace::DirectionEast);
    QVERIFY(window4->isActive());

    // Destroy all windows.
    shellSurface1.reset();
    QVERIFY(Test::waitForWindowDestroyed(window1));
    shellSurface2.reset();
    QVERIFY(Test::waitForWindowDestroyed(window2));
    shellSurface3.reset();
    QVERIFY(Test::waitForWindowDestroyed(window3));
    shellSurface4.reset();
    QVERIFY(Test::waitForWindowDestroyed(window4));
}

void ActivationTest::testSwitchToWindowAbove()
{
    // This test verifies that "Switch to Window Above" shortcut works.

    using namespace KWayland::Client;

    // Prepare the test environment.
    stackScreensVertically();

    // Create several windows on the top screen.
    QScopedPointer<KWayland::Client::Surface> surface1(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface1(Test::createXdgToplevelSurface(surface1.data()));
    Window *window1 = Test::renderAndWaitForShown(surface1.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window1);
    QVERIFY(window1->isActive());

    QScopedPointer<KWayland::Client::Surface> surface2(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface2(Test::createXdgToplevelSurface(surface2.data()));
    Window *window2 = Test::renderAndWaitForShown(surface2.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window2);
    QVERIFY(window2->isActive());

    window1->move(QPoint(200, 300));
    window2->move(QPoint(200, 500));

    // Create several windows on the bottom screen.
    QScopedPointer<KWayland::Client::Surface> surface3(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface3(Test::createXdgToplevelSurface(surface3.data()));
    Window *window3 = Test::renderAndWaitForShown(surface3.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window3);
    QVERIFY(window3->isActive());

    QScopedPointer<KWayland::Client::Surface> surface4(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface4(Test::createXdgToplevelSurface(surface4.data()));
    Window *window4 = Test::renderAndWaitForShown(surface4.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window4);
    QVERIFY(window4->isActive());

    window3->move(QPoint(200, 1224));
    window4->move(QPoint(200, 1424));

    // Switch to window above.
    workspace()->switchWindow(Workspace::DirectionNorth);
    QVERIFY(window3->isActive());

    // Switch to window above.
    workspace()->switchWindow(Workspace::DirectionNorth);
    QVERIFY(window2->isActive());

    // Switch to window above.
    workspace()->switchWindow(Workspace::DirectionNorth);
    QVERIFY(window1->isActive());

    // Switch to window above.
    workspace()->switchWindow(Workspace::DirectionNorth);
    QVERIFY(window4->isActive());

    // Destroy all windows.
    shellSurface1.reset();
    QVERIFY(Test::waitForWindowDestroyed(window1));
    shellSurface2.reset();
    QVERIFY(Test::waitForWindowDestroyed(window2));
    shellSurface3.reset();
    QVERIFY(Test::waitForWindowDestroyed(window3));
    shellSurface4.reset();
    QVERIFY(Test::waitForWindowDestroyed(window4));
}

void ActivationTest::testSwitchToWindowBelow()
{
    // This test verifies that "Switch to Window Bottom" shortcut works.

    using namespace KWayland::Client;

    // Prepare the test environment.
    stackScreensVertically();

    // Create several windows on the top screen.
    QScopedPointer<KWayland::Client::Surface> surface1(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface1(Test::createXdgToplevelSurface(surface1.data()));
    Window *window1 = Test::renderAndWaitForShown(surface1.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window1);
    QVERIFY(window1->isActive());

    QScopedPointer<KWayland::Client::Surface> surface2(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface2(Test::createXdgToplevelSurface(surface2.data()));
    Window *window2 = Test::renderAndWaitForShown(surface2.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window2);
    QVERIFY(window2->isActive());

    window1->move(QPoint(200, 300));
    window2->move(QPoint(200, 500));

    // Create several windows on the bottom screen.
    QScopedPointer<KWayland::Client::Surface> surface3(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface3(Test::createXdgToplevelSurface(surface3.data()));
    Window *window3 = Test::renderAndWaitForShown(surface3.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window3);
    QVERIFY(window3->isActive());

    QScopedPointer<KWayland::Client::Surface> surface4(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface4(Test::createXdgToplevelSurface(surface4.data()));
    Window *window4 = Test::renderAndWaitForShown(surface4.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window4);
    QVERIFY(window4->isActive());

    window3->move(QPoint(200, 1224));
    window4->move(QPoint(200, 1424));

    // Switch to window below.
    workspace()->switchWindow(Workspace::DirectionSouth);
    QVERIFY(window1->isActive());

    // Switch to window below.
    workspace()->switchWindow(Workspace::DirectionSouth);
    QVERIFY(window2->isActive());

    // Switch to window below.
    workspace()->switchWindow(Workspace::DirectionSouth);
    QVERIFY(window3->isActive());

    // Switch to window below.
    workspace()->switchWindow(Workspace::DirectionSouth);
    QVERIFY(window4->isActive());

    // Destroy all windows.
    shellSurface1.reset();
    QVERIFY(Test::waitForWindowDestroyed(window1));
    shellSurface2.reset();
    QVERIFY(Test::waitForWindowDestroyed(window2));
    shellSurface3.reset();
    QVERIFY(Test::waitForWindowDestroyed(window3));
    shellSurface4.reset();
    QVERIFY(Test::waitForWindowDestroyed(window4));
}

void ActivationTest::testSwitchToWindowMaximized()
{
    // This test verifies that we switch to the top-most maximized window, i.e.
    // the one that user sees at the moment. See bug 411356.

    using namespace KWayland::Client;

    // Prepare the test environment.
    stackScreensHorizontally();

    // Create several maximized windows on the left screen.
    QScopedPointer<KWayland::Client::Surface> surface1(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface1(Test::createXdgToplevelSurface(surface1.data()));
    QSignalSpy toplevelConfigureRequestedSpy1(shellSurface1.data(), &Test::XdgToplevel::configureRequested);
    QSignalSpy surfaceConfigureRequestedSpy1(shellSurface1->xdgSurface(), &Test::XdgSurface::configureRequested);
    Window *window1 = Test::renderAndWaitForShown(surface1.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window1);
    QVERIFY(window1->isActive());
    QVERIFY(surfaceConfigureRequestedSpy1.wait()); // Wait for the configure event with the activated state.
    workspace()->slotWindowMaximize();
    QVERIFY(surfaceConfigureRequestedSpy1.wait());
    QSignalSpy frameGeometryChangedSpy1(window1, &Window::frameGeometryChanged);
    QVERIFY(frameGeometryChangedSpy1.isValid());
    shellSurface1->xdgSurface()->ack_configure(surfaceConfigureRequestedSpy1.last().at(0).value<quint32>());
    Test::render(surface1.data(), toplevelConfigureRequestedSpy1.last().at(0).toSize(), Qt::red);
    QVERIFY(frameGeometryChangedSpy1.wait());

    QScopedPointer<KWayland::Client::Surface> surface2(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface2(Test::createXdgToplevelSurface(surface2.data()));
    QSignalSpy toplevelConfigureRequestedSpy2(shellSurface2.data(), &Test::XdgToplevel::configureRequested);
    QSignalSpy surfaceConfigureRequestedSpy2(shellSurface2->xdgSurface(), &Test::XdgSurface::configureRequested);
    Window *window2 = Test::renderAndWaitForShown(surface2.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window2);
    QVERIFY(window2->isActive());
    QVERIFY(surfaceConfigureRequestedSpy2.wait()); // Wait for the configure event with the activated state.
    workspace()->slotWindowMaximize();
    QVERIFY(surfaceConfigureRequestedSpy2.wait());
    QSignalSpy frameGeometryChangedSpy2(window2, &Window::frameGeometryChanged);
    QVERIFY(frameGeometryChangedSpy2.isValid());
    shellSurface2->xdgSurface()->ack_configure(surfaceConfigureRequestedSpy2.last().at(0).value<quint32>());
    Test::render(surface2.data(), toplevelConfigureRequestedSpy2.last().at(0).toSize(), Qt::red);
    QVERIFY(frameGeometryChangedSpy2.wait());

    const QList<Window *> stackingOrder = workspace()->stackingOrder();
    QVERIFY(stackingOrder.indexOf(window1) < stackingOrder.indexOf(window2));
    QCOMPARE(window1->maximizeMode(), MaximizeFull);
    QCOMPARE(window2->maximizeMode(), MaximizeFull);

    // Create several windows on the right screen.
    QScopedPointer<KWayland::Client::Surface> surface3(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface3(Test::createXdgToplevelSurface(surface3.data()));
    Window *window3 = Test::renderAndWaitForShown(surface3.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window3);
    QVERIFY(window3->isActive());

    QScopedPointer<KWayland::Client::Surface> surface4(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface4(Test::createXdgToplevelSurface(surface4.data()));
    Window *window4 = Test::renderAndWaitForShown(surface4.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window4);
    QVERIFY(window4->isActive());

    window3->move(QPoint(1380, 200));
    window4->move(QPoint(1580, 200));

    // Switch to window to the left.
    workspace()->switchWindow(Workspace::DirectionWest);
    QVERIFY(window3->isActive());

    // Switch to window to the left.
    workspace()->switchWindow(Workspace::DirectionWest);
    QVERIFY(window2->isActive());

    // Switch to window to the left.
    workspace()->switchWindow(Workspace::DirectionWest);
    QVERIFY(window4->isActive());

    // Destroy all windows.
    shellSurface1.reset();
    QVERIFY(Test::waitForWindowDestroyed(window1));
    shellSurface2.reset();
    QVERIFY(Test::waitForWindowDestroyed(window2));
    shellSurface3.reset();
    QVERIFY(Test::waitForWindowDestroyed(window3));
    shellSurface4.reset();
    QVERIFY(Test::waitForWindowDestroyed(window4));
}

void ActivationTest::testSwitchToWindowFullScreen()
{
    // This test verifies that we switch to the top-most fullscreen window, i.e.
    // the one that user sees at the moment. See bug 411356.

    using namespace KWayland::Client;

    // Prepare the test environment.
    stackScreensVertically();

    // Create several maximized windows on the top screen.
    QScopedPointer<KWayland::Client::Surface> surface1(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface1(Test::createXdgToplevelSurface(surface1.data()));
    QSignalSpy toplevelConfigureRequestedSpy1(shellSurface1.data(), &Test::XdgToplevel::configureRequested);
    QSignalSpy surfaceConfigureRequestedSpy1(shellSurface1->xdgSurface(), &Test::XdgSurface::configureRequested);
    Window *window1 = Test::renderAndWaitForShown(surface1.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window1);
    QVERIFY(window1->isActive());
    QVERIFY(surfaceConfigureRequestedSpy1.wait()); // Wait for the configure event with the activated state.
    workspace()->slotWindowFullScreen();
    QVERIFY(surfaceConfigureRequestedSpy1.wait());
    QSignalSpy frameGeometryChangedSpy1(window1, &Window::frameGeometryChanged);
    QVERIFY(frameGeometryChangedSpy1.isValid());
    shellSurface1->xdgSurface()->ack_configure(surfaceConfigureRequestedSpy1.last().at(0).value<quint32>());
    Test::render(surface1.data(), toplevelConfigureRequestedSpy1.last().at(0).toSize(), Qt::red);
    QVERIFY(frameGeometryChangedSpy1.wait());

    QScopedPointer<KWayland::Client::Surface> surface2(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface2(Test::createXdgToplevelSurface(surface2.data()));
    QSignalSpy toplevelConfigureRequestedSpy2(shellSurface2.data(), &Test::XdgToplevel::configureRequested);
    QSignalSpy surfaceConfigureRequestedSpy2(shellSurface2->xdgSurface(), &Test::XdgSurface::configureRequested);
    Window *window2 = Test::renderAndWaitForShown(surface2.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window2);
    QVERIFY(window2->isActive());
    QVERIFY(surfaceConfigureRequestedSpy2.wait()); // Wait for the configure event with the activated state.
    workspace()->slotWindowFullScreen();
    QVERIFY(surfaceConfigureRequestedSpy2.wait());
    QSignalSpy frameGeometryChangedSpy2(window2, &Window::frameGeometryChanged);
    QVERIFY(frameGeometryChangedSpy2.isValid());
    shellSurface2->xdgSurface()->ack_configure(surfaceConfigureRequestedSpy2.last().at(0).value<quint32>());
    Test::render(surface2.data(), toplevelConfigureRequestedSpy2.last().at(0).toSize(), Qt::red);
    QVERIFY(frameGeometryChangedSpy2.wait());

    const QList<Window *> stackingOrder = workspace()->stackingOrder();
    QVERIFY(stackingOrder.indexOf(window1) < stackingOrder.indexOf(window2));
    QVERIFY(window1->isFullScreen());
    QVERIFY(window2->isFullScreen());

    // Create several windows on the bottom screen.
    QScopedPointer<KWayland::Client::Surface> surface3(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface3(Test::createXdgToplevelSurface(surface3.data()));
    Window *window3 = Test::renderAndWaitForShown(surface3.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window3);
    QVERIFY(window3->isActive());

    QScopedPointer<KWayland::Client::Surface> surface4(Test::createSurface());
    QScopedPointer<Test::XdgToplevel> shellSurface4(Test::createXdgToplevelSurface(surface4.data()));
    Window *window4 = Test::renderAndWaitForShown(surface4.data(), QSize(100, 50), Qt::blue);
    QVERIFY(window4);
    QVERIFY(window4->isActive());

    window3->move(QPoint(200, 1224));
    window4->move(QPoint(200, 1424));

    // Switch to window above.
    workspace()->switchWindow(Workspace::DirectionNorth);
    QVERIFY(window3->isActive());

    // Switch to window above.
    workspace()->switchWindow(Workspace::DirectionNorth);
    QVERIFY(window2->isActive());

    // Switch to window above.
    workspace()->switchWindow(Workspace::DirectionNorth);
    QVERIFY(window4->isActive());

    // Destroy all windows.
    shellSurface1.reset();
    QVERIFY(Test::waitForWindowDestroyed(window1));
    shellSurface2.reset();
    QVERIFY(Test::waitForWindowDestroyed(window2));
    shellSurface3.reset();
    QVERIFY(Test::waitForWindowDestroyed(window3));
    shellSurface4.reset();
    QVERIFY(Test::waitForWindowDestroyed(window4));
}

void ActivationTest::stackScreensHorizontally()
{
    // Process pending wl_output bind requests before destroying all outputs.
    QTest::qWait(1);

    const QVector<QRect> screenGeometries{
        QRect(0, 0, 1280, 1024),
        QRect(1280, 0, 1280, 1024),
    };

    const QVector<int> screenScales{
        1,
        1,
    };

    QMetaObject::invokeMethod(kwinApp()->platform(),
                              "setVirtualOutputs",
                              Qt::DirectConnection,
                              Q_ARG(int, screenGeometries.count()),
                              Q_ARG(QVector<QRect>, screenGeometries),
                              Q_ARG(QVector<int>, screenScales));
}

void ActivationTest::stackScreensVertically()
{
    // Process pending wl_output bind requests before destroying all outputs.
    QTest::qWait(1);

    const QVector<QRect> screenGeometries{
        QRect(0, 0, 1280, 1024),
        QRect(0, 1024, 1280, 1024),
    };

    const QVector<int> screenScales{
        1,
        1,
    };

    QMetaObject::invokeMethod(kwinApp()->platform(),
                              "setVirtualOutputs",
                              Qt::DirectConnection,
                              Q_ARG(int, screenGeometries.count()),
                              Q_ARG(QVector<QRect>, screenGeometries),
                              Q_ARG(QVector<int>, screenScales));
}

}

WAYLANDTEST_MAIN(KWin::ActivationTest)
#include "activation_test.moc"
