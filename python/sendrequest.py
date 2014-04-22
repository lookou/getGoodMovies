import sys, urllib, json

key_words = [];
f = open('movie.txt');
line = f.readline();
while line:
	key_words.append(line);
	line = f.readline();


result = {};
i = 1;
for key in key_words:
	f = urllib.urlopen('https://api.douban.com/v2/movie/search?count=20&q=' + urllib.quote(key));

	obj = f.read();
	oobj = json.loads(obj);
	if 'subjects' not in oobj:
		print obj;
		continue;
	if len(oobj['subjects']) == 0:
		print 'not found:', urllib.quote(key), '|index:', i;
		print '###################################';
		print oobj;
		print '###################################';
	else:
		result[oobj['subjects'][0]['title']] = oobj['subjects'][0]['rating']['average'];
	i += 1;

sort_list = sorted(result.iteritems(), key = lambda d:d[1], reverse = True);

for res in sort_list:
	print  res[0], '\t\t', res[1];
