#include<iostream>
#include<map>


#include"OpenApiV3.h"


#define SERVER_NAME "api.douban.com"
#define SCRIPT_NAME "/v2/movie/search"


using namespace std;




int main(){

	OpenApiV3 getData;
	map<string, string> v_params;
	v_params["count"] = "20";
	v_params["q"] = "%E6%95%99%E7%88%B6";
	string server = SERVER_NAME;
	getData.init(server);
	Json::Value resJson;
	string script = SCRIPT_NAME;
	getData.api(script, v_params, resJson);

	Json::FastWriter w;
	cout<<w.write(resJson)<<endl;
	return 0;
}
