/* vi: ts=8 sts=4 sw=4
 *
 * $Id$
 *
 * This file is part of the KDE project, module kdesu.
 * Copyright (C) 1999,2000 Geert Jansen <jansen@kde.org>
 */

#ifndef __Lexer_h_included__
#define __Lexer_h_included__

class QCString;

/**
 * This is a lexer for the kdesud protocol.
 */

class Lexer {
public:
    Lexer(const QCString &input);
    ~Lexer();

    /**
     * Read next token.
     */
    int lex();

    /**
     * Return the token's semantic value. A reference is returned because it
     * might contains sensitive information, that I don't want to copy.
     */
    QCString &lval();

    enum Tokens { 
	Tok_none, 
	Tok_exec=256, Tok_pass, Tok_user, Tok_delCmd,
	Tok_ping, Tok_str, Tok_num , Tok_stop,
	Tok_set, Tok_get, Tok_delVar, Tok_host, 
	Tok_prio, Tok_sched
    };

private:
    QCString m_Input;
    QCString m_Output;

    int in;
};

#endif