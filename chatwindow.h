#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QString>

class QVBoxLayout;
class QHBoxLayout;
class QTextEdit;
class QLineEdit;
class QTextTableFormat;
class QCloseEvent;
class KPushButton;
class ConnectionObject;

class ChatWindow : public QWidget
{
    Q_OBJECT
  public:
    enum MessType { MyMess, ConMess, StatusMess };
    ChatWindow( QString myName, ConnectionObject* connection, QWidget* parent=0);
    ~ChatWindow();
    
  public slots:
    void newMessage(QString mess);
    void myNameChanged(QString name);
    void conNameChanged(QString name);
//    void connected();
//    void disconnected();
    
  signals:
    void sigMessage(QString mess, ConnectionObject* connection);
    void sigDestroy( ChatWindow* window );
    
protected:
    void closeEvent(QCloseEvent* event);
    
  private slots:
    void sendMessage();
    void connectionChanged();
    void connectionDestroyed();
    
  private:
    void setupGui();
    void writeMessage(MessType type, QString mess);
//    void closeEvent(QCloseEvent* event);
    void messNotify();
    
    QString m_myName;
    QString m_conName;
    QVBoxLayout* m_layout;
    QHBoxLayout* m_sendLayout;
    QTextEdit* m_chatEdit;
    QLineEdit* m_messageEdit;
    KPushButton* m_sendBtn;
    QTextTableFormat* m_tableFormat;
    
    ConnectionObject* m_chatConnection;
    
};

#endif //CHATWINDOW_H
