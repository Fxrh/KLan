#ifndef DEF_H
#define DEF_H

#include <QString>

namespace connection
{
  const QString Mess_Ping = "PING";
  const QString Mess_Chat = "CHAT_MESSAGE";
  const QString Mess_Short = "SHORT_MESSAGE";
  const QString Mess_Server = "SERVER";
  const QString Mess_Name = "NAME";
  const quint16 Broadcast_Port = 32695;
}

#endif //DEF_H
