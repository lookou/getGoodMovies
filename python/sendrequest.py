import urllib
import json

'''
key_words = ['%E6%95%99%E7%88%B6', 
	'%E5%A4%A9%E5%A0%82%E7%94%B5%E5%BD%B1%E9%99%A2',
	'%E7%BE%8E%E5%9B%BD%E5%BE%80%E4%BA%8B',
	'%E9%A3%98',
	'%E7%BE%8E%E4%B8%BD%E4%BA%BA%E7%94%9F'];
'''
key_words = [];
f = open('movie.txt');
line = f.readline();
while line:
	key_words.append(line);
	line = f.readline();


result = {};
for key in key_words:
	f = urllib.urlopen('https://api.douban.com/v2/movie/search?count=20&q=' + key);

	obj = f.read();
	#print obj;
	oobj = json.loads(obj);
	result[oobj['subjects'][0]['title']] = oobj['subjects'][0]['rating']['average'];

sort_list = sorted(result.iteritems(), key = lambda d:d[1], reverse = True);

for res in sort_list:
	print  res[0], '\t\t', res[1];
