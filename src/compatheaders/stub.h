/*
    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include <kdesu/kdesu_export.h>

// KDE4 compat header
#if KDESU_ENABLE_DEPRECATED_SINCE(5, 105)
#include "stubprocess.h"
#if KDESU_DEPRECATED_WARNINGS_SINCE >= 0x056900
#pragma message("Deprecated header. Since 5.0, use #include <KDESu/StubProcess> instead")
#endif
#else
#error "Include of deprecated header is disabled"
#endif
