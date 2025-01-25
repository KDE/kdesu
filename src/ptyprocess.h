/*
    This file is part of the KDE project, module kdesu.
    SPDX-FileCopyrightText: 1999, 2000 Geert Jansen <jansen@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef KDESUPTYPROCESS_H
#define KDESUPTYPROCESS_H

#include <memory>
#include <sys/types.h>

#include <QByteArray>
#include <QList>
#include <QString>
#include <QStringList>

#include <kdesu/kdesu_export.h>

#include <KPty>

namespace KDESu
{
class PtyProcessPrivate;

/*!
 * \class KDESu::PtyProcess
 * \inmodule KDESu
 * \inheaderfile KDESu/PtyProcess
 *
 * \brief Synchronous communication with tty programs.
 *
 * PtyProcess provides synchronous communication with tty based programs.
 * The communications channel used is a pseudo tty (as opposed to a pipe)
 * This means that programs which require a terminal will work.
 */
class KDESU_EXPORT PtyProcess
{
public:
    /*!
     * Error return values for checkPidExited()
     *
     * \value Error No child
     * \value NotExited Child hasn't exited
     * \value Killed Child terminated by signal
     */
    enum checkPidStatus {
        Error = -1,
        NotExited = -2,
        Killed = -3,
    };

    PtyProcess();
    virtual ~PtyProcess();

    /*!
     * Forks off and execute a command.
     *
     * The command's standard in and output
     * are connected to the pseudo tty. They are accessible with readLine
     * and writeLine.
     *
     * \a command The command to execute.
     *
     * \a args The arguments to the command.
     *
     * Returns 0 on success, -1 on error. errno might give more information then.
     */
    int exec(const QByteArray &command, const QList<QByteArray> &args);

    /*!
     * Reads a line from the program's standard out. Depending on the \a block
     * parameter, this call blocks until something was read.
     *
     * Note that in some situations this function will return less than a full
     * line of output, but never more. Newline characters are stripped.
     *
     * \a block Block until a full line is read?
     *
     * Returns the output string.
     */
    QByteArray readLine(bool block = true);

    /*!
     * Read all available output from the program's standard out.
     *
     * \a block If no output is in the buffer, should the function block
     * (else it will return an empty QByteArray)?
     *
     * Returns the output.
     */
    QByteArray readAll(bool block = true);

    /*!
     * Writes a line of text to the program's standard in.
     *
     * \a line The text to write.
     *
     * \a addNewline Adds a '\n' to the line.
     */
    void writeLine(const QByteArray &line, bool addNewline = true);

    /*!
     * Puts back a line of input.
     *
     * \a line The line to put back.
     *
     * \a addNewline Adds a '\n' to the line.
     */
    void unreadLine(const QByteArray &line, bool addNewline = true);

    /*!
     * Sets the exit string. If a line of program output matches this,
     * waitForChild() will terminate the program and return.
     */
    void setExitString(const QByteArray &exit);

    /*!
     * Waits for the child to exit. See also setExitString.
     */
    int waitForChild();

    /*!
     * Waits until the pty has cleared the ECHO flag. This is useful
     * when programs write a password prompt before they disable ECHO.
     * Disabling it might flush any input that was written.
     */
    int waitSlave();

    /*!
     * Enables/disables local echo on the pseudo tty.
     */
    int enableLocalEcho(bool enable = true);

    /*!
     * Enables/disables terminal output. Relevant only to some subclasses.
     */
    void setTerminal(bool terminal);

    /*!
     * Overwrites the password as soon as it is used. Relevant only to
     * some subclasses.
     */
    void setErase(bool erase);

    /*!
     * Set additinal environment variables.
     */
    void setEnvironment(const QList<QByteArray> &env);

    /*!
     * Returns the filedescriptor of the process.
     */
    int fd() const;

    /*!
     * Returns the pid of the process.
     */
    int pid() const;

    /*
     * This is a collection of static functions that can be
     * used for process control inside kdesu. I'd suggest
     * against using this publicly. There are probably
     * nicer Qt based ways to do what you want.
     */

    /*!
     * Wait \a ms milliseconds (ie. 1/10th of a second is 100ms),
     * using \a fd as a filedescriptor to wait on.
     *
     * Returns
     * select(2)'s result, which is -1 on error, 0 on timeout,
     * or positive if there is data on one of the selected fd's.
     *
     * \a ms must be in the range 0..999 (i.e. the maximum wait
     * duration is 999ms, almost one second).
     */
    static int waitMS(int fd, int ms);

    /*!
     * Basic check for the existence of \a pid.
     *
     * Returns true iff \a pid is an extant process,
     * (one you could kill - see man kill(2) for signal 0).
     */
    static bool checkPid(pid_t pid);

    /*!
     * Check process exit status for process \a pid.
     *
     * If child \a pid has exited, return its exit status,
     * (which may be zero).
     *
     * On error (no child, no exit), return -1.
     *
     * If child \a has not exited, return -2.
     */
    static int checkPidExited(pid_t pid);

protected:
    KDESU_NO_EXPORT explicit PtyProcess(PtyProcessPrivate &dd);

    /* Standard hack to add virtual methods in a BC way. Unused. */
    virtual void virtual_hook(int id, void *data);
    QList<QByteArray> environment() const;

    // KF6 TODO: move to PtyProcessPrivate
    bool m_erase;
    bool m_terminal; /* Indicates running in a terminal, causes additional
                           newlines to be printed after output. Set to @c false
                           in constructors */
    int m_pid; /* PID of child process */
    QByteArray m_command; /* Unused */
    QByteArray m_exitString; /* String to scan for in output that indicates child has exited. */

private:
    KDESU_NO_EXPORT int init();
    KDESU_NO_EXPORT int setupTTY();

protected:
    std::unique_ptr<PtyProcessPrivate> const d_ptr;

private:
    Q_DECLARE_PRIVATE(PtyProcess)
};

}

#endif // KDESUPTYPROCESS_H
