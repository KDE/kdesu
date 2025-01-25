/*
    This file is part of the KDE project, module kdesu.
    SPDX-FileCopyrightText: 1999, 2000 Geert Jansen <jansen@kde.org>

    SPDX-License-Identifier: GPL-2.0-only

    client.h: client to access kdesud.
*/

#ifndef KDESUCLIENT_H
#define KDESUCLIENT_H

#include <kdesu/kdesu_export.h>

#include <QByteArray>
#include <QList>
#include <memory>

#ifdef Q_OS_UNIX

namespace KDESu
{
/*!
 * \class KDESu::Client
 * \inmodule KDESu
 * \inheaderfile KDESu/Client
 *
 * \brief A client class to access kdesud, the KDE su daemon.
 *
 * Kdesud can assist in password caching in two ways:
 *
 * For high security passwords, like for su and ssh, it executes the
 * password requesting command for you. It feeds the password to the
 * command, without ever returning it to you, the user.
 * See exec, setPass, delCommand.
 *
 * For lower security passwords, like web and ftp passwords, it can act
 * as a persistent storage for string variables. These variables are
 * returned to the user.
 * See setVar, delVar, delGroup.
 *
 * Porting from KF5 to KF6:
 *
 * The class KDESu::KDEsuClient was renamed to KDESu::Client.
 *
 * \since 6.0
 */
class KDESU_EXPORT Client
{
public:
    /*!
     *
     */
    Client();
    ~Client();

    Client(const Client &) = delete;
    Client &operator=(const Client &) = delete;

    /*!
     * Lets kdesud execute a command. If the daemon does not have a password
     * for this command, this will fail and you need to call setPass().
     *
     * \a command The command to execute.
     *
     * \a user The user to run the command as.
     *
     * \a options Extra options.
     *
     * \a env Extra environment variables.
     *
     * Returns Zero on success, -1 on failure.
     */
    int exec(const QByteArray &command, const QByteArray &user, const QByteArray &options = nullptr, const QList<QByteArray> &env = QList<QByteArray>());

    /*!
     * Wait for the last command to exit and return the exit code.
     *
     * Returns the exit code of last command, -1 on failure.
     */
    int exitCode();

    /*!
     * Set root's password, lasts one session.
     *
     * \a pass Root's password.
     *
     * \a timeout The time that a password will live.
     *
     * Returns zero on success, -1 on failure.
     */
    int setPass(const char *pass, int timeout);

    /*!
     * Set the target host (optional).
     */
    int setHost(const QByteArray &host);

    /*!
     * Set the desired priority (optional), see StubProcess.
     */
    int setPriority(int priority);

    /*!
     * Set the desired scheduler (optional), see StubProcess.
     */
    int setScheduler(int scheduler);

    /*!
     * Remove a password for a user/command.
     *
     * \a command The command.
     *
     * \a user The user.
     *
     * Return zero on success, -1 on an error
     */
    int delCommand(const QByteArray &command, const QByteArray &user);

    /*!
     * Set a persistent variable.
     *
     * \a key The name of the variable.
     *
     * \a value Its value.
     *
     * \a timeout The timeout in seconds for this key. Zero means
     * no timeout.
     *
     * \a group Make the key part of a group. See delGroup.
     *
     * Return zero on success, -1 on failure.
     */
    int setVar(const QByteArray &key, const QByteArray &value, int timeout = 0, const QByteArray &group = nullptr);

    /*!
     * Get a persistent variable.
     *
     * \a key The name of the variable.
     *
     * Returns its value.
     */
    QByteArray getVar(const QByteArray &key);

    /*!
     * Gets all the keys that are membes of the given group.
     *
     * \a group the group name of the variables.
     *
     * Returns a list of the keys in the group.
     */
    QList<QByteArray> getKeys(const QByteArray &group);

    /*!
     * Returns true if the specified group exists is
     * cached.
     *
     * \a group the group key
     *
     * Returns true if the group is found
     */
    bool findGroup(const QByteArray &group);

    /*!
     * Delete a persistent variable.
     *
     * \a key The name of the variable.
     *
     * Returns zero on success, -1 on failure.
     */
    int delVar(const QByteArray &key);

    /*!
     * Delete all persistent variables with the given key.
     *
     * A specicalized variant of delVar(QByteArray) that removes all
     * subsets of the cached variables given by \a key. In order for all
     * cached variables related to this key to be deleted properly, the
     * value given to the \a group argument when the setVar function
     * was called, must be a subset of the argument given here and the key
     *
     * \note Simply supplying the group key here WILL not necessarily
     * work. If you only have a group key, then use delGroup instead.
     *
     * \a special_key the name of the variable.
     *
     * Returns zero on success, -1 on failure.
     */
    int delVars(const QByteArray &special_key);

    /*!
     * Delete all persistent variables in a group.
     *
     * \a group the group name. See setVar.
     */
    int delGroup(const QByteArray &group);

    /*!
     * Ping kdesud. This can be used for diagnostics.
     *
     * Returns zero on success, -1 on failure
     */
    int ping();

    /*!
     * Stop the daemon.
     */
    int stopServer();

    /*!
     * Try to start up kdesud
     */
    int startServer();

private:
    KDESU_NO_EXPORT int connect();

    KDESU_NO_EXPORT int command(const QByteArray &cmd, QByteArray *result = nullptr);
    KDESU_NO_EXPORT QByteArray escape(const QByteArray &str);

private:
    std::unique_ptr<class ClientPrivate> const d;
};

} // END namespace KDESu

#endif // Q_OS_UNIX

#endif // KDESUCLIENT_H
