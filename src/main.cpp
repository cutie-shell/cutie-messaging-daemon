#include "messaging.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    MessagingDaemon messagingDaemon(&app);
    app.exec();
}
