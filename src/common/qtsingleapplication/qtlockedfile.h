/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QTLOCKEDFILE_H
#define QTLOCKEDFILE_H
#include <QtCore/QFile>

#if defined(Q_WS_WIN)
#  if !defined(QT_QTLOCKEDFILE_EXPORT) && !defined(QT_QTLOCKEDFILE_IMPORT)
#    define QT_QTLOCKEDFILE_EXPORT
#  elif defined(QT_QTLOCKEDFILE_IMPORT)
#    if defined(QT_QTLOCKEDFILE_EXPORT)
#      undef QT_QTLOCKEDFILE_EXPORT
#    endif
#    define QT_QTLOCKEDFILE_EXPORT __declspec(dllimport)
#  elif defined(QT_QTLOCKEDFILE_EXPORT)
#    undef QT_QTLOCKEDFILE_EXPORT
#    define QT_QTLOCKEDFILE_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTLOCKEDFILE_EXPORT
#endif

class QT_QTLOCKEDFILE_EXPORT QtLockedFile : public QFile
{
public:    
    enum LockMode { NoLock = 0, ReadLock, WriteLock };
    QtLockedFile();
    QtLockedFile(const QString &name);
    ~QtLockedFile();
    
    bool lock(LockMode mode, bool block = true);
    bool unlock();
    bool isLocked() const;
    LockMode lockMode() const;

private:
#ifdef Q_OS_WIN
    Qt::HANDLE m_semaphore_hnd;
    Qt::HANDLE m_mutex_hnd;
#endif

    LockMode m_lock_mode;
};

#endif // QTLOCKEDFILE_H