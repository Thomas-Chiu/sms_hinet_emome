VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "SMSSoap"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Private smsService As SMSSoapService
Private sessionKey As String
Private batchID As String
Private credit As Double
Private processMsg As String

Private Sub Class_Initialize()
    Set smsService = New SMSSoapService
    sessionKey = ""
End Sub


''' <summary>
''' ���ѱҰʳs���A�ȡA�ǰe²�T�e���������o���oSessionKey�A
''' �ǥѦ�SessionKey�i�����²�T�ǰe���A�ȡASessionKey���Ĵ������A�j��Y���ġC
''' ��ĳ�󧹦��ǰe�@�~��A����closeConnection�H�T�O�w���C
''' </summary>
''' <param name="userID">�n�J�b��</param>
''' <param name="password">�K�X</param>
''' <returns>true:���o�s�u���\�Ffalse:���o�s�u����</returns>
''' <remarks></remarks>
Public Function getConnection(ByVal userID As String, ByVal password As String) As Boolean
    Dim success As Boolean
    success = False
    Dim resultXml As String
    resultXml = smsService.getConnection(userID, password)
    Dim xmlDoc As DOMDocument '�ϥ�DOMDocument�Ц�Project�]�wReferences �yMicrosoft XML, version 2.0�z
    Set xmlDoc = New DOMDocument
    xmlDoc.loadXML (resultXml)
    Dim returnCode As String
    returnCode = xmlDoc.selectSingleNode("/SMS/GET_CONNECTION/CODE").Text
    If returnCode = "0" Then
        sessionKey = xmlDoc.selectSingleNode("/SMS/GET_CONNECTION/SESSION_KEY").Text
        processMsg = xmlDoc.selectSingleNode("/SMS/GET_CONNECTION/DESCRIPTION").Text
        success = True
    Else
        processMsg = xmlDoc.selectSingleNode("/SMS/GET_CONNECTION/DESCRIPTION").Text
    End If
   getConnection = success
End Function

''' <summary>
''' SessionKey���Ĵ������A�j��Y���ġC��ĳ�󧹦��ǰe�@�~��A����closeConnection�H�T�O�w��
''' </summary>
''' <returns>true:�������\�Ffalse:��������</returns>
''' <remarks></remarks>
Public Function closeConnection() As Boolean
    Dim success As Boolean
    success = False
    Dim resultString As String
    resultString = smsService.closeConnection(sessionKey)
    If Not Mid(resultString, 1, 1) = "-" Then
        sessionKey = ""
        success = True
    End If
    closeConnection = success
End Function

''' <summary>
''' �ǰe²�T
''' </summary>
''' <param name="subject">²�T�D���A�D�����|�H��²�T���e�o�e�X�h�C�ΥH���O�����o�e���γ~�C�i�ǤJ�Ŧr��C</param>
''' <param name="content">²�T�o�e���e</param>
''' <param name="mobile">�����H��������X�C�榡��: +886912345678��09123456789�C�h�������H�ɡA�ХH�b�γr�I�j�}( , )�A�p0912345678,0922333444�C</param>
''' <param name="sendTime">²�T�w�w�o�e�ɶ��C-�ߧY�o�e�G�жǤJ�Ŧr��C-�w���o�e�G�жǤJ�w�p�o�e�ɶ��A�Y�ǰe�ɶ��p��t�α���ɶ��A�N�����ǰe�C�榡��YYYYMMDDhhmnss�F�Ҧp:�w��2009/01/31 15:30:00�o�e�A�h�ǤJ20090131153000�C�Y�ǻ��ɶ��w�O�{�b���ɶ��A�N�ߧY�o�e�C</param>
''' <returns>true:�ǰe���\�Ffalse:�ǰe����</returns>
''' <remarks></remarks>
Public Function sendSMS(ByVal subject As String, ByVal content As String, ByVal mobile As String, ByVal sendTime As String) As Boolean
    Dim success As Boolean
    success = False

    Dim resultString As String
    resultString = smsService.sendSMS(sessionKey, subject, content, mobile, sendTime)
    If Not Mid(resultString, 1, 1) = "-" Then
        ''�ǰe���\ �^�Ǧr�ꤺ�e�榡���GCREDIT,SENDED,COST,UNSEND,BATCH_ID�A�U�Ȥ����H�r�����j�C
        '' CREDIT�G�o�e��Ѿl�I�ơC�t�ȥN���o�e���ѡA�t�εL�k�B�z�өR�O
        '' SENDED�G�o�e�q�ơC
        '' COST�G�����o�e�����I��
        '' UNSEND�G�L�B�׮ɵo�e���q�ơA���ӭȤj��0�ӳѾl�I�Ƶ���0�ɪ��ܦ�������²�T�]�L�B�צӵL�k�Q�o�e�C
        '' BATCH_ID�G�妸�ѧO�N�X�C���@�ߤ@�ѧO�X�A�i�ǥѥ��ѧO�X�d�ߵo�e���A�C�榡�d�ҡG220478cc-8506-49b2-93b7-2505f651c12e
        Dim splitStr() As String
        splitStr() = split(resultString, ",")
        credit = CDbl(splitStr(0))
        batchID = splitStr(4)
        success = True
    Else
        ''�ǰe����
        processMsg = resultString
    End If
    sendSMS = success
End Function

''' <summary>
''' ���o�b���l�B
''' </summary>
''' <returns>true:���o���\�Ffalse:���o����</returns>
''' <remarks></remarks>
Public Function getCredit() As Boolean
    Dim success As Boolean
    success = False
    Dim resultString As String
    resultString = smsService.getCredit(sessionKey)
    If Not Mid(resultString, 1, 1) = "-" Then
        credit = CDbl(resultString)
        success = True
    Else
        processMsg = resultString
    End If
    getCredit = success
End Function

Public Function isConnected() As Boolean
    If Not sessionKey = "" Then
        isConnected = True
    End If
    isConnected = False
End Function

Public Function getProcessMsg() As String
    getProcessMsg = processMsg
End Function

Public Function getBatchID() As String
    getBatchID = batchID
End Function

Public Function getCreditValue() As Double
    getCreditValue = credit
End Function