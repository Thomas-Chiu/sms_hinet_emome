VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "SMSSoapService"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Private soapUrl As String

Private Sub Class_Initialize()
    soapUrl = "http://api.every8d.com/API21/SOAP/SMS.asmx"
End Sub

Public Function getConnection(ByVal userID As String, ByVal password As String) As String
    Dim result As String
    Dim soapData As String
    Dim soapResponse As String
    Dim xmlDoc As DOMDocument '�ϥ�DOMDocument�Ц�Project�]�wReferences �yMicrosoft XML, version 2.0�z
    Set xmlDoc = New DOMDocument
    '�Ѧҡyhttp://192.168.1.112/API21/SOAP/SMS.asmx?op=getConnection�z�զXSOAP���
    soapData = "<?xml version='1.0' encoding='utf-8'?>"
    soapData = soapData & "<soap12:Envelope xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xsd='http://www.w3.org/2001/XMLSchema' xmlns:soap12='http://www.w3.org/2003/05/soap-envelope'>"
    soapData = soapData & "<soap12:Body>"
    soapData = soapData & "<getConnection xmlns='http://tempuri.org/'>"
    soapData = soapData & "<userID>" & userID & "</userID>"
    soapData = soapData & "<password>" & password & "</password>"
    soapData = soapData & "</getConnection>"
    soapData = soapData & "</soap12:Body>"
    soapData = soapData & "</soap12:Envelope>"
    soapResponse = httpPostSoap12(soapUrl, soapData)
    xmlDoc.loadXML soapResponse
    result = xmlDoc.selectSingleNode("//getConnectionResult").Text
    
    getConnection = result
End Function

Public Function closeConnection(ByVal sessionKey As String) As String
    Dim result As String
    Dim soapData As String
    Dim soapResponse As String
    Dim xmlDoc As DOMDocument '�ϥ�DOMDocument�Ц�Project�]�wReferences �yMicrosoft XML, version 2.0�z
    Set xmlDoc = New DOMDocument
    '�Ѧҡyhttp://192.168.1.112/API21/SOAP/SMS.asmx?op=closeConnection�z�զXSOAP���
    soapData = "<?xml version='1.0' encoding='utf-8'?>"
    soapData = soapData & "<soap12:Envelope xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xsd='http://www.w3.org/2001/XMLSchema' xmlns:soap12='http://www.w3.org/2003/05/soap-envelope'>"
    soapData = soapData & "<soap12:Body>"
    soapData = soapData & "<closeConnection xmlns='http://tempuri.org/'>"
    soapData = soapData & "<sessionKey>" & sessionKey & "</sessionKey>"
    soapData = soapData & "</closeConnection>"
    soapData = soapData & "</soap12:Body>"
    soapData = soapData & "</soap12:Envelope>"
    soapResponse = httpPostSoap12(soapUrl, soapData)
    xmlDoc.loadXML soapResponse
    result = xmlDoc.selectSingleNode("//closeConnectionResult").Text
    closeConnection = result
End Function

Public Function sendSMS(ByVal sessionKey As String, ByVal subject As String, ByVal content As String, ByVal mobile As String, ByVal sendTime As String) As String
    Dim result As String
    Dim soapData As String
    Dim soapResponse As String
    Dim xmlDoc As DOMDocument '�ϥ�DOMDocument�Ц�Project�]�wReferences �yMicrosoft XML, version 2.0�z
    Set xmlDoc = New DOMDocument
    '�Ѧҡyhttp://192.168.1.112/API21/SOAP/SMS.asmx?op=sendSMS�z�զXSOAP���
    soapData = "<?xml version='1.0' encoding='utf-8'?>"
    soapData = soapData & "<soap12:Envelope xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xsd='http://www.w3.org/2001/XMLSchema' xmlns:soap12='http://www.w3.org/2003/05/soap-envelope'>"
    soapData = soapData & "<soap12:Body>"
    soapData = soapData & "<sendSMS xmlns='http://tempuri.org/'>"
    soapData = soapData & "<sessionKey>" & sessionKey & "</sessionKey>"
    soapData = soapData & "<subject>" & subject & "</subject>"
    soapData = soapData & "<content>" & content & "</content>"
    soapData = soapData & "<mobile>" & mobile & "</mobile>"
    soapData = soapData & "<sendTime>" & sendTime & "</sendTime>"
    soapData = soapData & "</sendSMS>"
    soapData = soapData & "</soap12:Body>"
    soapData = soapData & "</soap12:Envelope>"
    soapResponse = httpPostSoap12(soapUrl, soapData)
    xmlDoc.loadXML soapResponse
    result = xmlDoc.selectSingleNode("//sendSMSResult").Text
    sendSMS = result
End Function

Public Function getCredit(ByVal sessionKey As String) As String
    Dim result As String
    Dim soapData As String
    Dim soapResponse As String
    Dim xmlDoc As DOMDocument '�ϥ�DOMDocument�Ц�Project�]�wReferences �yMicrosoft XML, version 2.0�z
    Set xmlDoc = New DOMDocument
    '�Ѧҡyhttp://192.168.1.112/API21/SOAP/SMS.asmx?op=getCredit�z�զXSOAP���
    soapData = "<?xml version='1.0' encoding='utf-8'?>"
    soapData = soapData & "<soap12:Envelope xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xsd='http://www.w3.org/2001/XMLSchema' xmlns:soap12='http://www.w3.org/2003/05/soap-envelope'>"
    soapData = soapData & "<soap12:Body>"
    soapData = soapData & "<getCredit xmlns='http://tempuri.org/'>"
    soapData = soapData & "<sessionKey>" & sessionKey & "</sessionKey>"
    soapData = soapData & "</getCredit>"
    soapData = soapData & "</soap12:Body>"
    soapData = soapData & "</soap12:Envelope>"
    soapResponse = httpPostSoap12(soapUrl, soapData)
    xmlDoc.loadXML soapResponse
    result = xmlDoc.selectSingleNode("//getCreditResult").Text
    getCredit = result
End Function

Private Function httpPostSoap12(ByVal url As String, ByVal postData As String) As String
    Dim result As String
    result = ""
    Dim winHttp As WinHttpRequest   '�ϥ�WinHttpRequest�Ц�Project�]�wReferences �yMicrosoft WinHTTP Services, version 5.1�z
    Set winHttp = New WinHttpRequest
    winHttp.Open "POST", url, False
    winHttp.SetRequestHeader "Content-Type", "application/soap+xml; charset=utf-8"
    winHttp.SetRequestHeader "Content-Length", Len(postData)
    winHttp.Send (postData)
    result = winHttp.ResponseText
    httpPostSoap12 = result
End Function