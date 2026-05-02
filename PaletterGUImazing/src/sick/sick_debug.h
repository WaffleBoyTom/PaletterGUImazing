#ifndef WIDGET_DEBUG_H
#define WIDGET_DEBUG_H

#include <QWidget>

void sickDebugGeometry(const QWidget *w, const std::string &name = "");
void sickDebugSizing(const QWidget *w, const std::string &name = "");
void sickDebugLayout(const QWidget *w, const std::string &name = "");

#endif
