#ifndef CONTAINERXMLPARSEREXCEPTION_H
#define CONTAINERXMLPARSEREXCEPTION_H

#include <QException>

class ContainerXmlParserException : public QException
{
public:
    ContainerXmlParserException();

    ContainerXmlParserException(QString message);

    void raise() const
    {
        throw *this;
    }

    ContainerXmlParserException *clone() const
    {
        return new ContainerXmlParserException(*this);
    }
    void setMessage(QString message)
    {
        messageString = message;
    }

    QString message() const { return messageString; }

private:
    QString messageString;
};

#endif // CONTAINERXMLPARSEREXCEPTION_H
