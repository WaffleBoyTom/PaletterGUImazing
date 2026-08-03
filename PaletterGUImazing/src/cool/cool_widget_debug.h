#ifndef COOLUTIL_DEBUG_H
#define COOLUTIL_DEBUG_H

#include <QWidget>

void coolDebugGeometry(const QWidget *w, const std::string &name = "");
void coolDebugSizing(const QWidget *w, const std::string &name = "");
void coolDebugLayout(const QWidget *w, const std::string &name = "");

#endif // COOLUTIL_DEBUG_H
