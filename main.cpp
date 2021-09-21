#include <QApplication>
#include <QFutureInterface>
#include <QFuture>
#include <QFutureWatcher>
#include <QProgressDialog>

#include "runextensions.h"

void func(QFutureInterface<void> &future)
{
    const int iterations = 100000000;
    for (int i = 0; i < iterations; ++i) {
        // long operation:)
        QString("bla bla bla").detach();

        if (future.isCanceled())
            return;
        future.setProgressValue((double)i/iterations*100);
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QProgressDialog dialog;
    dialog.setLabelText("Running");
    dialog.show();

    QFutureWatcher<void> watcher;

    QObject::connect(&dialog, &QProgressDialog::canceled,
                     &watcher, &QFutureWatcher<void>::cancel);
    QObject::connect(&watcher, &QFutureWatcher<void>::finished,
                     &dialog, &QProgressDialog::close);
    QObject::connect(&watcher, &QFutureWatcher<void>::progressValueChanged,
                     &dialog, &QProgressDialog::setValue);

    watcher.setFuture(QtConcurrent::run(&func));

    return app.exec();
}
