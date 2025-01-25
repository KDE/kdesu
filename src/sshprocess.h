/*
    This file is part of the KDE project, module kdesu.
    SPDX-FileCopyrightText: 2000 Geert Jansen <jansen@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef KDESUSSHPROCESS_H
#define KDESUSSHPROCESS_H

#include "stubprocess.h"

#include <kdesu/kdesu_export.h>

namespace KDESu
{
class SshProcessPrivate;

/*!
 * \class KDESu::SshProcess
 * \inmodule KDESu
 * \inheaderfile KDESu/SshProcess
 *
 * \brief Executes a remote command, using ssh.
 */
class KDESU_EXPORT SshProcess : public StubProcess
{
public:
    /*!
     * \value SshNotFound
     * \value SshNeedsPassword
     * \value SshIncorrectPassword
     */
    enum Errors {
        SshNotFound = 1,
        SshNeedsPassword,
        SshIncorrectPassword,
    };

    /*!
     *
     */
    explicit SshProcess(const QByteArray &host = QByteArray(), const QByteArray &user = QByteArray(), const QByteArray &command = QByteArray());
    ~SshProcess() override;

    /*!
     * Sets the target host.
     */
    void setHost(const QByteArray &host);

    /*!
     * Sets the location of the remote stub.
     */
    void setStub(const QByteArray &stub);

    // TODO The return doc is so obviously wrong that the C code needs to be checked.
    /*!
     * Checks if the current user\@host needs a password.
     *
     * Returns the prompt for the password if a password is required. A null
     * string otherwise.
     */
    int checkNeedPassword();

    /*!
     * Checks if the stub is installed and if the password is correct.
     *
     * Returns zero if everything is correct, nonzero otherwise.
     */
    int checkInstall(const char *password);

    /*!
     * Executes the command.
     */
    int exec(const char *password, int check = 0);

    /*!
     *
     */
    QByteArray prompt() const;

    /*!
     *
     */
    QByteArray error() const;

protected:
    void virtual_hook(int id, void *data) override;
    QByteArray display() override;
    QByteArray displayAuth() override;

private:
    KDESU_NO_EXPORT int converseSsh(const char *password, int check);

private:
    Q_DECLARE_PRIVATE(SshProcess)
};

}

#endif // KDESUSSHPROCESS_H
