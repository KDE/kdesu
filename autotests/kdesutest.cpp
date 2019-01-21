/*
 * Copyright 2019 Jonathan Riddell <jr@jriddell.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#define MYPASSWORD "ilovekde"
#define ROOTPASSWORD "ilovekde"
#include "config-kdesutest.h"

#include <QObject>
#include <QTest>
#include <QString>

#include <kconfig.h>
#include <kconfiggroup.h>
#include <ksharedconfig.h>

#include "suprocess.h"

namespace KDESu
{

class KdeSuTest: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase() {
        QStandardPaths::setTestModeEnabled(true);
    }

    void editConfig(QString command, QString commandPath) {
        KSharedConfig::Ptr config = KSharedConfig::openConfig();
        KConfigGroup group(config, "super-user-command");
        group.writeEntry("super-user-command", command);
        QString kdesuStubPath = QString::fromLocal8Bit(CMAKE_RUNTIME_OUTPUT_DIRECTORY) + QString::fromLocal8Bit("/kdesu_stub");
        group.writeEntry("kdesu_stub_path", kdesuStubPath);
        group.writeEntry("command", commandPath);
    }

    void sudoGoodPassword() {
        editConfig(QString::fromLocal8Bit("sudo"), QString::fromLocal8Bit(CMAKE_HOME_DIRECTORY) + QString::fromLocal8Bit("/autotests/sudo"));

        KDESu::SuProcess *suProcess = new KDESu::SuProcess("root", "ls");
        QString suapp = suProcess->superUserCommand();
        QVERIFY(suapp==QLatin1String("sudo"));
        int result = suProcess->exec(MYPASSWORD, 0);
        QVERIFY(result == 0);
    }

    void sudoBadPassword() {
        editConfig(QString::fromLocal8Bit("sudo"), QString::fromLocal8Bit(CMAKE_HOME_DIRECTORY) + QString::fromLocal8Bit("/autotests/sudo"));

        KDESu::SuProcess *suProcess = new KDESu::SuProcess("root", "ls");
        QString suapp = suProcess->superUserCommand();
        QVERIFY(suapp==QLatin1String("sudo"));
        int result2 = suProcess->exec("broken", 0);
        QVERIFY(result2 == KDESu::SuProcess::SuIncorrectPassword);
    }

    void suGoodPassword() {
        editConfig(QString::fromLocal8Bit("su"), QString::fromLocal8Bit(CMAKE_HOME_DIRECTORY) + QString::fromLocal8Bit("/autotests/su"));

        KDESu::SuProcess *suProcess = new KDESu::SuProcess("root", "ls");
        QString suapp = suProcess->superUserCommand();
        QVERIFY(suapp==QLatin1String("su"));
        int result2 = suProcess->exec(ROOTPASSWORD, 0);
        QVERIFY(result2 == 0);
    }

    void suBadPassword() {
        editConfig(QString::fromLocal8Bit("su"), QString::fromLocal8Bit(CMAKE_HOME_DIRECTORY) + QString::fromLocal8Bit("/autotests/su"));

        KDESu::SuProcess *suProcess = new KDESu::SuProcess("root", "ls");
        QString suapp = suProcess->superUserCommand();
        QVERIFY(suapp==QLatin1String("su"));
        int result2 = suProcess->exec("broken", 0);
        QVERIFY(result2 == KDESu::SuProcess::SuIncorrectPassword);
    }
};
}

#include <kdesutest.moc>
QTEST_MAIN(KDESu::KdeSuTest)
