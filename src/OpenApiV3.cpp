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
**函数名 init
**输入:
**          stServer_name  openapi的域名或地址，如果是调试请传入"119.147.19.43"
**          stStat_url : 统计域名，这个最好不要对其进行更改
**          format:  有json和xml,现在SDK只提供JSON的解析库，建议此值不改
**          IsStat: 是否进行统计，建议开启，有助于问题排查
**
**输出:  JsonRes  请求响应包
**
**返回:无
**描述:
**           进行API接口的初始化，调用接口时，请先调用这个接口，否则会出错
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
**函数名 api
**输入:
**          script_name : 调用的API方法，比如/v3/user/get_info，参考 http://wiki.open.qq.com/wiki/API_V3.0%E6%96%87%E6%A1%A3
**          params : 调用API时带的参数
**          method : OPEN_API_V3_METHOD_GET 或OPEN_API_V3_METHOD_POST 其他非法
**          protocol: 协议类型支持HTTP和HTTPS
**          timeout_sec:超时时间
**
**输出:  JsonRes  请求响应包
**
**返回:0 成功
               其他失败
**描述:
**            执行API调用，返回openapi server服务器返回的响应包
**************************************************************/
int OpenApiV3::api(string &script_name, map<string, string> &params, Json::Value &JsonRes , OPEN_API_V3_METHOD method, OPEN_API_V3_PROTOCOL protocol, double timeout_sec)
{

    int iRet = 0;
    m_iRet = 0;

    iRet = ParamIsValid(params, method);
    if (0 != iRet) {
        return iRet;
    }



    // 生成签名
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
        //打印出最终发送到openapi服务器的请求参数以及url
        printRequest(url, params, HttpMethod);
    }

    //记录接口调用开始时间
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

        //对JSON数据进行解析
        m_iRet = JsonResolve(JsonRes, res);
    }


    return m_iRet;
}

/***********************************************************
**函数名 apiUploadFile
**输入:
**          script_name : 调用的API方法，比如/v3/user/get_info，参考 http://wiki.open.qq.com/wiki/API_V3.0%E6%96%87%E6%A1%A3
**          params : 调用API时带的参数
**          FileParam:上传的文件参数
**          protocol: 协议类型支持HTTP和HTTPS
**          timeout_sec:超时时间
**
**输出:  JsonRes  请求响应包
**
**返回:0 成功
               其他失败
**描述:
**            进行文件的上传,返回服务器的响应包
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
        //通过调用以下方法，可以打印出最终发送到openapi服务器的请求参数以及url
        printRequest(url, params, HttpMethod);
    }


    //记录接口调用开始时间
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
**函数名 ParamIsValid
**输入:
**          params  传入参数
             params  get or port
**输出:  无
**
**返回:true   合法
               false  非法
**描述:
**           判断参数是否合法
**************************************************************/
int OpenApiV3::ParamIsValid(map<string, string> &params, OPEN_API_V3_METHOD method)
{
    //判断是否调用了初始化接口
    if (0 == m_initflag) {
        OPENAPIV3_ERROR("must call init");
        return OPENAPI_ERROR_REQUIRED_PARAMETER_EMPTY;
    }

    return 0;
}

/***********************************************************
**函数名 JsonResolve
**输入:
**              res  待解析的字符串
**输出:  JsonRes 解析后的JSON结构体
**
**返回: 解析的返回值
**描述:
**           判断参数是否合法
**************************************************************/
int  OpenApiV3::JsonResolve(Json::Value &JsonRes, string &res)
{
    int iRet = 0;
    //对JSON数据进行解析
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

