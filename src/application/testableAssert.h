#ifndef TESTABLE_ASSERT_H
#define TESTABLE_ASSERT_H

#include <QString>

/** @file testableAssert.h
 *  This file exports macro T_ASSERT.
 *  Such macro is the same as Q_ASSERT if current build is not inside a
 *  unit test suite, while it throws an exception in case of unit test.
 *  Exception has a QString parameter with a fixed string.
 */

#ifndef  UNIT_TEST

namespace Application {
void Assert(bool c, const char * file, int line);
void Assert(bool c, const QString & msg = "");
}

#define T_ASSERT(c)    Application::Assert( (c), __FILE__, __LINE__)
#define T_ASSERT_REP(c, msg)    Application::Assert( (c), (msg))
#else
#define T_ASSERT(c)    if ((c)==false) throw QString("Assert fail: %1").arg(#c);
#define T_ASSERT_REP(c, msg)    if ((c)==false) throw QString("Assert fail: %1-%2").arg(msg).arg(#c);
#endif


#endif  // TESTABLE_ASSERT_H
