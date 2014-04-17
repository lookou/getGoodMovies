#include<iostream>
#include<fstream>
#include<map>
#include<vector>


#include"OpenApiV3.h"


#define SERVER_NAME "api.douban.com"
#define SCRIPT_NAME "/v2/movie/search"
#define MOVIE_NAME_LENGTH 100

using namespace std;

int main(){

	int iRet = 0;

	OpenApiV3 getData;
	map<string, string> v_params;
	map<string, double> v_result;
	vector<string> movies;
	v_params["count"] = "20";
	ifstream f("python/movie.txt");
	char movie_name[MOVIE_NAME_LENGTH] = {0};
	while(f.getline(movie_name, MOVIE_NAME_LENGTH)){
		movies.push_back(movie_name);
	}
	string server = SERVER_NAME;
	getData.init(server);
	Json::Value resJson;
	string script = SCRIPT_NAME;
	for(int i = 0, iMax = movies.size(); i < iMax; i ++){
		v_params["q"] = movies[i];
		iRet = getData.api(script, v_params, resJson);
		if(iRet != 0){
			cout<<"ret:"<<iRet<<endl;
			return 0;
		}
		string key = resJson["subjects"][0U]["title"].asString();
		double value = resJson["subjects"][0U]["rating"]["average"].asDouble();
		v_result[key] = value;
	}

	map<string, double>::iterator itr = v_result.begin();
	for(; itr != v_result.end(); itr ++){
		cout<<itr->first<<"=>"<<itr->second<<endl;
	}

	return 0;
}
