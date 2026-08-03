#ifndef COOLUTIL_DEBUG_H
#define COOLUTIL_DEBUG_H

#include <QWidget>

void coolUtilDebugGeometry(const QWidget *w, const std::string &name = "");
void coolUtilDebugSizing(const QWidget *w, const std::string &name = "");
void coolUtilDebugLayout(const QWidget *w, const std::string &name = "");

#endif // COOLUTIL_DEBUG_H
