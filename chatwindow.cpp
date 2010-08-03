#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableFormat>
#include <QCloseEvent>
#include <QScrollBar>
#include <KPushButton>
#include <KNotification>
#include <kdebug.h>

#include "chatwindow.h"
#include "connection/connectionobject.h"

ChatWindow::ChatWindow( QString myName, ConnectionObject* connection, QWidget* parent)
    : QWidget(parent),
      m_myName(myName),
      m_chatConnection(connection)      
{
  m_conName = QString(m_chatConnection->getIp() + ":" + QString::number(m_chatConnection->getClientPort()) );
  setupGui();
  
  m_tableFormat = new QTextTableFormat();
  m_tableFormat->setBorder(0);
  
  connect( m_sendBtn, SIGNAL(clicked()), this, SLOT(sendMessage()) );
  connect( m_messageEdit, SIGNAL(returnPressed()), m_sendBtn, SLOT(click()) );
  connect( m_chatConnection, SIGNAL(sigChange(ConnectionObject*)), this, SLOT(connectionChanged()) );
  connect( m_chatConnection, SIGNAL(destroyed()), this, SLOT(connectionDestroyed()) );
}

ChatWindow::~ChatWindow()
{
}

void ChatWindow::newMessage(QString mess)
{
  writeMessage(ConMess, mess);
}

void ChatWindow::myNameChanged(QString name)
{
  if( m_myName == name ){
    return;
  }
  writeMessage(StatusMess, QString("%1 changed name to %2").arg(m_myName, name) );
  m_myName = name;
}

void ChatWindow::conNameChanged(QString name)
{
  if( m_conName == name ){
    return;
  }
  writeMessage(StatusMess, QString("%1 changed name to %2").arg(m_conName, name) );
  m_conName = name;
}

//void ChatWindow::connected()
//{
//  if( ! m_sendBtn->isEnabled() ){
//    m_sendBtn->setEnabled(true);
//    writeMessage(StatusMess, QString("Verbunden mit %1").arg(m_conName) );
//  }
//}
//
//void ChatWindow::disconnected()
//{
//  if( m_sendBtn->isEnabled() ){
//    m_sendBtn->setEnabled(false);
//    writeMessage( StatusMess, QString("Verbindung unterbrochen") );
//  }
//}

void ChatWindow::closeEvent(QCloseEvent* event)
{
  if( m_chatConnection == 0 ){
    event->ignore();
    hide();
    emit sigDestroy(this);
  }
}

void ChatWindow::sendMessage()
{
  QString message = m_messageEdit->text();
  writeMessage(MyMess, message);
  m_messageEdit->setText("");
  emit sigMessage(message, m_chatConnection );
}

void ChatWindow::connectionChanged()
{
  if( m_chatConnection->isConnected() && ! m_sendBtn->isEnabled() ){
    m_sendBtn->setEnabled(true);
    writeMessage( StatusMess, QString("Connected to %1").arg(m_conName) );
  }
  if( !m_chatConnection->isConnected() && m_sendBtn->isEnabled() ){
    m_sendBtn->setEnabled(false);
    writeMessage( StatusMess, QString("Disconnected from %1").arg(m_conName) );
  }
}

void ChatWindow::connectionDestroyed()
{
  m_chatConnection = 0;
  if( isHidden() ){
    emit sigDestroy(this);
  }
}

void ChatWindow::setupGui()
{
  m_chatEdit = new QTextEdit();
  m_chatEdit->setReadOnly(true);
  
  m_messageEdit = new QLineEdit();
  m_messageEdit->setMaxLength(500);
  
  m_sendBtn = new KPushButton("Send");
  //m_sendBtn->setEnabled(false);
  
  m_sendLayout = new QHBoxLayout();
  m_sendLayout->addWidget(m_messageEdit);
  m_sendLayout->addWidget(m_sendBtn);
  
  m_layout = new QVBoxLayout();
  m_layout->addWidget(m_chatEdit);
  m_layout->addLayout(m_sendLayout);
  
  setLayout(m_layout);
  resize(600, 300);
  
  show();
}

void ChatWindow::writeMessage(MessType type, QString mess )
{
  if( mess.isEmpty() ){
    return;
  }
  
  QTextCursor cursor( m_chatEdit->textCursor() );
  cursor.movePosition(QTextCursor::End);
  QTextTable* table = cursor.insertTable(1, 2, *m_tableFormat );
  QTextCharFormat charFormat;
  
  switch( type ){
    case MyMess :
      charFormat.setForeground(Qt::blue);
      table->cellAt(0,0).firstCursorPosition().insertText("<" + m_myName + "> ", charFormat);
      table->cellAt(0,1).firstCursorPosition().insertText(mess);
      break;
    case ConMess :
      charFormat.setForeground(Qt::darkGreen);
      table->cellAt(0,0).firstCursorPosition().insertText("<" + m_conName + "> ", charFormat);
      table->cellAt(0,1).firstCursorPosition().insertText(mess);
      break;
    case StatusMess :     
      charFormat.setForeground(Qt::darkRed);
      cursor.insertText(" *** " + mess + " *** ", charFormat);
      break;
    default :
      kWarning() << "Wrong MessType";
      break;
  }
  QScrollBar *bar = m_chatEdit->verticalScrollBar();
  bar->setValue(bar->maximum());
}

void ChatWindow::messNotify()
{
  KNotification* notification = new KNotification("chat");
  notification->sendEvent();
  notification->deleteLater();
}

//void ChatWindow::closeEvent(QCloseEvent* event )
//{
//  this->hide();
//  event->ignore();
//}
