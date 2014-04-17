/**
 * C++ SDK for  OpenAPI V3
 *
 * @version 3.0.0
 * @author open.qq.com
 * @copyright (c) 2013, Tencent Corporation. All rights reserved.
 * @ History:
 *               3.0.0 | jixingguan | 2013-05-06 10:18:11 | initialization
 */
#include "OpenApiV3.h"
#include "http_request_helper.h"
#include "openapi_comm_def.h"
#include <sys/time.h>

const string HTTP_METHOD_POST = "POST";
const string HTTP_METHOD_GET =  "GET";
const string PROTOCOL_HTTP = "http";
const string PROTOCOL_HTTPS = "https";

/***********************************************************
**������ init
**����:
**          stServer_name  openapi���������ַ������ǵ����봫��"119.147.19.43"
**          stStat_url : ͳ�������������ò�Ҫ������и���
**          format:  ��json��xml,����SDKֻ�ṩJSON�Ľ����⣬�����ֵ����
**          IsStat: �Ƿ����ͳ�ƣ����鿪���������������Ų�
**
**���:  JsonRes  ������Ӧ��
**
**����:��
**����:
**           ����API�ӿڵĳ�ʼ�������ýӿ�ʱ�����ȵ�������ӿڣ���������
**************************************************************/
void OpenApiV3::init(string stServer_name, string stStat_url, bool IsStat)
{
    if (0 == m_initflag) {
        m_server_name = stServer_name;
        m_stat_url = stStat_url;
        m_initflag = 1;
        m_is_stat = IsStat;
        m_debugswitch = 0;
    }
}


/***********************************************************
**������ api
**����:
**          script_name : ���õ�API����������/v3/user/get_info���ο� http://wiki.open.qq.com/wiki/API_V3.0%E6%96%87%E6%A1%A3
**          params : ����APIʱ���Ĳ���
**          method : OPEN_API_V3_METHOD_GET ��OPEN_API_V3_METHOD_POST �����Ƿ�
**          protocol: Э������֧��HTTP��HTTPS
**          timeout_sec:��ʱʱ��
**
**���:  JsonRes  ������Ӧ��
**
**����:0 �ɹ�
               ����ʧ��
**����:
**            ִ��API���ã�����openapi server���������ص���Ӧ��
**************************************************************/
int OpenApiV3::api(string &script_name, map<string, string> &params, Json::Value &JsonRes , OPEN_API_V3_METHOD method, OPEN_API_V3_PROTOCOL protocol, double timeout_sec)
{

    int iRet = 0;
    m_iRet = 0;

    iRet = ParamIsValid(params, method);
    if (0 != iRet) {
        return iRet;
    }



    // ����ǩ��
    string HttpMethod;
    if (OPEN_API_V3_METHOD_GET ==  method) {
        HttpMethod = HTTP_METHOD_GET;
    } else {

        HttpMethod = HTTP_METHOD_POST;
    }



    string url;
    if (OPEN_API_V3_METHOD_HTTP == protocol) {
        url = PROTOCOL_HTTP + "://" + m_server_name + script_name;
    } else {
        url = PROTOCOL_HTTPS + "://" + m_server_name + script_name;
    }

    if (m_debugswitch) {
        //��ӡ�����շ��͵�openapi����������������Լ�url
        printRequest(url, params, HttpMethod);
    }

    //��¼�ӿڵ��ÿ�ʼʱ��
    struct timeval start_tv;
    gettimeofday(&start_tv, NULL);

    string res;
    if (OPEN_API_V3_METHOD_GET  == method) {
        iRet = HttpRequestApi.GetHttpRequest(url, params, timeout_sec, res);
    } else {
        iRet = HttpRequestApi.PostHttpRequest(url, params, timeout_sec, res);
    }


    if (0 != iRet) {
        OPENAPIV3_ERROR("PostHttpRequest failed err[%s]", HttpRequestApi.GetErrMsg());
        m_iRet = (iRet + OPENAPI_ERROR_CURL);
    } else {

        if (m_debugswitch) {
            OutPutDebug("JsonRes=%s\n", res.c_str());
        }

        //��JSON���ݽ��н���
        m_iRet = JsonResolve(JsonRes, res);
    }


    return m_iRet;
}

/***********************************************************
**������ apiUploadFile
**����:
**          script_name : ���õ�API����������/v3/user/get_info���ο� http://wiki.open.qq.com/wiki/API_V3.0%E6%96%87%E6%A1%A3
**          params : ����APIʱ���Ĳ���
**          FileParam:�ϴ����ļ�����
**          protocol: Э������֧��HTTP��HTTPS
**          timeout_sec:��ʱʱ��
**
**���:  JsonRes  ������Ӧ��
**
**����:0 �ɹ�
               ����ʧ��
**����:
**            �����ļ����ϴ�,���ط���������Ӧ��
**************************************************************/
int  OpenApiV3::apiUploadFile(string &script_name, map<string, string> &params, Json::Value &JsonRes, struct FormFileElement &FileParam, OPEN_API_V3_PROTOCOL protocol, double timeout_sec)
{
    int iRet = 0;
    m_iRet = 0;

    iRet = ParamIsValid(params, OPEN_API_V3_METHOD_POST);
    if (0 != iRet) {
        return iRet;
    }



    string HttpMethod = HTTP_METHOD_POST;

    string url;
    if (OPEN_API_V3_METHOD_HTTP == protocol) {
        url = PROTOCOL_HTTP + "://" + m_server_name + script_name;
    } else {
        url = PROTOCOL_HTTPS + "://" + m_server_name + script_name;
    }

    if (m_debugswitch) {
        //ͨ���������·��������Դ�ӡ�����շ��͵�openapi����������������Լ�url
        printRequest(url, params, HttpMethod);
    }


    //��¼�ӿڵ��ÿ�ʼʱ��
    struct timeval start_tv;
    gettimeofday(&start_tv, NULL);

    string res;
    iRet = HttpRequestApi.PostHttpRequestWithFile(url, params, FileParam, timeout_sec, res);
    if (0 != iRet) {
        OPENAPIV3_ERROR("PostHttpRequest failed err[%s]", HttpRequestApi.GetErrMsg());
        m_iRet = (OPENAPI_ERROR_CURL + iRet);
    } else {
        if (m_debugswitch) {
            OutPutDebug("JsonRes=%s\n", res.c_str());
        }
        m_iRet = JsonResolve(JsonRes, res);

    }


    return m_iRet;
}

/***********************************************************
**������ ParamIsValid
**����:
**          params  �������
             params  get or port
**���:  ��
**
**����:true   �Ϸ�
               false  �Ƿ�
**����:
**           �жϲ����Ƿ�Ϸ�
**************************************************************/
int OpenApiV3::ParamIsValid(map<string, string> &params, OPEN_API_V3_METHOD method)
{
    //�ж��Ƿ�����˳�ʼ���ӿ�
    if (0 == m_initflag) {
        OPENAPIV3_ERROR("must call init");
        return OPENAPI_ERROR_REQUIRED_PARAMETER_EMPTY;
    }

    return 0;
}

/***********************************************************
**������ JsonResolve
**����:
**              res  ���������ַ���
**���:  JsonRes �������JSON�ṹ��
**
**����: �����ķ���ֵ
**����:
**           �жϲ����Ƿ�Ϸ�
**************************************************************/
int  OpenApiV3::JsonResolve(Json::Value &JsonRes, string &res)
{
    int iRet = 0;
    //��JSON���ݽ��н���
    Json::Reader _reader;

    if (!_reader.parse(res, JsonRes)) {
        OPENAPIV3_ERROR("Json::Reader parse Error, Msg=%s", _reader.getFormatedErrorMessages().c_str());
        iRet = OPENAPI_ERROR_RESPONSE_DATA_INVALID;
    } else {
        iRet = JsonRes["ret"].asInt();
        if (0 != iRet) {
            OPENAPIV3_ERROR("%s", JsonRes["msg"].asString().c_str());
        }
    }
    return iRet;
}



void OpenApiV3::printRequest(const string &url, map<string, string> &params, const string &method)
{
    string urltemp;
    urltemp = url;
    OutPutDebug("\n============= request info ================\n\n");
    OutPutDebug("method : %s\n", method.c_str());
    OutPutDebug("url    : %s\n", urltemp.c_str());
    if (method == HTTP_METHOD_POST) {
    }
    OutPutDebug("\n");
}
