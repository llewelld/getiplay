/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <QtQuick>
#include <sailfishapp.h>
#include "programme.h"
#include "refresh.h"
#include "download.h"
#include "control.h"
#include <QDebug>

#include "GetiPlay.h"

int main(int argc, char *argv[])
{
    int result;
    QFile file;

    // SailfishApp::main() will display "qml/template.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //
    // To display the view, call "show()" (will show fullscreen on device).

    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));

    // These values are used by QSettings to access the config file in
    // /home/nemo/.local/share/flypig/GetiPlay.conf
    QCoreApplication::setOrganizationName("flypig");
    QCoreApplication::setOrganizationDomain("www.flypig.co.uk");
    QCoreApplication::setApplicationName(APP_NAME);

    Control * control = new Control();

    QList<ProgModel*> models;
    ProgModel modelradio;
    ProgModel modeltv;
    models.append(&modelradio);
    models.append(&modeltv);

    file.setFileName(DIR_CONFIG "/radio.txt");
    modelradio.importFromFile(file);
    file.setFileName(DIR_CONFIG "/tv.txt");
    modeltv.importFromFile(file);

    /*
    QFile file("/opt/sdk/GetiPlay/usr/share/GetiPlay/output01.txt");
    if (file.open(QIODevice::ReadOnly)) {
        QRegExp progInfo("^(\\d+):\\t(.*)$");
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            int foundPos = progInfo.indexIn(line);
            if (foundPos > -1) {
                unsigned int progId = progInfo.cap(1).toUInt();
                QString title = progInfo.cap(2);
                model.addProgramme(Programme(progId, title, 0.0));
            }
        }
        file.close();
    }
    */

    QScopedPointer<QQuickView> view(SailfishApp::createView());

    view->setSource(SailfishApp::pathTo("qml/GetiPlay.qml"));

    QQmlContext *ctxt = view->rootContext();
    ctxt->setContextProperty("control", control);

    QSortFilterProxyModel * proxyModelRadio = new QSortFilterProxyModel ();
    proxyModelRadio->setSourceModel(&modelradio);
    proxyModelRadio->setDynamicSortFilter(true);
    proxyModelRadio->setFilterRole(ProgModel::NameRole);
    proxyModelRadio->setFilterCaseSensitivity(Qt::CaseInsensitive);

    QSortFilterProxyModel * proxyModelTV = new QSortFilterProxyModel ();
    proxyModelTV->setSourceModel(&modeltv);
    proxyModelTV->setDynamicSortFilter(true);
    proxyModelTV->setFilterRole(ProgModel::NameRole);
    proxyModelTV->setFilterCaseSensitivity(Qt::CaseInsensitive);

    ctxt->setContextProperty("programmesradio", proxyModelRadio);
    ctxt->setContextProperty("programmestv", proxyModelTV);

    Refresh * refresh = new Refresh (models);
    view->rootContext()->setContextProperty("Refresh", refresh);
    refresh->initialise();

    Download * download = new Download ();
    view->rootContext()->setContextProperty("Download", download);
    download->initialise();

    view->show();
    result = app->exec();

    // Write out the programme lists

    QDir dir;
    dir.mkdir(DIR_CONFIG);
    file.setFileName(DIR_CONFIG "/radio.txt");
    modelradio.exportToFile(file);
    file.setFileName(DIR_CONFIG "/tv.txt");
    modeltv.exportToFile(file);

    delete proxyModelRadio;
    delete proxyModelTV;
    delete control;

    return result;
    /*
    return SailfishApp::main(argc, argv);
    */
}

